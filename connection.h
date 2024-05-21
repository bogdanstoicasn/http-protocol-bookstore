#ifndef CONNECTION_H
#define CONNECTION_H

/*
    Opens a connection to the server.
    Returns the socket file descriptor.
*/
int open_connection(const char *hostip, int port, int typeip, int typesocket, int flag);

/*
    Sends a message to the server.
*/
void send_to_server(int sockfd, char *message);

/*
    Receives a message from the server.
    Returns the message.
*/
char *receive_from_server(int sockfd);

#endif