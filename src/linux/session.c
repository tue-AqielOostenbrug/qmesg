#include <client_session.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/tcp.h>
#include <time.h>

#define CONNECTION_DELAY 1

/**
 * @file session.c
 * @brief Manage TCP client-side sessions.
 * 
 * @author Aqiel Oostenbrug
 * @date December 7, 2025
 * @version 1.0.1
 * @bug None known
*/

/**
 * @brief Initialize \p sesh with \p prt, \p adr, and \p adr_len.
 * @param sesh session to initialize
 * @param prt port to connect to
 * @param adr ip address to connect to
 * @param adr_len length of \p adr
 * @return  sesh if successful,<br>
 *          NULL otherwise
 */
session_t *session_init(session_t *sesh, int prt, char * adr, int adr_len) {
    if (adr == NULL || prt > 65535 || prt < 0) return NULL;
    if (adr_len > MAX_ADDR_LEN || adr_len < 0) return NULL;
    if ((sesh->fd = socket(DOMAIN, TYPE, PROTOCOL)) == -1) return NULL;
    int yes = 1;
    setsockopt(sesh->fd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));
    sesh->adr.sin_family = DOMAIN;
    sesh->adr.sin_port = htons(prt);
    sesh->adr.sin_addr.s_addr = inet_addr(adr);
    sesh->status = 1; //connection is closed
    return sesh;
}

/**
 * @brief Destroy \p sesh.
 * @param sesh session to destroy
 * @return  0 if successful<br>
 *          1 if \p sesh was not initialized
 */
int session_destroy(session_t *sesh) {
    if (sesh == NULL) return 1;
    if (sesh->status) session_disconnect(sesh);
    free(sesh);
    sesh = NULL;
    return 0;
}

/**
 * @brief Connect to the server specified by \p sesh.
 * @param sesh session to start
 * @return  0 if successful,<br>
 *          1 otherwise
 */
int session_connect(session_t *sesh) {
    if(((sesh->status) = connect(sesh->fd, (struct sockaddr *) &(sesh->adr), sizeof(sesh->adr))) == -1) return 1;
    sleep(CONNECTION_DELAY);
    return 0;
}

/**
 * @brief Disconnect from the server specified by \p sesh.
 * @param sesh session to end
 * @return  0 if successful,<br>
 *          1 otherwise
 */
int session_disconnect(session_t *sesh) {
    if (sesh == NULL) return 1;
    close(sesh->fd);
    sesh->status = 1;
    return 0;
}

/**
 * @brief Return the file descriptor granted by \p sesh.
 * @param sesh to source the file descriptor from
 * @return  \p sesh->fd file descriptor of \p sesh<br>
 *          -1 otherwise
 */
int session_fd(session_t *sesh) {
    if (sesh == NULL || sesh->status == 1) return -1;
    return sesh->fd;
}

