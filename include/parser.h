#ifndef PARSER_H
#define PARSER_H

// Function to extract the path from an HTTP request line
const char *get_path(const char *request);
char *percent_decode(const char *src);

#endif
