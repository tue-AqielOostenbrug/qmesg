#include <string.h>
#include <unistd.h>
#include <command.h>

#define MSG_MAX_LEN 512

int pass(int fd,  char * args[], int len) {
    if (fd < 0 || args == NULL || len < 1) return 1;
    write(fd, "PASS ", 5);
    write(fd, args[0], strlen(args[0]));
    return 0;
}

int nick(int fd,  char * args[], int len) {
    if (fd < 0 || args == NULL || len < 1) return 1;
    write(fd, "NICK ", 5);
    write(fd, args[0], strlen(args[0]));
    return 0;
}

int user(int fd,  char * args[], int len) {
    if (fd < 0 || args == NULL || len < 2) return 1;
    write(fd, "USER ", 5);
    write(fd, args[0], strlen(args[0]));
    write(fd, " 0 * :", 6);
    write(fd, args[1], strlen(args[1]));
    return 0;
}

int ping(int fd, char * args[], int len) {
    if (fd < 0 || args == NULL || len < 1) return 1;
    write(fd, "PING ", 5);
    write(fd, args[0], strlen(args[0]));
    return 0;
}

int pong(int fd, char * args[], int len) {
    if (fd < 0 || args == NULL ||len < 1) return 1;
    write(fd, "PONG ", 5);
    write(fd, args[0], strlen(args[0]));
    return 0;
}

// TODO: Make a perfect hashmap -> O(1) lookup[BERND]
client_command cmd_table[] = { // TODO: Find good hash function?
    {"PASS", pass},
    {"NICK", nick},
    {"USER", user},
    {"PING", ping},
    {"PONG", pong}
};


int exec(int fd, char * cmd, char * args[], int len) {
    int ret = 1;
    for (int i = 0; i < (sizeof(cmd_table)/sizeof(client_command)); i++) {
        if (strcmp(cmd_table[i].name, cmd) == 0) {
            ret = (int) cmd_table[i].func(fd, args, len);
            write(fd, "\r\n", 1);
            // TODO: add to log?
            // TODO: update controller?
            // fflush(fdopen(fd));?
            return ret;
        }
    }
    return 1;
}

// DIFF ABSTRACTION LEVEL FROM S2C?
// ALSO IS THIS NECESSARY ON LINUX? SEEMS MOST USEFULL FOR REPLIES IF WE ASSUME
// USER HAS NO IRC EXPERIENCE

// Remove hash - cmd but keep individual funcs