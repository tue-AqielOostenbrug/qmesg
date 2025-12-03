#ifndef COMMAND_H
#define COMMAND_H

typedef struct client_message_t {
    char * name;
    int len;
    int (*func)(int fd, char * args);
} client_message;

extern int parse(int fd, char *msg, int len);
#endif