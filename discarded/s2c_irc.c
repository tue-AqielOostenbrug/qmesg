#include <string.h>
#include <unistd.h>
#include <command.h>
#include <common.h>
#include <stdio.h>
#include <irc.h>

/**
 * @file responder.c
 * @brief Provide responses for messages received from the Server.
 * 
 * @author Aqiel Oostenbrug
 * @date November 26, 2025
 * @version 0.1
 * @bug None known
 * @see https://modern.ircdocs.horse/
*/

static int respond_pong(int fd, char * args) {
    if (fd < 0 || args == NULL) return 1;
    pong(fd, args);
    printf("%s\n", args);
    return 0;
}

int cmd_check(char * cmd, char *args) {
    if (cmd == NULL || args == NULL) return 1;
    int i = 0;
    while(cmd[i] != '\0') {
        if (cmd[i] != args[i]) return 1;
        i++;
    }
    return 0; // Return length of matched Client Message
}

server_command cmd_table[] = {
    {"PING", respond_pong}
};


int parse(int fd, char * response, int response_len) { // Consider potential dirty parsing issues
    // Check parameters
    if (fd < 0) return 1;
    if (response == NULL) return 2;
    if (response_len > 0) return 3;



    // int i = 0;
    // char c;
    // while (i < len && (c = response[i]) != '\0' || c != ' ') i++;
    // while 
    
    // char * param;
    // // Remove newlines
    // for (int i = 0; i < strlen(args); i++)
    // {
    //     if (response[i] == '\n' || response[i] == '\r') {
    //         response[i] = '\0';
    //         break;
    //     }
    // }

    // Handle server to client "Client Messages"
    for (int i = 0; i < (sizeof(cmd_table)/sizeof(server_command)); i++) { // Go through all CMs
        if (cmd_check(cmd_table[i].name, response) == 0) { // Check ith CM
            int params_start = strlen(cmd_table[i].name) + 1; // Start checking after first space
            while (response[params_start] == ' ') params_start++; // Ignore spaces
        
            // Make a clean copy of the response
            int params_len = response_len - params_start;
            char params[params_len + 1]; // Make room for param
            memcpy(params, response + params_start, params_len);
            params[params_len] = '\0'; // Ensure end added

            // message = message;
            // while (!(response[cmd_len - 1] == ' ' && response[cmd_len] == '\0')) {
            //     if (response[cmd_len - 1] == '\0') return 1;
            //     response++;
            // }
            printf("{Match}> "); // Matched
            return (int) cmd_table[i].func(fd, params);
        }
    }

    // // Handle rest
    // for (int i = 0; i < MSG_MAX_LEN; i++)
    // {
    //     if (args[i] != '\0') {
    //         write(fd, "Unknown response encountered -> ", 33);
    //         write(fd, args, i);
    //         write(fd, "\n", 1);
    //         return 0;
    //     }
    // }
    // write(fd, "Bad response encountered!\n", 27);
    printf("{Miss}> %s\n", response); // Unknown
    return 1;
}


// int parse(int fd, char * args, int len) { // Consider potential dirty parsing issues
//     // Separate

//     // Match with command actions

//     // Match with
//     int ret = 1;
//     for (int i = 0; i < (sizeof(cmd_table)/sizeof(server_command)); i++) {
//         if (cmd_check(cmd_table[i].name, args) == 0) {
//             ret = (int) cmd_table[i].func(fd, *(&args + strlen(cmd_table[i].name) + 1), len);
//             write(fd, "\r\n", 1);
//             return ret;
//         }
//     }
//     // Add error message matching for important client commands? and others? maybe mult parsers
//     // make sure to split on : (colon)
//     // potential func to func (matcher func, handler func)
//     return 1;
// }

// int is_client_message(char *args) {
//     return 0;
// }

// int is_numeric_reply(char *args) {
//     /*Distinct from a normal message,
//     a numeric reply MUST contain a <source> and
//     use a three-digit numeric as the command.
//     A numeric reply SHOULD contain the target of the reply as the first parameter of the message.
//     A numeric reply is not allowed to originate from a client.
//     */
//     return 0;
// }

// int handle_client_message(int fd, char *args) {
//     return 0;
// }

// // TODO: Handle client/numeric separately
// // TODO: Check strlen properly