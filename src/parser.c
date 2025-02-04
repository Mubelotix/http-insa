#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "parser.h"

// Function to extract the path from an HTTP request line
char *get_path(const char *request) {
    // Check if the request is NULL or empty
    if (request == NULL || *request == '\0') {
        return NULL;
    }

    // Find the first space which separates the method and path
    const char *start = strchr(request, ' ');
    if (start == NULL) {
        return NULL;
    }
    start++; // Move to the character after the space

    // Find the next space which separates the path and the HTTP version
    const char *end = strchr(start, ' ');
    if (end == NULL) {
        return NULL;
    }

    // Allocate memory for the path and copy it
    size_t path_length = end - start;
    char *path = malloc(path_length + 1); // +1 for the null terminator
    if (path == NULL) {
        return NULL;
    }

    // Copy the path from the request
    strncpy(path, start, path_length);
    path[path_length] = '\0'; // Null-terminate the string

    // Remove query parameters from the path (if any)
    char *query_pos = strchr(path, '?');
    if (query_pos) {
        *query_pos = '\0';
    }

    // Decode the path
    char *decoded = percent_decode(path);
    free(path);

    return decoded;
}

// Function to perform percent-decoding on a URL-encoded string
char *percent_decode(const char *src) {
    // Allocate memory for the decoded string
    size_t len = strlen(src);
    char *decoded = malloc(len + 1);
    if (!decoded) {
        perror("Failed to allocate memory");
        return NULL;
    }

    char *d = decoded; // Pointer to decoded string
    for (size_t i = 0; i < len; i++) {
        if (src[i] == '%') {
            // Check for valid percent-encoded sequence
            if (i + 2 < len && isxdigit(src[i + 1]) && isxdigit(src[i + 2])) {
                // Convert hex value to char
                int value;
                sscanf(src + i + 1, "%2x", &value);
                *d++ = (char)value; // Store decoded character
                i += 2; // Move past the percent-encoded sequence
            } else {
                // Invalid percent-encoded sequence, treat '%' as normal character
                *d++ = '%';
            }
        } else if (src[i] == '+') {
            // Decode '+' to space
            *d++ = ' ';
        } else {
            // Normal character, copy as is
            *d++ = src[i];
        }
    }
    *d = '\0'; // Null-terminate the decoded string

    return decoded;
}
