#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <dirent.h>
#include "filesystem.h"

// Function to get the content type based on the file extension
const char* get_content_type(const char *path) {
    const char *ext = strrchr(path, '.'); // Find the last dot in the path
    if (ext) {
        if (strcmp(ext, ".html") == 0) return "text/html";
        if (strcmp(ext, ".htm") == 0) return "text/html";
        if (strcmp(ext, ".txt") == 0) return "text/plain";
        if (strcmp(ext, ".css") == 0) return "text/css";
        if (strcmp(ext, ".js") == 0) return "application/javascript";
        if (strcmp(ext, ".json") == 0) return "application/json";
        if (strcmp(ext, ".png") == 0) return "image/png";
        if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
        if (strcmp(ext, ".gif") == 0) return "image/gif";
        if (strcmp(ext, ".pdf") == 0) return "application/pdf";
        if (strcmp(ext, ".svg") == 0) return "image/svg+xml";
        if (strcmp(ext, ".bmp") == 0) return "image/bmp";
        if (strcmp(ext, ".tiff") == 0 || strcmp(ext, ".tif") == 0) return "image/tiff";
        if (strcmp(ext, ".webp") == 0) return "image/webp";
        if (strcmp(ext, ".zip") == 0) return "application/zip";
        if (strcmp(ext, ".tar") == 0) return "application/x-tar";
        if (strcmp(ext, ".gz") == 0) return "application/gzip";
        if (strcmp(ext, ".rar") == 0) return "application/x-rar-compressed";
        if (strcmp(ext, ".mp3") == 0) return "audio/mpeg";
        if (strcmp(ext, ".wav") == 0) return "audio/wav";
        if (strcmp(ext, ".ogg") == 0) return "audio/ogg";
        if (strcmp(ext, ".mp4") == 0) return "video/mp4";
        if (strcmp(ext, ".avi") == 0) return "video/x-msvideo";
        if (strcmp(ext, ".mov") == 0) return "video/quicktime";
        if (strcmp(ext, ".mkv") == 0) return "video/x-matroska";
        if (strcmp(ext, ".xml") == 0) return "application/xml";
        if (strcmp(ext, ".csv") == 0) return "text/csv";
        if (strcmp(ext, ".sql") == 0) return "application/sql";
        if (strcmp(ext, ".ico") == 0) return "image/vnd.microsoft.icon";
        if (strcmp(ext, ".exe") == 0) return "application/vnd.microsoft.portable-executable";
        if (strcmp(ext, ".bin") == 0) return "application/octet-stream";
        if (strcmp(ext, ".apk") == 0) return "application/vnd.android.package-archive";
        if (strcmp(ext, ".dll") == 0) return "application/vnd.microsoft.portable-executable";
        if (strcmp(ext, ".jsonld") == 0) return "application/ld+json";
        if (strcmp(ext, ".mpg") == 0 || strcmp(ext, ".mpeg") == 0) return "video/mpeg";
        if (strcmp(ext, ".zipx") == 0) return "application/x-zip-compressed";
        if (strcmp(ext, ".7z") == 0) return "application/x-7z-compressed";
        if (strcmp(ext, ".css.map") == 0) return "application/json";
        if (strcmp(ext, ".map") == 0) return "application/json";
        if (strcmp(ext, ".woff") == 0) return "font/woff";
        if (strcmp(ext, ".woff2") == 0) return "font/woff2";
        if (strcmp(ext, ".ttf") == 0) return "font/ttf";
        if (strcmp(ext, ".eot") == 0) return "application/vnd.ms-fontobject";
        if (strcmp(ext, ".otf") == 0) return "font/otf";
        if (strcmp(ext, ".xml") == 0) return "application/xml";
        if (strcmp(ext, ".svgz") == 0) return "image/svg+xml";
        if (strcmp(ext, ".wasm") == 0) return "application/wasm";
        if (strcmp(ext, ".dmg") == 0) return "application/x-apple-diskimage";
        if (strcmp(ext, ".rtf") == 0) return "application/rtf";
        if (strcmp(ext, ".midi") == 0 || strcmp(ext, ".mid") == 0) return "audio/midi";
        if (strcmp(ext, ".3gp") == 0) return "video/3gpp";
        if (strcmp(ext, ".3g2") == 0) return "video/3gpp2";
        if (strcmp(ext, ".flac") == 0) return "audio/flac";
        if (strcmp(ext, ".csv") == 0) return "text/csv";
        if (strcmp(ext, ".latex") == 0 || strcmp(ext, ".tex") == 0) return "application/x-latex";
        if (strcmp(ext, ".sql") == 0) return "application/sql";
        if (strcmp(ext, ".woff") == 0) return "font/woff";
        if (strcmp(ext, ".woff2") == 0) return "font/woff2";
        if (strcmp(ext, ".ts") == 0) return "video/mp2t";
        if (strcmp(ext, ".msg") == 0) return "application/vnd.ms-outlook";
        if (strcmp(ext, ".pot") == 0 || strcmp(ext, ".potx") == 0) return "application/vnd.ms-powerpoint";
        if (strcmp(ext, ".doc") == 0 || strcmp(ext, ".docx") == 0) return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
        if (strcmp(ext, ".xls") == 0 || strcmp(ext, ".xlsx") == 0) return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
        if (strcmp(ext, ".ppt") == 0 || strcmp(ext, ".pptx") == 0) return "application/vnd.openxmlformats-officedocument.presentationml.presentation";
        if (strcmp(ext, ".epub") == 0) return "application/epub+zip";
        if (strcmp(ext, ".azw") == 0) return "application/vnd.amazon.ebook";
        if (strcmp(ext, ".mobi") == 0) return "application/x-mobipocket-ebook";
        if (strcmp(ext, ".vcard") == 0) return "text/vcard";
        if (strcmp(ext, ".ics") == 0) return "text/calendar";
        if (strcmp(ext, ".png") == 0) return "image/png";
        if (strcmp(ext, ".jfif") == 0) return "image/jpeg";
        if (strcmp(ext, ".bat") == 0) return "application/x-msdownload";
        if (strcmp(ext, ".sh") == 0) return "application/x-sh";
        if (strcmp(ext, ".plist") == 0) return "application/x-plist";
        if (strcmp(ext, ".apk") == 0) return "application/vnd.android.package-archive";
        if (strcmp(ext, ".m4a") == 0) return "audio/mp4a-latm";
        if (strcmp(ext, ".ogg") == 0) return "application/ogg";
        if (strcmp(ext, ".rar") == 0) return "application/x-rar-compressed";
        if (strcmp(ext, ".epub") == 0) return "application/epub+zip";
        if (strcmp(ext, ".ai") == 0) return "application/postscript";
        if (strcmp(ext, ".eps") == 0) return "application/postscript";
        if (strcmp(ext, ".svg") == 0) return "image/svg+xml";
    }
    perror("Extension inconnue: veuillez utiliser un fichier avec une extension plus courante\n");
    return "application/octet-stream"; // Default content type
}

void send_directory_listing(int socket, const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        const char *error_response = "HTTP/1.1 500 Internal Server Error\r\n"
                                     "Content-Type: text/plain\r\n"
                                     "Content-Length: 21\r\n"
                                     "\r\n"
                                     "500 Internal Error\n";
        send(socket, error_response, strlen(error_response), 0);
        return;
    }

    char response[4096];
    int response_length = snprintf(response, sizeof(response),
                                    "HTTP/1.1 200 OK\r\n"
                                    "Content-Type: text/html\r\n"
                                    "\r\n"
                                    "<html><body><h1>Directory Listing</h1><ul>");
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') { // Skip hidden files (optional)
            response_length += snprintf(response + response_length, sizeof(response) - response_length,
                                         "<li><a href=\"%s\">%s</a></li>", entry->d_name, entry->d_name);
        }
    }
    response_length += snprintf(response + response_length, sizeof(response) - response_length,
                                 "</ul></body></html>");

    closedir(dir);
    send(socket, response, response_length, 0); // Send directory listing
}

// Function to send a file over a socket
int send_file(int socket, const char *path) {
    // Send HTTP headers before the file content
    const char *content_type = get_content_type(path);
    struct stat file_stat;
    if (stat(path, &file_stat) < 0) {
        const char *error_response = "HTTP/1.1 404 Not Found\r\n"
                                     "Content-Type: text/plain\r\n"
                                     "Content-Length: 14\r\n"
                                     "\r\n"
                                     "404 Not Found\n";
        send(socket, error_response, strlen(error_response), 0);
        return -1; // Return -1 if file does not exist
    }

    // Check if it's a directory
    if (S_ISDIR(file_stat.st_mode)) {
        // Call the directory listing function
        send_directory_listing(socket, path);
        return 0; // Return 0 on success
    }

    int file_fd = open(path, O_RDONLY);  // Open the file in read-only mode
    if (file_fd < 0) {
        const char *error_response = "HTTP/1.1 404 Not Found\r\n"
                                     "Content-Type: text/plain\r\n"
                                     "Content-Length: 19\r\n"
                                     "\r\n"
                                     "404 Not Found\n";
        send(socket, error_response, strlen(error_response), 0);
        return -1; // Return -1 if file opening fails
    }

    // Prepare and send the HTTP response headers
    char length_header[512]; // Buffer for the response headers
    snprintf(length_header, sizeof(length_header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %lld\r\n"
             "\r\n",
             content_type, (long long)file_stat.st_size);
    send(socket, length_header, strlen(length_header), 0); // Send headers

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

