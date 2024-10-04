#ifndef FILESYSTEM_H
#define FILESYSTEM_H

const char* get_content_type(const char *path);
int send_file(int socket, const char *path);
void send_directory_listing(int socket, const char *path);

#endif
