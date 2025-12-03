#ifndef USER_H
#define USER_H
#define PASSWORD_MAX_LEN 30
#define NICKNAME_MAX_LEN 30
#define USERNAME_MAX_LEN 30
#define REALNAME_MAX_LEN 30

typedef struct _user_t {
    char *password;
    char *nickname;
    char *username;
    char *realname;
} user_t;

extern user_t *user_init(user_t *usr, char *psw, char *nck, char *usn, char *rln);
extern void user_destroy(user_t *usr);
// TODO: Make save and load function

// Setters for user values
extern int set_password(user_t *usr, char *psw);
extern int set_nickname(user_t *usr, char *nck);
extern int set_username(user_t *usr, char *usn);
extern int set_realname(user_t *usr, char *rln);

#endif

