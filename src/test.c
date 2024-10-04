#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
