#ifndef CONNECTION_H
#define CONNECTION_H

int open_connection(const char *host_ip, int port, int ip_type, int socket_type, int flag);

void send_to_server(int sockfd, char *message);

char *receive_from_server(int sockfd);

#endif