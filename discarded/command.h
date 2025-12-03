#ifndef COMMAND_H
#define COMMAND_H

typedef struct client_command_t {
    char * name;
    int (*func)(int fd, char * args[], int len);
} client_command;

typedef struct server_command_t {
    char * name;
    int (*func)(int fd, char * args);
} client_message;

typedef struct action_t {
    int (*mask)(char * args);
    int (*func)(int fd, char * args);
} action;

extern int parse(int fd, char *msg, int len);
// int execute(char * cmd, char *args);
#endif