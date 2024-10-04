#include "settings.h"
#include <string.h>
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
