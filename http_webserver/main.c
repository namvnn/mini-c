#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
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
    char buffer[BUFFER_SIZE];
    char resp[] =
        "HTTP/1.0 200 OK\r\n"
        "Server: http-webserver-c\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<html><head><title>HTTP Webserver in C</title></head><body>Hello, "
        "World!</body></html>\r\n";

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("HTTP webserver (socket)");
        return 1;
    }
    printf("socket created successfully\n");

    // Create the address to bind the socket to
    struct sockaddr_in host_addr;
    int host_addrlen = sizeof(host_addr);

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen) != 0) {
        perror("HTTP webserver (bind)");
        return 1;
    }
    printf("socket bound to address successfully\n");

    // Listen for incoming connections
    if (listen(sockfd, SOMAXCONN) != 0) {
        perror("HTTP webserver (listen)");
        return 1;
    }
    printf("server listening for connections\n");

    for (;;) {
        // Accept incoming connections
        int client_sockfd = accept(sockfd, (struct sockaddr *)&host_addr,
                                   (socklen_t *)&host_addrlen);
        if (client_sockfd < 0) {
            perror("HTTP webserver (accept)");
        }
        printf("connection accepted\n");

        // Get client address
        struct sockaddr_in client_addr;
        int client_addrlen = sizeof(client_addr);
        int sockn = getsockname(client_sockfd, (struct sockaddr *)&client_addr,
                                (socklen_t *)&client_addrlen);
        if (sockn < 0) {
            perror("HTTP webserver (getsockname)");
            continue;
        }

        // Read from the socket
        int valread = read(client_sockfd, buffer, BUFFER_SIZE);
        if (valread < 0) {
            perror("HTTP webserver (read)");
            continue;
        }

        // Read teh request
        char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
        sscanf(buffer, "%s %s %s", method, uri, version);
        printf("[%s:%u] %s %s %s\n", inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port), method, uri, version);

        // Write to the socket
        int valwrite = write(client_sockfd, resp, strlen(resp));
        if (valwrite < 0) {
            perror("HTTP webserver (write)");
            continue;
        }

        close(client_sockfd);
    }
}
