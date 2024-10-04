#include "settings.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Function to initialize the server settings with default values
ServerSettings default_server_settings() {
    ServerSettings settings;

    // Set default IP address, port, and root folder
    strncpy(settings.ip_address, "0.0.0.0", sizeof(settings.ip_address)); // Listen on all interfaces
    settings.port = 8080; // Default port
    strncpy(settings.root_folder, "./www", sizeof(settings.root_folder)); // Default root folder

    return settings; // Return the initialized settings
}

int read_server_settings(ServerSettings *settings) {
    FILE *file = fopen("./etc/settings.conf", "r");
    if (!file) {
        perror("Failed to open ./etc/settings.conf, try /etc/settings.conf");
        file = fopen("/etc/settings.conf", "r");
        if (!file) {
            perror("Failed to open /etc/settings.conf");
            return -1;
        }
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char key[256], value[256];
        if (sscanf(line, "%255[^=]=%255[^\n]", key, value) == 2) {
            if (strcmp(key, "ip_address") == 0) {
                strncpy(settings->ip_address, value, sizeof(settings->ip_address) - 1);
                settings->ip_address[sizeof(settings->ip_address) - 1] = '\0';
            } else if (strcmp(key, "port") == 0) {
                settings->port = atoi(value);
            } else if (strcmp(key, "root_folder") == 0) {
                strncpy(settings->root_folder, value, sizeof(settings->root_folder) - 1);
                settings->root_folder[sizeof(settings->root_folder) - 1] = '\0';
            } else {
                fprintf(stderr, "Unknown key: %s\n", key);
            }
        }
    }

    fclose(file);
    return 0;
}

ServerSettings init_server_settings() {
    ServerSettings settings = default_server_settings();
    if (read_server_settings(&settings) == 0) {
        printf("Settings loaded from ./etc/settings.conf\n");
    }
    return settings;
}
