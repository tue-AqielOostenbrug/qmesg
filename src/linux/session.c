#include <client_session.h>
#include <stdlib.h>
#include <stdio.h>

session_t *session_init(session_t *sesh, int prt, char * adr) {
    if (adr == NULL || prt > 65535 || prt < 0) return NULL;
    int adr_len = sizeof(adr) - 1;
    if (adr_len > MAX_ADDR_LEN || adr_len < 0) return NULL;
    if ((sesh->fd = socket(DOMAIN, TYPE, PROTOCOL)) == -1) return NULL;
    sesh->adr.sin_family = DOMAIN;
    sesh->adr.sin_port = htons(prt);
    sesh->adr.sin_addr.s_addr = inet_addr(adr);
    sesh->status = 1; //connection is closed
    return sesh;
}

int session_destroy(session_t *sesh) {
    if (sesh == NULL) return 1;
    if (sesh->status) {
        session_disconnect(sesh);
        return 2;
    }
    free(sesh);
    sesh = NULL;
    return 0;
}

int session_connect(session_t *sesh) {
    if(((sesh->status) = connect(sesh->fd, (struct sockaddr *) &(sesh->adr), sizeof(sesh->adr))) == -1) return 1;
    return 0;
}

int session_disconnect(session_t *sesh) {
    if (sesh == NULL) return 1;
    close(sesh->fd);
    sesh->status = 1;
    return 0;
}

int session_fd(session_t *sesh) {
    if (sesh == NULL || sesh->status == 1) return -1;
    return sesh->fd;
}

