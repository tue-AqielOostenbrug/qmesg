#ifndef SERVER_H // TODO: rename, client2server? c2s? session?
#define SERVER_H
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include "user.h"

#define MAX_ADDR_LEN 100
#define DOMAIN AF_INET // IPv4
#define TYPE SOCK_STREAM // TCP
#define PROTOCOL 0 // default
#define PORT 6667 // default

typedef struct _server_t {
    struct sockaddr_in adr; // pointer?
    int fd;
    int status;
} server_t;

server_t *server_init(server_t *srv, int prt, char * adr);
extern int server_destroy(server_t *srv);
extern int server_connect(server_t *srv);
extern int server_authenticate(server_t *srv, user_t *usr);
extern int server_disconnect(server_t *srv);
#endif

