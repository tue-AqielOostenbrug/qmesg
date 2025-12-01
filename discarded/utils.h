#ifndef UTILS_H
#define UTILS_H

// Strings
extern int serialize(char* cmd, char* params);
extern int deserialize(char* cmd, char* params, char* data);

// Server
extern int send_msg(int sock, char* cmd, char* params);
extern int receive_data(int sock, char* data);

// User
extern int receive_input(char* input);
extern int output_msg(char* sender, char* cmd, char* params);

// Too trivial
// // Commands
// extern int reverse_ping(char* cmd, char* params);

#endif
