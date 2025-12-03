#include <string.h>
#include <command.h>
#include <common.h>
#include <stdio.h>
#include <irc.h>

/**
 * @file response.c
 * @brief Provide responses for messages received from a server.
 * 
 * @author Aqiel Oostenbrug
 * @date December 3, 2025
 * @version 1.1
 * @bug None known
*/

/**
 * @brief Respond PONG with \p token to \p fd. 
 * @param fd file descriptor to respond to
 * @param token token to respond with
 * @return \p pong(fd, token)
 */
static int respond_pong(int fd, char * token) {
    printf("PONG %s\n", token);
    return pong(fd, token);
}

/**
 * @brief Check if \p response starts with \p cm
 * @param cm client message to match
 * @param response response to check
 * @return  0 if succesful,<br>
 *          1 otherwise
 */
static int client_message_check(char * cm, char *response) {
    if (cm == NULL || response == NULL) return 1; // Error
    int i = 0;
    while(cm[i] != '\0') { // Not at end
        if (cm[i] != response[i]) return 1; // No match
        i++;
    }
    return 0; // Match
}

// Global Command Tables
client_message cm_table[] = {
    {"PING", 4, respond_pong}
};


/**
 * @brief Execute command corresponding to \p response with \p response_len.
 * @param fd file descriptor for the command to write to
 * @param response response to parse
 * @param response_len length of the response to pars
 * @return  cm_table[i].func(fd, params) if successful<br>
 *          0 if unmatched,<br>
 *          -1 if \p fd is invalid,<br>
 *          -2 if response is invalid,<br>
 *          -3 if response is too short,<br>
 *          -4 if response only includes a client message parameter,<br>
 *          -5 if response only includes a space as a client message parameter
 */
int parse(int fd, char * response, int response_len) { // Consider potential dirty parsing issues
    // Check parameters
    if (fd < 0) return -1;
    if (response == NULL) return -2;
    if (response_len <= 0) return -3;

    // Handle "Client Messages"
    for (int i = 0; i < (sizeof(cm_table)/sizeof(client_message)); i++) { // Go through all CMs
        if (client_message_check(cm_table[i].name, response) == 0) { // Check ith CM
            // Check for params
            if ((response_len - cm_table[i].len) < 2) {
                printf("{ERROR} Client Message received without parameters[1]\n");
                return -4;
            }

            // Ignore spaces
            int params_start = cm_table[i].len + 2; // Start ignoring after first space
            while (response[params_start] == ' ') params_start++;
            
            // Check for params
            if (response[params_start] == '\0') {
                printf("{ERROR} Client Message received without parameters[2]\n");
                return -5;
            }

            // Make a clean copy of the parameters in the response
            int params_len = response_len - params_start;
            char params[params_len + 1]; // Make room for param
            memcpy(params, response + params_start, params_len);
            params[params_len] = '\0'; // Ensure end added

            // Return response
            printf("{MATCH} "); // Matched
            return (int) cm_table[i].func(fd, params);
        }
    }

    // Handle unmatched messages
    printf("{MISS} %.*s\n", response_len, response);
    return 0;
}
