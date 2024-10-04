#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct {
    char ip_address[16];
    int port;
    char root_folder[256];
} ServerSettings;

ServerSettings init_server_settings();
ServerSettings default_server_settings();
int read_server_settings(ServerSettings *settings);

#endif
