#ifndef CMD_H
#define CMD_H

/*
    Gets username and password from stdin.
    Sends a POST request to the server with the username and password.
*/
void authentification(struct instance_data *data);

/*
    Gets username and password from stdin.
    Sends a POST request to the server with the username and password.
*/
void login(int sockfd, char **cookies);

/*
    Sends a GET request to the server with the cookies.
    Receives a token from the server.
*/
void library(struct instance_data *data);

/*
    Sends a GET request to the server with the cookies and token.
    Receives a list of books from the server.
*/
void get_books(struct instance_data *data);

/*
    Sends a GET request to the server with the cookies and token.
    Receives a book from the server.
*/
void get_book(struct instance_data *data);

/*
    Gets a book from stdin.
    Sends a POST request to the server with the book.
*/
void add_book(struct instance_data *data);

/*
    Gets a book from stdin.
    Sends a DELETE request to the server with the book.
*/
void delete_book(struct instance_data *data);

/*
    Sends logout request to the server.
    Receives a message from the server.
*/
void logout(struct instance_data *data);
#endif