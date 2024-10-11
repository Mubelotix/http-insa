#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "filesystem.h"
#include "settings.h"
#include "parser.h"

#define BACKLOG 10      // Number of pending connections queue will hold
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RESET "\033[0m"
#define COLOR_GRAY "\033[0;30m"

typedef struct {
    int socket_desc;
    ServerSettings settings;
} ConnectionData;

void *handle_connection(void* data);

int main() {
    ServerSettings settings = init_server_settings();

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket with options to reuse the address and port
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    // Define the server address and port to bind to
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(settings.ip_address);
    address.sin_port = htons(settings.port);

    // Bind the socket to the specified IP and port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        printf("Try running: kill -9 $(lsof -t -i:%d)\n", settings.port);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, BACKLOG) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf(COLOR_GREEN "Serving %s on http://%s:%d\n" COLOR_RESET, settings.root_folder, settings.ip_address, settings.port);

    // Accept an incoming connection
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            continue;  // Continue on error to accept the next connection
        }

        printf("Connection received from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        // Create the parameters for the connection handler
        pthread_t thread_id;
        ConnectionData *conn_data = malloc(sizeof(ConnectionData));
        if (conn_data == NULL) {
            perror("Failed to allocate memory for connection data");
            close(new_socket);
            continue;
        }
        conn_data->socket_desc = new_socket;
        conn_data->settings = settings;

        // Spawn a new thread to handle the connection
        if (pthread_create(&thread_id, NULL, handle_connection, (void*)conn_data) != 0) {
            perror("Failed to create thread");
            free(conn_data);
            close(new_socket);
        }

        // Detach the thread to allow independent execution and automatic cleanup
        pthread_detach(thread_id);
    }

    // Close the server socket (unreachable in this case because of the infinite loop)
    close(server_fd);

    return 0;
}

void *handle_connection(void* data) {
    ConnectionData *conn_data = (ConnectionData *)data;
    int sock = conn_data->socket_desc;
    ServerSettings settings = conn_data->settings;

    // Buffer for reading the request. Doesn't have to be big
    char buffer[4096];
    int read_size = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (read_size <= 0) {
        printf("Failed to receive request.\n");
        close(sock);
        free(data);
        return NULL;
    }
    buffer[read_size] = '\0';

    // Extract the path from the request
    char *path = get_path(buffer);
    if (path == NULL) {
        const char *error_response = "HTTP/1.1 404 Not Found\r\n"
                                     "Content-Type: text/plain\r\n"
                                     "Content-Length: 14\r\n"
                                     "\r\n"
                                     "404 Not Found\n";
        send(sock, error_response, strlen(error_response), 0);
        close(sock);
        free(data);
        return NULL;
    }
    printf(COLOR_GRAY "Extracted path: '%s'\n" COLOR_RESET, path);
    
    // Construct the full path to the requested file (considering the root folder)
    char full_path[4096];
    snprintf(full_path, sizeof(full_path), "%s%s", settings.root_folder, path);
    free(path);
    printf(COLOR_GRAY "Full path: '%s'\n" COLOR_RESET, full_path);

    // Check if the requested path is within the root folder
    char *root_path = realpath(settings.root_folder, NULL);
    char *requested_path = realpath(full_path, NULL);
    if (root_path == NULL || requested_path == NULL || strncmp(requested_path, root_path, strlen(root_path)) != 0) {
        const char *error_response = "HTTP/1.1 403 Forbidden\r\n"
                                     "Content-Type: text/plain\r\n"
                                     "Content-Length: 14\r\n"
                                     "\r\n"
                                     "403 Forbidden\n";
        send(sock, error_response, strlen(error_response), 0);
        close(sock);
        free(data);
        free(root_path);
        free(requested_path);
        return NULL;
    }
    printf(COLOR_GRAY "Requested path: '%s'\n" COLOR_RESET, requested_path);

    // Send the file
    if (send_file(sock, full_path) < 0) {
        printf("Failed to send file: %s\n", full_path);
    }

    close(sock);
    free(data);
    free(root_path);
    free(requested_path);
    return NULL;
}
