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

    // 1. Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    // 2. Define the address to bind to
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(settings.ip_address);
    address.sin_port = htons(settings.port);

    // 3. Bind the socket to the specified IP and port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        printf("Try running: kill -9 $(lsof -t -i:%d)\n", settings.port);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 4. Listen for incoming connections
    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Serving %s on http://%s:%d\n", settings.root_folder, settings.ip_address, settings.port);

    // 5. Accept an incoming connection
    while (1) {
        // Accept a new connection
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            continue;  // Continue on error to accept the next connection
        }

        // Print incoming connection details
        printf("Connection received from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        // 6. Spawn a new thread to handle the connection
        pthread_t thread_id;
        ConnectionData *conn_data = malloc(sizeof(ConnectionData));
        if (conn_data == NULL) {
            perror("Failed to allocate memory for connection data");
            close(new_socket); // Close socket if unable to allocate memory
            continue;
        }        conn_data->socket_desc = new_socket;
        conn_data->settings = settings; // Copy the settings to the thread

        if (pthread_create(&thread_id, NULL, handle_connection, (void*)conn_data) != 0) {
            perror("Failed to create thread");
            free(conn_data);  // Free memory if thread creation fails
            close(new_socket);  // Close socket if unable to create a thread
        }

        // Detach the thread to allow independent execution and automatic cleanup
        pthread_detach(thread_id);
    }

    // Close the server socket (unreachable in this case because of the infinite loop)
    close(server_fd);

    return 0;
}

// 7. Function to handle a connection
void *handle_connection(void* data) {
    ConnectionData *conn_data = (ConnectionData *)data;
    int sock = conn_data->socket_desc;
    ServerSettings settings = conn_data->settings;

    // Send a message to the client²
    // char *message = "Hello! You are connected to the server.\n";
    // send(sock, message, strlen(message), 0);

    // Print a message indicating that the connection is being handled
    printf("Handling connection on socket %d\n", sock);

    // Buffer for reading the request
    char buffer[4096];
    int read_size = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (read_size <= 0) {
        printf("Failed to receive request.\n");
        close(sock);
        free(data);
        return NULL;
    }

    buffer[read_size] = '\0';  // Null-terminate the received data
    char *path = get_path(buffer); // Use the get_path function

    if (path == NULL) {
        const char *error_response = "HTTP/1.1 404 Not Found\r\n"
                                     "Content-Type: text/plain\r\n"
                                     "Content-Length: 14\r\n"
                                     "\r\n"
                                     "404 Not Found\n";
        send(socket, error_response, strlen(error_response), 0);
        close(sock);
        free(data);
        return NULL;
    }
    printf("Extracted path: '%s'\n", path);
    
    char full_path[4096];
    snprintf(full_path, sizeof(full_path), "%s%s", settings.root_folder, path);
    free(path);
    printf("Full path: '%s'\n", full_path);

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
    printf("Requested path: '%s'\n", requested_path);

    if (send_file(sock, full_path) < 0) {
        printf("Failed to send file: %s\n", full_path);
    }

    // Close the client socket after handling
    close(sock);
    free(data);
    free(root_path);
    free(requested_path);

    return NULL;
}
