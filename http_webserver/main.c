#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>

#define PORT 8080

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

        close(client_sockfd);
    }
}
