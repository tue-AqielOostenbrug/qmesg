#include <string.h>
#include <unistd.h>
#include <common.h> // Provides common magic literals
#include <irc.h>


/**
 * @file irc.c
 * @brief Provide IRC commands for Client to Server commnication.
 * 
 * Note that since write is used no fflush is required!
 * 
 * @author Aqiel Oostenbrug
 * @date November 26, 2025
 * @version 1.1
 * @bug None known
 * @see https://modern.ircdocs.horse/
*/

// Q: Specify arg len?
// Q: Check write returns?

/**
 * @brief write \p password to the server.
 * @param[in] fd file descriptor to write to
 * @param[in] password password to write
 * @return  0 if successful,<br>
 *          1 if \p fd is invalid,<br>
 *          2 if \p password is invalid,<br>
 *          3 if message is too long
 */
int pass(int fd,  char *password) {
    if (fd < 0) return 1;
    if (password == NULL) return 2; // Q: Is there a better way to do all these checks?
    int len; // Q: Is the position necessary? Does the compiler move it up?
    if ((len = strlen(password)) + 1 + 5 + 2 > MSG_MAX_LEN) return 3;
    write(fd, "PASS ", 5); // Q: Should this become a magic literal?
    write(fd, password, len);
    write(fd, "\r\n", 2);
    return 0;
}

/**
 * @brief write \p nickname to the server.
 * @param[in] fd file descriptor to write to
 * @param[in] nickname nickname to write
 * @return  0 if successful,<br>
 *          1 if \p fd is invalid,<br>
 *          2 if \p nickname is invalid,<br>
 *          3 if message is too long
 */
int nick(int fd,  char *nickname) {
    if (fd < 0) return 1;
    if (nickname == NULL) return 2;
    int len;
    if ((len = strlen(nickname)) + 1 + 5 + 2 > MSG_MAX_LEN) return 3;
    write(fd, "NICK ", 5);
    write(fd, nickname, len);
    write(fd, "\r\n", 2);
    return 0;
}

/**
 * @brief write \p username and \p realname to the server.
 * @param fd file descriptor to write to
 * @param[in] username username to write
 * @param[in] realname realname to write
 * @return  0 if successful,<br>
 *          1 if \p fd is invalid,<br>
 *          2 if \p username is invalid,<br>
 *          3 if \p realname is invalid,<br>
 *          4 if message is too long
 */
int user(int fd,  char *username, char *realname) {
    if (fd < 0) return 1;
    if (username == NULL) return 2;
    if (realname == NULL ) return 3;
    int u_len, r_len;
    if ((u_len = strlen(username)) + (r_len = strlen(realname)) + 1 + 5 + 6 + 2 > MSG_MAX_LEN) return 4;
    write(fd, "USER ", 5);
    write(fd, username, u_len);
    write(fd, " 0 * :", 6);
    write(fd, realname, r_len);
    write(fd, "\r\n", 2);
    return 0;
}

/**
 * @brief Ping the server.
 * @param[in] fd file descriptor to write to
 * @param[in] token token to write
 * @return  0 if successful,<br>
 *          1 if \p fd is invalid,<br>
 *          3 if \p token is invalid,<br>
 *          4 if message is too long
 */
int ping(int fd, char *token) {
    if (fd < 0) return 1;
    if (token == NULL) return 2;
    int len;
    if ((len = strlen(token)) + 1 + 5 + 2 > MSG_MAX_LEN) return 3;
    write(fd, "PING ", 5);
    write(fd, token, len);
    write(fd, "\r\n", 2);
    return 0;
}

/**
 * @brief Pong the server.
 * @param[in] fd file descriptor to write to
 * @param[in] token token to write
 * @return  0 if successful,<br>
 *          1 if \p fd is invalid,<br>
 *          3 if \p token is invalid,<br>
 *          4 if message is too long
 */
int pong(int fd, char *token) {
    if (fd < 0) return 1;
    if (token == NULL) return 2;
    int len;
    if ((len = strlen(token)) + 1 + 5 + 2 > MSG_MAX_LEN) return 3;
    write(fd, "PONG ", 5);
    write(fd, token, len);
    write(fd, "\r\n", 2);
    return 0;
}

/**
 * @brief Join \p server.
 * @param[in] fd file descriptor to write to
 * @param[in] channel channel to join
 * @return  0 if successful,<br>
 *          1 if \p fd is invalid,<br>
 *          3 if \p channel is invalid,<br>
 *          4 if message is too long
 */
int join(int fd, char *channel) {
    if (fd < 0) return 1;
    if (channel == NULL) return 2;
    int len;
    if ((len = strlen(channel)) + 1 + 6 + 2 > MSG_MAX_LEN) return 3;
    write(fd, "JOIN #", 6);
    write(fd, channel, len);
    write(fd, "\r\n", 2);
    return 0;
}

/**
 * @brief Terminate the connection to the server.
 * @param[in] fd file descriptor to write to
 * @param[in] token token to write
 * @return  0 if successful,<br>
 *          1 if \p fd is invalid
 */
int quit(int fd) {
    if (fd < 0) return 1;
    write(fd, "QUIT\r\n", 6);
    return 0;
}

/**
 * @brief write \p msg \p channel to the server.
 * @param fd file descriptor to write to
 * @param[in] channel channel to write
 * @param[in] msg message to write
 * @return  0 if request send successfully,<br>
 *          1 if \p fd is invalid,<br>
 *          2 if \p channel is invalid,<br>
 *          3 if \p msg is invalid,<br>
 *          4 if total message is too long
 */
int privmsg(int fd, char *channel, char *msg) {
    if (fd < 0) return 1;
    if (channel == NULL) return 2;
    if (msg == NULL) return 3;
    int t_len, m_len;
    if ((t_len = strlen(channel)) + (m_len = strlen(msg)) + 1 + 9 + 1 + 2 > MSG_MAX_LEN) return 4;
    write(fd, "PRIVMSG #", 9);
    write(fd, channel, t_len);
    write(fd, " ", 1);
    write(fd, msg, m_len);
    write(fd, "\r\n", 2);
    return 0;
}

/**
 * @brief Request the list of channels.
 * @param[in] fd file descriptor to write to
 * @return  0 if request send successfully,<br>
 *          1 if \p fd is invalid
 */
int list(int fd) {
    if (fd < 0) return 1;
    write(fd, "LIST\r\n", 6);
    return 0;
}

/**
 * @brief Request the topic of \p channel.
 * @param[in] fd file descriptor to write to
 * @param[in] channel channel from which the topic to request
 * @return  0 if successful,<br>
 *          1 if \p fd is invalid,<br>
 *          2 if \p channel is invalid,<br>
 *          3 if total message is too long
 */
int topic(int fd, char *channel) {
    if (fd < 0) return 1;
    if (channel == NULL) return 2;
    int len;
    if ((len = strlen(channel)) + 1 + 7 + 2 > MSG_MAX_LEN) return 3;
    write(fd, "TOPIC #", 7);
    write(fd, channel, len);
    write(fd, "\r\n", 2);
    return 0;
}

/**
 * @brief Request to leave \p channel.
 * @param[in] fd file descriptor to write to
 * @param[in] channel channel to leave
 * @return  0 if successful,<br>
 *          1 if \p fd is invalid,<br>
 *          2 if \p channel is invalid,<br>
 *          3 if total message is too long
 */
int part(int fd, char *channel) {
    if (fd < 0) return 1;
    if (channel == NULL) return 2;
    int len;
    if ((len = strlen(channel)) + 1 + 6 + 2 > MSG_MAX_LEN) return 3;
    write(fd, "PART #", 6);
    write(fd, channel, len);
    write(fd, "\r\n", 2);
    return 0;
}