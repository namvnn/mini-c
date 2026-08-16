#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 2048

//  CLIENT                             SERVER
//
// socket()                           socket()
//    |                                  |
//    | active                           | active
//    |                                  |
// connect()                          listen()
//    |                                  |
//    | -------------------------------> |
//    |                               accept()
//    |                                  |
//    +----------- connection -----------+

int main(void) {
    char req[BUFFER_SIZE];
    char resp[BUFFER_SIZE] =
        "HTTP/1.0 200 OK\r\n"
        "Server: http-webserver-c\r\n"
        "Content-Type: text/html\r\n\r\n";
    size_t resp_prefix_length = strlen(resp);

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("HTTP webserver (socket)");
        return 1;
    }
    printf("socket created successfully\n");

    // Set socket option to enable local address reuse
    // Issue: Address already in use
    // Reason: Socket can enter a TIME_WAIT state
    // Debug: netstat -nlt | grep '8080'
    // Reference: https://stackoverflow.com/q/5106674
    int option = 1;
    int sockopt =
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (sockopt < 0) {
        perror("HTTP webserver (sockopt)");
        return 1;
    }

    // Create the address to bind the socket to
    struct sockaddr_in host_addr;
    int host_addrlen = sizeof(host_addr);

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen) < 0) {
        perror("HTTP webserver (bind)");
        return 1;
    }
    printf("socket bound to address successfully\n");

    // Listen for incoming connections
    if (listen(sockfd, SOMAXCONN) < 0) {
        perror("HTTP webserver (listen)");
        return 1;
    }
    printf("server listening for connections\n");

    for (;;) {
        // Accept incoming connections
        int client_sockfd = accept(sockfd,
                                   (struct sockaddr *)&host_addr,
                                   (socklen_t *)&host_addrlen);
        if (client_sockfd < 0) {
            perror("HTTP webserver (accept)");
        }
        printf("connection accepted\n");

        // Get client address
        struct sockaddr_in client_addr;
        int client_addrlen = sizeof(client_addr);
        int sockn = getsockname(client_sockfd,
                                (struct sockaddr *)&client_addr,
                                (socklen_t *)&client_addrlen);
        if (sockn < 0) {
            perror("HTTP webserver (getsockname)");
            continue;
        }

        // Read from the socket
        int valread = read(client_sockfd, req, BUFFER_SIZE);
        if (valread < 0) {
            perror("HTTP webserver (read)");
            continue;
        }

        // Read the request
        char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
        sscanf(req, "%s %s %s", method, uri, version);
        printf("[%s:%u] %s %s %s\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port),
               method,
               uri,
               version);

        // Write the response
        char *filename = "static/index.html";
        FILE *file = fopen(filename, "r");
        if (!file) {
            perror("HTTP webserver (fopen)");
            continue;
        }
        struct stat filestat;
        if (stat(filename, &filestat) < 0) {
            perror("HTTP webserver (stat)");
            continue;
        }
        size_t rbytes =
            fread(resp + resp_prefix_length, 1, filestat.st_size, file);
        if (rbytes < 0) {
            perror("HTTP webserver (fread)");
            continue;
        }
        resp[resp_prefix_length + rbytes] = '\0';

        // Write to the socket
        int valwrite = write(client_sockfd, resp, strlen(resp));
        if (valwrite < 0) {
            perror("HTTP webserver (write)");
            continue;
        }

        close(client_sockfd);
    }
}
