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
#define NUM_FEEDS 2

// Q: Separate viewer and controller functions from this model backend?

/**
 * @brief Handle \p terminal_input from the user.
 * @param sesh server session to which requests need to be send
 * @param terminal_input \p terminal_input received from the user
 */
void handle_outgoing(session_t *sesh, char *terminal_input)
{
    fgets(terminal_input, MSG_MAX_LEN, stdin);
    write(session_fd(sesh), terminal_input, strlen(terminal_input));
    memset(terminal_input, 0, MSG_MAX_LEN); // Clean input buffer for safety
}

/**
 * @brief Handle responses from the server to the client.
 * @param sesh \p server session from which the \p input originates
 * @param server_input \p server_input received from the server
 */
void handle_incoming(session_t *sesh, char *server_input)
{
    // Receive responses (multiple are gathered in one read)
    int fd = session_fd(sesh);
    read(
        fd,
        server_input,
        MSG_MAX_LEN
    );

    // Parse responses
    int i = 0;
    int j = strcspn(server_input, "\n"); // End of first response
    char c; // Place holder for the response delimiter check
    while (i < MSG_MAX_LEN && server_input[i] != '\0') { // If end not reached
        parse(fd, server_input + i, j - i); // j - i gives len
        // printf("server> %.*s\n", j, server_input + i);
        // Update start and end for next response
        i = j + 1; // Move to the first index after the last response
        while ((c = server_input[i]) == ' ' || c == '\r' || c == '\n') i++; // Skip delimiters
        j = strcspn(server_input + i, "\n"); // Move to end of the response
    }
    memset(server_input, 0, MSG_MAX_LEN); // Clean input buffer for safety
}

/**
 * @brief Request user credentials through terminal.
 * @param password \p password pointer to save the provided \p password to
 * @param nickname \p nickname pointer to save the provided \p nickname to
 * @param username \p username pointer to save the provided \p username to
 * @param realname \p realname pointer to save the provided \p realname to
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

/**
 * @brief Request server credentials through terminal.
 * @param ip \p ip address pointer to save the provided \p ip to
 * @param port \p port pointer to save the provided \p port to
 */
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
 * @brief Authenticate \p user on \p sesh.
 * @param sesh \p sesh to authenticate \p user on
 * @param user \p user to authenticate
 */
void authenticate(session_t *sesh, user_t *usr) {
    // Q: Implement Capability negotiation
    int fd = session_fd(sesh);
    pass(fd, usr->password);
    nick(fd, usr->nickname);
    user(fd, usr->username, usr->realname);
    // Q: Implement SASL auth
}

int main(int argc, char const *argv[])
{
    printf(TITLE);

    // TODO: Re-implement?
    // printf("Set up signal handler\n");
    // signal(SIGINT, handle_interrupt);

    printf("> Provide server credentials\n");
    char adr[IP_MAX_LEN + 1];
    int port;
    get_server(adr, &port);

    printf("> Setting up polls\n");
    struct pollfd fds[NUM_FEEDS];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    printf("> Provide credentials\n");
    char password[PASSWORD_MAX_LEN + 1]; // +1 for \0
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

    printf("> Authenticating user\n");
    authenticate(sesh, user);
    fds[1].fd = session_fd(sesh);
    fds[1].events = POLLIN;

    // Loop variables
    char server_input[MSG_MAX_LEN];
    char terminal_input[MSG_MAX_LEN];

    printf("> Starting session\n");
    
    while(1) {
        // Check if new data in feeds
        if (poll(fds, NUM_FEEDS, 500) > 0) {
            if (fds[1].revents & POLLIN) handle_incoming(sesh, server_input);
            else if(fds[0].revents & POLLIN) handle_outgoing(sesh, terminal_input);
            else printf("An unprecedented event has occured\n");
        }
    }
    printf("!!!Unreachable end has been reached!!!\n");
    while(1);
}
