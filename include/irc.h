#include <string.h>
#include <unistd.h>
#include <common.h> // Provides common magic literals

extern int pass(int fd,  char *password);
extern int nick(int fd,  char *nickname);
extern int user(int fd,  char *username, char *realname);
extern int ping(int fd, char *token);
extern int pong(int fd, char *token) ;
extern int join(int fd, char *channel);
extern int quit(int fd);
extern int privmsg(int fd, char *channel, char *msg);
extern int list(int fd);
extern int topic(int fd, char *channel);
extern int part(int fd, char *channel);