#ifndef CONNECTION_H
#define CONNECTION_H

#include "server.h"
#include "user.h"

typedef struct connection_t {
    server_t server;
    user_t user;
} connection;

#endif

