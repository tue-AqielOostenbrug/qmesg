#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H
// Q: Should these be included here?
// Q: Can I remove some of these?
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_ADDR_LEN 100
#define DOMAIN AF_INET // IPv4
#define TYPE SOCK_STREAM // TCP
#define PROTOCOL 0 // use provided protocol configuration
#define PORT 6667 // default irc port
#define ADDR "127.0.0.1" // Loop back address

typedef struct _session_t {
    struct sockaddr_in adr; // Q: Should this be a pointer?
    int fd;
    int status; // 0: open connection, 1: closed connection
} session_t;

session_t *session_init(session_t *srv, int prt, char * adr);
extern int session_destroy(session_t *srv);
extern int session_connect(session_t *srv);
extern int session_disconnect(session_t *srv);
extern int session_fd(session_t *sesh);
// TODO: Make save and load function
#endif

