#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>

#define BUFLEN              4096
#define LINE                128

#define REGISTER            "/api/v1/tema/auth/register"
#define LOGIN               "/api/v1/tema/auth/login"
#define LOGOUT              "/api/v1/tema/auth/logout"
#define LIBRARY             "/api/v1/tema/library/access"
#define BOOKS               "/api/v1/tema/library/books"
#define BOOK                "/api/v1/tema/library/books/"

#define PAYLOAD             "application/json"
#define SERVER              "34.246.184.49:8080"
#define IP                  "34.246.184.49"
#define PORT                8080

#define HEADER_END          "\r\n\r\n"
#define HEADER_END_SIZE     4
#define CONTENT_LENGTH      "Content-Length: "
#define CONTENT_LENGTH_SIZE 16

#define DIE(assertion, call_description)                                       \
    do {                                                                       \
        if (assertion) {                                                       \
            fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);                 \
            perror(call_description);                                          \
            exit(errno);                                                       \
        }                                                                      \
    } while (0)

/*
    Structure that holds the instance data.
*/
struct instance_data {
    char *cookie;
    char *token;
    int sockfd;
};

/*
    Parses a string and returns the number of arguments.
*/
int whitespace_parse(char *buff, char **argv);

/*
    Initializes the instance data.
*/
void init_instance_data(struct instance_data *data);

/*
    Reads a command from stdin and executes it.
*/
int handle_command(struct instance_data *data, char *buffer);

#endif