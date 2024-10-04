#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct {
    char ip_address[16]; // For IPv4 addresses (xxx.xxx.xxx.xxx)
    int port;            // Port number
    char root_folder[256]; // Path to the root folder
} ServerSettings;

ServerSettings init_server_settings();
ServerSettings default_server_settings();
int read_server_settings(ServerSettings *settings);

#endif
