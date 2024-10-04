#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "test.h"

// Function to extract the path from an HTTP request line
const char *get_path(const char *request) {
    // Check if the request is NULL or empty
    if (request == NULL || *request == '\0') {
        return NULL; // Return NULL if the request is invalid
    }

    // Find the first space which separates the method and path
    const char *start = strchr(request, ' ');
    if (start == NULL) {
        return NULL; // Return NULL if no space found
    }
    start++; // Move to the character after the space

    // Find the next space which separates the path and the HTTP version
    const char *end = strchr(start, ' ');
    if (end == NULL) {
        return NULL; // Return NULL if no second space found
    }

    // Calculate the length of the path
    size_t path_length = end - start;

    // Allocate memory for the path and copy it
    char *path = malloc(path_length + 1); // +1 for the null terminator
    if (path == NULL) {
        return NULL; // Return NULL if memory allocation fails
    }

    strncpy(path, start, path_length);
    path[path_length] = '\0'; // Null-terminate the string

    return path; // Return the extracted path
}

// Function to send a file over a socket
int send_file(int socket, const char *path) {
    int file_fd = open(path, O_RDONLY);  // Open the file in read-only mode
    if (file_fd < 0) {
        perror("Failed to open file");
        return -1; // Return -1 if file opening fails
    }

    char buffer[4096];  // Buffer for reading the file
    ssize_t bytes_read, bytes_sent;

    // Loop to read from the file and send over the socket
    while ((bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0) {
        bytes_sent = send(socket, buffer, bytes_read, 0); // Send the data
        if (bytes_sent < 0) {
            perror("Failed to send file");
            close(file_fd); // Close file descriptor before returning
            return -1; // Return -1 if sending fails
        }
    }

    if (bytes_read < 0) {
        perror("Failed to read file");
        close(file_fd); // Close file descriptor before returning
        return -1; // Return -1 if reading fails
    }

    close(file_fd); // Close the file descriptor
    return 0; // Return 0 on success
}

