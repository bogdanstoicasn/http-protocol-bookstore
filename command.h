#ifndef COMMAND_H
#define COMMAND_H

void authentification(struct instance_data *data);

void login(int sockfd, char **cookies);

void library(struct instance_data *data);

void get_books(struct instance_data *data);

void get_book(struct instance_data *data);

void add_book(struct instance_data *data);

void delete_book(struct instance_data *data);

void logout(struct instance_data *data);
#endif