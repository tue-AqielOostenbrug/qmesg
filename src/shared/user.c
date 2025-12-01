#include <user.h>
#include <string.h>
#include <stdlib.h>

int strcheck(char * str, int max) {
    if (str == NULL) return 1;
    int len = strlen(str);
    return (len > max) ||
    (len <= 0);
}

user_t * user_init(user_t * usr, char * psw, char * nck, char * usn, char * rln) {
    if (
        strcheck(psw, PASSWORD_MAX_LEN) ||
        strcheck(nck, NICKNAME_MAX_LEN) ||
        strcheck(usn, USERNAME_MAX_LEN) ||
        strcheck(rln, REALNAME_MAX_LEN)
    ) {
        return NULL;
    }
    
    usr->password = strdup(psw);
    usr->nickname = strdup(nck);
    usr->username = strdup(usn);
    usr->realname = strdup(rln);

    return usr;
}

int *user_destroy(user_t *usr) {
    if (usr == NULL) return 0;
    free(usr);
    usr = NULL;
    return 0;
}

int set_password(user_t * usr, char * psw) {
    if ((usr == NULL) || strcheck(psw, PASSWORD_MAX_LEN)) {
        return 1;
    }
    
    usr->password = strdup(psw);

    return 0;
}

int set_nickname(user_t * usr, char * nck) {
    if ((usr == NULL) || strcheck(nck, PASSWORD_MAX_LEN)) {
        return 1;
    }
    
    usr->nickname = strdup(nck);

    return 0;
}

int set_username(user_t * usr, char * usn) {
    if ((usr == NULL) || strcheck(usn, PASSWORD_MAX_LEN)) {
        return 1;
    }
    
    usr->username = strdup(usn);

    return 0;
}

int set_realname(user_t * usr, char * rln) {
    if ((usr == NULL) || strcheck(rln, PASSWORD_MAX_LEN)) {
        return 1;
    }
    
    usr->username = strdup(rln);

    return 0;
}
