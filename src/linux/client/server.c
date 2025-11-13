#include <server.h>
#include <stdlib.h>

server_t *server_init(server_t *srv, int prt, char * adr) {
    if (adr == NULL || prt > 65535 || prt < 0) return NULL;
    int adr_len = sizeof(adr) - 1;
    if (adr_len > MAX_ADDR_LEN || adr_len < 0) return NULL;
    srv = (server_t *) malloc(sizeof(server_t));
    if ((srv->fd = socket(DOMAIN, TYPE, PROTOCOL)) == -1) return NULL;
    srv->adr.sin_family = DOMAIN;
    srv->adr.sin_port = htons(prt);
    srv->adr.sin_addr.s_addr = inet_addr(adr);
    srv->status = 1; //connection is closed
    return srv;
}

int server_destroy(server_t *srv) {
    if (srv == NULL) return 1;
    if (srv->status) server_disconnect(srv);
    free(srv);
    srv = NULL;
    return 0;
}

int server_connect(server_t *srv) {
    if((srv -> status = connect(srv->fd, (struct sockaddr *) &(srv->adr), sizeof(srv->adr))) == -1) return 1;
    return 0;
}
int server_authenticate(server_t *srv, user_t *usr) {
    // TODO: use funny func
    // TODO: update status after each func
    // // NEW?
    // out("PASS", usr->password);       
    // out("NICK", usr->nickname);
    // out("USER", usr->username, usr->realname);
    // OLD
    // snprintf(
    //         psw_msg,
    //         strlen(password) + 6,
    //         "PASS %s",
    //         password
    //     );
    // snprintf(
    //     nck_msg,
    //     strlen(nickname) + 6,
    //     "NICK %s",
    //     nickname
    // );
    // snprintf(
    //     usr_msg,
    //     strlen(username) + strlen(realname) + 12,
    //     "USER %s 0 * :%s",
    //     username,
    //     realname
    // );
    return 1;
}

int server_disconnect(server_t *srv) {
    if (srv == NULL) return 1;
    close(srv->fd);
    return 0;
}