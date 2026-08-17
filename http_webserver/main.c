#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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

#define DEFAULT_PORT 8080
#define DEFAULT_ROOT "static"

int resolve_file_path(char *file_path, const char *req_uri, size_t root_len);
int get_file_ext(char *file_ext, const char *file_path);
int get_content_type(char *content_type, const char *file_ext);

int main(int argc, char *argv[]) {
    char root[256];
    size_t root_len;
    char req[2048];
    char req_method[256];
    char req_uri[256];
    char req_http_version[256];
    char res[131072] =
        "HTTP/1.0 200 OK\r\n"
        "Server: http-webserver-c\r\n";
    size_t res_prefix_len = strlen(res);
    char res_content_type[256];
    char file_path[256];
    char file_ext[256];

    // Parse arguments
    if (argc == 1) {
        strcpy(root, DEFAULT_ROOT);
    } else if (argc == 2) {
        strcpy(root, argv[1]);
    } else {
        fprintf(stderr, "HTTP webserver: too much arguments\n");
        return 1;
    }

    root_len = strlen(root);
    printf("root of static hosted files: %s\n", root);

    // Prefix root to file_path
    strcpy(file_path, root);

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("HTTP webserver: socket failed");
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
        perror("HTTP webserver: sockopt failed");
        return 1;
    }

    // Create the address to bind the socket to
    struct sockaddr_in host_addr;
    int host_addrlen = sizeof(host_addr);

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(DEFAULT_PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen) < 0) {
        perror("HTTP webserver: bind failed");
        return 1;
    }
    printf("socket bound to address successfully\n");

    // Listen for incoming connections
    if (listen(sockfd, SOMAXCONN) < 0) {
        perror("HTTP webserver: listen failed");
        return 1;
    }
    printf("server listening for connections: http://localhost:%d\n",
           DEFAULT_PORT);

    for (;;) {
        // Accept incoming connections
        int client_sockfd = accept(sockfd,
                                   (struct sockaddr *)&host_addr,
                                   (socklen_t *)&host_addrlen);
        if (client_sockfd < 0) {
            perror("HTTP webserver: accept failed");
        }
        printf("connection accepted\n");

        // Get client address
        struct sockaddr_in client_addr;
        int client_addrlen = sizeof(client_addr);
        int sockn = getsockname(client_sockfd,
                                (struct sockaddr *)&client_addr,
                                (socklen_t *)&client_addrlen);
        if (sockn < 0) {
            perror("HTTP webserver: getsockname failed");
            continue;
        }

        // Read from the socket
        int valread = read(client_sockfd, req, sizeof(req));
        if (valread < 0) {
            perror("HTTP webserver: read failed");
            continue;
        }

        // Read the request
        sscanf(req, "%s %s %s", req_method, req_uri, req_http_version);
        printf("  client %s:%u\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));
        printf("  %s %s %s\n", req_method, req_uri, req_http_version);

        // Write the response
        resolve_file_path(file_path, req_uri, root_len);

        FILE *file = fopen(file_path, "rb");
        if (!file) {
            perror("HTTP webserver: fopen failed");
            continue;
        }

        size_t res_headers_len;
        get_file_ext(file_ext, file_path);
        get_content_type(res_content_type, file_ext);
        strcpy(res + res_prefix_len, "Content-Type: ");
        strcpy(res + strlen(res), res_content_type);
        strcpy(res + strlen(res), "\r\n\r\n");
        res_headers_len = strlen(res);

        struct stat filestat;
        if (stat(file_path, &filestat) < 0) {
            perror("HTTP webserver: stat failed");
            continue;
        }

        size_t rbytes = fread(res + res_headers_len, 1, filestat.st_size, file);
        if (rbytes < 0) {
            perror("HTTP webserver: fread failed");
            continue;
        }

        fclose(file);

        // Write to the socket
        int valwrite = write(client_sockfd, res, res_headers_len + rbytes);
        if (valwrite < 0) {
            perror("HTTP webserver: write failed");
            continue;
        }

        printf("    serve file %s\n", file_path);

        close(client_sockfd);
    }
}

int resolve_file_path(char *file_path, const char *req_uri, size_t root_len) {
    bool is_file = false;

    for (size_t i = 0; req_uri[i] != '\0'; i++) {
        if (req_uri[i] == '.') {
            is_file = true;
            break;
        }
    }

    strcpy(file_path + root_len, req_uri);

    if (is_file) {
        return 0;
    }

    if (strcmp(req_uri, "/") != 0) {
        strcpy(file_path + strlen(file_path), "/");
    }
    strcpy(file_path + strlen(file_path), "index.html");

    return 0;
}

int get_file_ext(char *file_ext, const char *file_path) {
    size_t len = 0;
    bool start = false;

    for (size_t i = 0; file_path[i] != '\0'; i++) {
        if (file_path[i] == '.') {
            start = true;
            continue;
        }
        if (!start) {
            continue;
        }

        file_ext[len] = file_path[i];
        len += 1;
    }

    file_ext[len] = '\0';

    return 0;
}

int get_content_type(char *content_type, const char *file_ext) {
    if (strcmp(file_ext, "ico") == 0) {
        strcpy(content_type, "image/x-icon");
    } else if (strcmp(file_ext, "html") == 0) {
        strcpy(content_type, "text/html; charset=utf-8");
    } else if (strcmp(file_ext, "css") == 0) {
        strcpy(content_type, "text/css; charset=utf-8");
    } else if (strcmp(file_ext, "js") == 0) {
        strcpy(content_type, "application/javascript");
    } else {
        strcpy(content_type, "text/plain; charset=utf-8");
    }

    return 0;
}
