#ifndef TEST_H
#define TEST_H

const char *get_path(const char *request);
int send_file(int socket, const char *path);

#endif
