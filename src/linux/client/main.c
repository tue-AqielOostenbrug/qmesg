#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <signal.h> 
#include <stdbool.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <user.h>
#include <irc.h>
#include <client_session.h>
#include <command.h>
#include <common.h>

#define TITLE "qmesg linux client\n"
#define INTERRUPT_MSG "\nCaught interrupt\n"
#define PORT 6667 // default
#define ADDR "127.0.0.1"
#define NUM_FEEDS 2

// Q: Separate viewer and controller functions from this model backend?

/**
 * Print incoming input.
*/
void print_incoming(char *input) {
    int i = 0;
    int j = strcspn(input, "\n");
    while (i < strlen(input)) {
        printf("server> %.*s\n", j, input + i);
        i += j + 1;
        j = strcspn(input + i, "\n");
    }
}

/**
 * Parse incoming input.
*/
void parse_incoming(int fd, char *input) {
    int i = 0;
    int j = strcspn(input, "\n");
    while (i < strlen(input)) {
        parse(fd, input + i);
        i += j + 1;
        j = strcspn(input + i, "\n");
    }
}

/**
 * @brief Request user credentials through terminal.
 * @param password 
 * @param nickname 
 * @param username 
 * @param realname 
 */
void get_credentials(char *password, char *nickname, char * username, char *realname) {
    printf("password: ");
    fgets(password, PASSWORD_MAX_LEN + 1, stdin); // +1 for \n
    password[strcspn(password, "\n")] = 0;
    printf("nickname: ");
    fgets(nickname, NICKNAME_MAX_LEN + 1, stdin);
    nickname[strcspn(nickname, "\n")] = 0;
    printf("username: ");
    fgets(username, USERNAME_MAX_LEN + 1, stdin);
    username[strcspn(username, "\n")] = 0;
    printf("realname: ");
    fgets(realname, REALNAME_MAX_LEN + 1, stdin);
    realname[strcspn(realname, "\n")] = 0;
}

void get_server(char *ip, int *port) {
    printf("ip: ");
    fgets(ip, IP_MAX_LEN + 1, stdin); // +1 for \n
    ip[strcspn(ip, "\n")] = 0;
    printf("port: ");
    char s_port[PORT_MAX_LEN + 1] = "";
    fgets(s_port, PORT_MAX_LEN + 1, stdin);
    sscanf(s_port, " %d", port);
}

/**
 * @brief Authenticate \p usr on \p srv
 * @param srv server
 * @param usr user
 */
void authenticate(session_t *srv, user_t *usr) {
    pass(srv->fd, usr->password);
    nick(srv->fd, usr->nickname);
    user(srv->fd, usr->username, usr->realname);
}

int main(int argc, char const *argv[])
{
    printf(TITLE);

    // TODO: Re-implement?
    // printf("Set up signal handler\n");
    // signal(SIGINT, handle_interrupt);

    printf("> Setting up terminal\n");
    struct pollfd fds[NUM_FEEDS];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    printf("> Provide server\n");
    char adr[IP_MAX_LEN + 1];
    int port;
    get_server(adr, &port);

    printf("> Provide credentials\n");
    char password[PASSWORD_MAX_LEN + 1]; // +1 for \n
    char nickname[NICKNAME_MAX_LEN + 1];
    char username[USERNAME_MAX_LEN + 1];
    char realname[REALNAME_MAX_LEN + 1];
    get_credentials(password, nickname, username, realname);

    printf("> Generating user\n");
    user_t *user = malloc(sizeof(user_t));
    user_init(user, password, nickname, username, realname);

    printf("> Initializing session\n");
    session_t *sesh = (session_t *) malloc(sizeof(session_t));
    if (session_init(sesh, port, adr) == NULL) {
        printf("Session Initialization failed\n");
        return 1;
    }
    if (session_connect(sesh) == 1) {
        printf("Session connection failed\n");
        return 2;
    }
    fds[1].fd = sesh->fd;
    fds[1].events = POLLIN;

    printf("> Authenticating user\n");
    authenticate(sesh, user);

    // Loop variables
    char input[MSG_MAX_LEN];
    char output[MSG_MAX_LEN];

    printf("Starting session\n");
    
    while(1) {
        // Check if new data in feeds
        if (poll(fds, NUM_FEEDS, 500) > 0) {
            // Handle incoming data
            if(fds[1].revents & POLLIN) {
                read(
                    session_fd(sesh),
                    input,
                    MSG_MAX_LEN
                );
                print_incoming(input);
                parse_incoming(sesh->fd, input);
                memset(input, 0, MSG_MAX_LEN);
            }
            // Handle outgoing data
            else if(fds[0].revents & POLLIN) {
                // handle_outgoing(sesh);
                // char msg_out[MSG_MAX_LEN];
                fgets(output, MSG_MAX_LEN, stdin);
                write(sesh->fd, output, strlen(output));
            }
            else { 
                printf("An unprecedented event has occured\n");
            }
        }
    }
    printf("!!!Unreachable end has been reached!!!\n");
    while(1);
}
