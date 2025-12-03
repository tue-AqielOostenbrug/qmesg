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

#define TITLE "qmesg - linux client\n"
#define INTERRUPT_MSG "\nCaught interrupt\n"
#define NUM_FEEDS 2
#define TEST_PASSWORD "psw"
#define TEST_NICKNAME "nck"
#define TEST_USERNAME "usr"
#define TEST_REALNAME "rln"
#define TEST_PORT 6667 // default irc port
#define TEST_ADDR "127.0.0.1" // Loop back address

// Q: Separate viewer and controller functions from this model backend?

/**
 * @file main.c
 * @brief Main.
 * 
 * @author Aqiel Oostenbrug
 * @date December 3, 2025
 * @version 1.1
 * @bug Possible race conditions during authentication resulting in misses.
 * @bug Unexpected cuts in responses from the server
 * @bug Seems to get stuck in a livelock somewhere resulting in not being able to input commands
*/

/**
 * @brief Handle \p terminal_input from the user.
 * @param sesh server session to which requests need to be send
 * @param terminal_input \p terminal_input received from the user
 * @return  0 if successful,<br>
 *          1 if a reading error occurs,<br>
 *          2 if a writing error occurs
 */
static int handle_outgoing(session_t *sesh, char *terminal_input)
{
    if (fgets(terminal_input, MSG_MAX_LEN, stdin) == NULL) {
        printf("# An reading error has occurred while handling outgoing data\n");
        return 1;
    }
    if (write(session_fd(sesh), terminal_input, strlen(terminal_input)) == -1) {
        printf("# An reading error has occurred while handling outgoing data\n");
        return 2;
    }
    // TODO: Ensure \r\n
    memset(terminal_input, 0, MSG_MAX_LEN); // Clean input buffer for safety
    return 0;
}

/**
 * @brief Handle responses from the server to the client.
 * @param sesh \p server session from which the \p input originates
 * @param server_input \p server_input received from the server
 * @return  0 if successful,<br>
 *          1 if a read error occurs,<br>
 *          2 if a parsing error occurs
 */
static int handle_incoming(session_t *sesh, char *server_input)
{
    // Receive responses (multiple are gathered in one read)
    int fd = session_fd(sesh);
    if (read(fd, server_input, MSG_MAX_LEN) == -1) {
        printf("# An reading error has occurred while handling incoming data\n");
        return 1;
    }
    // Parse responses
    int i = 0;
    int j = strcspn(server_input, "\n"); // End of first response
    char c; // Place holder for the response delimiter check
    while (i < MSG_MAX_LEN && server_input[i] != '\0' && j > i) { // If end not reached
        if (parse(fd, server_input + i, j - i) != 0) { // j - i gives len
            printf("# An error has occurred while parsing:\n");
            return 2;
        }
        // printf("server> %.*s\n", j, server_input + i);
        // Update start and end for next response
        i += j; // Move to the first index after the last response
        while ((c = server_input[i]) == ' ' || c == '\r' || c == '\n') i++; // Skip delimiters
        j = strcspn(server_input + i, "\r\n"); // Move to end of the response
    }
    memset(server_input, 0, MSG_MAX_LEN); // Clean input buffer for safety
    return 0;
}

/**
 * @brief Request user credentials through terminal.
 * @param password \p password pointer to save the provided \p password to
 * @param nickname \p nickname pointer to save the provided \p nickname to
 * @param username \p username pointer to save the provided \p username to
 * @param realname \p realname pointer to save the provided \p realname to
 * @return 0 if successful<br>
 *          1 if password was not properly received<br>
 *          2 if nickname was not properly received<br>
 *          3 if username was not properly received<br>
 *          4 if realname was not properly received
 */
static int get_credentials(char *password, char *nickname, char * username, char *realname) {
    printf("password: ");
    if(fgets(password, PASSWORD_MAX_LEN + 1, stdin) == NULL) {
        printf("# Receiving password has failed\n"); // +1 for \n
        return 1;
    }
    password[strcspn(password, "\n")] = 0;
    printf("nickname: ");
    if(fgets(nickname, NICKNAME_MAX_LEN + 1, stdin) == NULL) {
        printf("# Receiving nickname has failed\n"); // +1 for \n
        return 2;
    }
    nickname[strcspn(nickname, "\n")] = 0;
    printf("username: ");
    if(fgets(username, USERNAME_MAX_LEN + 1, stdin) == NULL) {
        printf("# Receiving username has failed\n"); // +1 for \n
        return 3;
    }
    username[strcspn(username, "\n")] = 0;
    printf("realname: ");
    if (fgets(realname, REALNAME_MAX_LEN + 1, stdin) == NULL) {
        printf("# Receiving realname has failed\n"); // +1 for \n
        return 4;
    }
    realname[strcspn(realname, "\n")] = 0;
    return 0;
}

/**
 * @brief Request server credentials through terminal.
 * @param ip \p ip address pointer to save the provided \p ip to
 * @param port \p port pointer to save the provided \p port to
 * @return  0 if successful,<br>
 *          1 if receiving ip fails,<br>
 *          2 if receiving port fails
 */
static int get_server(char *ip, int *port) {
    printf("ip: ");
    if (fgets(ip, IP_MAX_LEN + 1, stdin)  == NULL) { // +1 for \n
        printf("# Receiving ip has failed\n"); // +1 for \n
        return 1;
    }
    ip[strcspn(ip, "\n")] = 0;
    printf("port: ");
    char s_port[PORT_MAX_LEN + 1] = "";
    if (fgets(s_port, PORT_MAX_LEN + 1, stdin) == NULL) {
        printf("# Receiving port has failed\n"); // +1 for \n
        return 2;
    }
    sscanf(s_port, " %d", port);
    return 0;
}

/**
 * @brief Authenticate \p user on \p sesh.
 * @param sesh \p sesh to authenticate \p user on
 * @param user \p user to authenticate
 * @return  0 if successful,<br>
 *          1 otherwise
 */
static int authenticate(session_t *sesh, user_t *usr) {
    // Q: Implement Capability negotiation
    int fd = session_fd(sesh);
    if ((pass(fd, usr->password) != 0) ||
        (nick(fd, usr->nickname) != 0) ||
        (user(fd, usr->username, usr->realname) != 0)) {
        printf("> Authenticating the user has failed\n");
        return 1;
    }
    return 0;
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
    if (get_server(adr, &port) != 0) return 1;

    printf("> Setting up polls\n");
    struct pollfd fds[NUM_FEEDS];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    printf("> Provide credentials\n");
    char password[PASSWORD_MAX_LEN + 1]; // +1 for \0
    char nickname[NICKNAME_MAX_LEN + 1];
    char username[USERNAME_MAX_LEN + 1];
    char realname[REALNAME_MAX_LEN + 1];
    if (get_credentials(password, nickname, username, realname) != 0) return 1;

    printf("> Generating user\n");
    user_t *user = malloc(sizeof(user_t));
    if (user_init(user, TEST_PASSWORD, TEST_NICKNAME, TEST_USERNAME, TEST_REALNAME) == NULL) return printf("SHT");

    printf("> Initializing session\n");
    session_t *sesh = (session_t *) malloc(sizeof(session_t));
    if (session_init(sesh, TEST_PORT, TEST_ADDR, strlen(TEST_ADDR)) == NULL) {
        printf("# Session Initialization failed\n");
        return 1;
    }
    if (session_connect(sesh) == 1) {
        printf("# Session connection failed\n");
        return 1;
    }
    fds[1].fd = session_fd(sesh);
    fds[1].events = POLLIN;

    printf("> Authenticating user\n");
    if (authenticate(sesh, user) != 0) return 1;

    // Loop variables
    char server_input[MSG_MAX_LEN];
    char terminal_input[MSG_MAX_LEN];

    printf("> Starting session\n");
    
    while(1) {
        // Check if new data in feeds
        if (poll(fds, NUM_FEEDS, 500) > 0) {
            if (fds[1].revents & POLLIN) {
                if (handle_incoming(sesh, server_input) != 0) return 1;
            }
            else if(fds[0].revents & POLLIN) {
                if (handle_outgoing(sesh, terminal_input) != 0) return 1;
            }
            else {
                printf("# An unexpected poll event has occured.\n");
                printf("- fds[1].revents: %d\n", fds[1].revents);
                printf("- fds[0].revents: %d\n", fds[0].revents);
                return 1;
            }
            printf("-------------------------------------------------------------------------------");
        }
    }
    printf("!!!Unreachable end has been reached!!!\n");
    while(1);
}
