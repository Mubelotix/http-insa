#ifndef TEST_H
#define TEST_H

const char *get_path(const char *request);
const char* get_content_type(const char *path);
int send_file(int socket, const char *path);

#endif
