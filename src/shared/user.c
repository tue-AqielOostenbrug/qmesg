#include <user.h>
#include <string.h>
#include <stdlib.h>

/**
 * @file user.c
 * @brief Manage user data.
 * 
 * @author Aqiel Oostenbrug
 * @date December 3, 2025
 * @version 1.0
 * @bug None known
*/

/**
 * Check the validity of \p str.
*/
static int strcheck(char * str, int max) {
    if (str == NULL) return 1;
    int len = strlen(str);
    return (len > max) ||
    (len <= 0);
}

/**
 * @brief Initialize \p usr with \p psw, \p prt, \p adr, and \p adr_len.
 * @param usr \p user to initialize
 * @param psw \p password of \p user
 * @param nck \p nickname of \p user
 * @param usn \p username of \p user
 * @param rln \p realname of \p user
 * @return  \p usr if successful,
 *          \p NULL otherwise
 */
user_t * user_init(user_t * usr, char * psw, char * nck, char * usn, char * rln) { // Q: is user_t as return value redundant here?
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

/**
 * @brief Destroy \p usr.
 * @param usr user to destroy
 */
void user_destroy(user_t *usr) {
    if (usr == NULL) return;
    free(usr);
    usr = NULL;
}

/**
 * @brief Set \p psw to \p usr.
 * @param usr user to update
 * @param psw password to set
 * @return  0 if successful,
 *          1 otherwise
 */
int set_password(user_t * usr, char * psw) {
    if ((usr == NULL) || strcheck(psw, PASSWORD_MAX_LEN)) return 1;
    usr->password = strdup(psw);
    return 0;
}

/**
 * @brief Set \p nck to \p usr.
 * @param usr user to update
 * @param nck nickname to set
 * @return  0 if successful,
 *          1 otherwise
 */
int set_nickname(user_t * usr, char * nck) {
    if ((usr == NULL) || strcheck(nck, PASSWORD_MAX_LEN)) return 1;
    usr->nickname = strdup(nck);
    return 0;
}

/**
 * @brief Set \p usn to \p usr.
 * @param usr user to update
 * @param usn username to set
 * @return  0 if successful,
 *          1 otherwise
 */
int set_username(user_t * usr, char * usn) {
    if ((usr == NULL) || strcheck(usn, PASSWORD_MAX_LEN)) return 1;
    usr->username = strdup(usn);
    return 0;
}

/**
 * @brief Set \p rln to \p usr.
 * @param usr user to update
 * @param rln realname to set
 * @return  0 if successful,
 *          1 otherwise
 */
int set_realname(user_t * usr, char * rln) {
    if ((usr == NULL) || strcheck(rln, PASSWORD_MAX_LEN)) return 1;
    usr->username = strdup(rln);
    return 0;
}
