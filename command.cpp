#include "command.h"
#include "utils.h"
#include "json.hpp"
#include "req.h"
#include "connection.h"

using json = nlohmann::json;

static void helper(char *username, char *password) {
    char buff[LINE];
    int err = 0;

    for (;;) {
        printf("username=");
        fgets(buff, LINE, stdin);
        buff[strlen(buff) - 1] = '\0';

        for (size_t i = 0; i < strlen(buff); i++) {
            if (buff[i] == ' ') {
                printf("No whitespaces.\n");
                err = 1;
                break;
            }
        }
        if (err) {
            err = 0;
            continue;
        }        

        strcpy(username, buff);
        username[strlen(username)] = '\0';
        break;
    }

    err = 0;

    for (;;) {
        printf("password=");
        fgets(buff, LINE, stdin);
        buff[strlen(buff) - 1] = '\0';

        for (size_t i = 0; i < strlen(buff); i++) {
            if (buff[i] == ' ') {
                printf("No whitespaces.\n");
                err = 1;
                break;
            }
        }

        if (err) {
            err = 0;
            continue;
        }

        strcpy(password, buff);
        password[strlen(password)] = '\0';
        break;
    }
}

void authentification(struct instance_data *data) {
    char username[LINE], password[LINE];

    helper(username, password);

    json auth = {
        {"username", username},
        {"password", password}
    };

    char *msg = (char *)malloc(auth.dump().length() + 1);
    DIE(msg == NULL, "malloc() failed");

    strcpy(msg, auth.dump().c_str());

    char *req = compute_post_request(SERVER, REGISTER, PAYLOAD, msg, NULL, NULL);

    send_to_server(data->sockfd, req);
    free(req);

    char *response = receive_from_server(data->sockfd);

     if (strstr(response, "{")) {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout<<"Error:";
        std::cout << r_auth.value("error", "") << "\n";
    } else {
        printf("Succesfully registered user %s.\n", username);
    }

    free(response);
    free(msg);
}

void login(int sockfd, char **cookies) {
    char username[LINE], password[LINE];

    helper(username, password);

    json auth = {
        {"username", username},
        {"password", password}
    };

    char *msg = (char *)malloc(auth.dump().length() + 1);
    DIE(msg == NULL, "malloc() failed");

    strcpy(msg, auth.dump().c_str());

    char *req = compute_post_request(SERVER, LOGIN, PAYLOAD, msg, NULL, NULL);

    send_to_server(sockfd, req);
    free(req);

    char *response = receive_from_server(sockfd);

    if (strstr(response, "{")) {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout << r_auth.value("error", "") << "\n";
    } else {
        char *tmp = strtok(strstr(response, "connect.sid"), ";");
        free(*cookies);
        *cookies = (char *)malloc(strlen(tmp) + 1);
        DIE(!(*cookies), "malloc() failed");
        memcpy(*cookies, tmp, strlen(tmp) + 1);
        printf("Successfully logged in user %s.\n", username);
    }

    free(response);
    free(msg);
}

void library(struct instance_data *data) {
    char *token;
    char *req = compute_get_request(SERVER, LIBRARY, data->cookie, NULL);

    send_to_server(data->sockfd, req);
    free(req);

    char *response = receive_from_server(data->sockfd);

    json r_enter = json::parse(strstr(response, "{"));

    if (r_enter.contains("error")) {
        std::cout << r_enter.value("error", "") << "\n";
    } else {
        std::string tmp;
        r_enter.at("token").get_to(tmp);

        token = (char *)malloc(tmp.length() + 1);
        DIE(!token, "malloc() failed");
        strcpy(token, tmp.c_str());

        printf("Successfully entered the library.\n");

        if (data->token != NULL) {
            free(data->token);
        }
        data->token = token;
    }
    free(response);
}

void get_books(struct instance_data *data) {
    char *req = compute_get_request(SERVER, BOOKS, data->cookie, data->token);

    send_to_server(data->sockfd, req);
    free(req);

    char *response = receive_from_server(data->sockfd);

    if (strstr(response, "[")) {
        std::cout << json::parse(strstr(response, "[")).dump(4) << "\n";
    } else {
        json r_get = json::parse(strstr(response, "{"));
        std::cout << r_get.value("error", "") << "\n";
    }

    free(response);
}

void get_book(struct instance_data *data) {
    char id[LINE] = { 0 }, buff[LINE];

    printf("id=");
    fgets(buff, LINE, stdin);
    memcpy(id, buff, strlen(buff) - 1);

    char *path = (char *)malloc(strlen(BOOK) + strlen(id) + 1);
    DIE(!path, "malloc() failed");
    strcpy(path, BOOK);
    strcat(path, id);

    char *request = compute_get_request(SERVER, path, NULL, data->token);
    send_to_server(data->sockfd, request);
    free(path);
    free(request);

    char *response = receive_from_server(data->sockfd);

    /**
     * This call will ALWAYS return a non-null value due to the
     * get_book API providing a json response for error and
     * non-error returns
     */
    json r_get = json::parse(strstr(response, "{"));
    if (r_get.contains("error")) {
        std::cout << r_get.value("error", "") << "\n";
    } else {
        std::cout << r_get.dump(4) << "\n";
    }

    free(response);
}

void add_book(struct instance_data *data) {
    char buff[LINE];
    char title[LINE] = { 0 }, author[LINE] = { 0 }, genre[LINE] = { 0 };
    char publisher[LINE] = { 0 }, page_count[LINE] = { 0 };

    printf("title=");
    fgets(buff, LINE, stdin);
    memcpy(title, buff, strlen(buff) - 1);

    printf("author=");
    fgets(buff, LINE, stdin);
    memcpy(author, buff, strlen(buff) - 1);

    printf("genre=");
    fgets(buff, LINE, stdin);
    memcpy(genre, buff, strlen(buff) - 1);

    printf("publisher=");
    fgets(buff, LINE, stdin);
    memcpy(publisher, buff, strlen(buff) - 1);

    printf("page_count=");
    fgets(buff, LINE, stdin);
    memcpy(page_count, buff, strlen(buff) - 1);

    // check if page_count is a number
    for (size_t i = 0; i < strlen(page_count); i++) {
        if (!isdigit(page_count[i])) {
            printf("Page count must be a number.\n");
            return;
        }
    }

    json add = {{"title", title}, {"author", author}, {"genre", genre},
        {"page_count", atoi(page_count)}, {"publisher", publisher}};

    char *add_string = (char *)malloc(add.dump().length() + 1);
    DIE(!add_string, "malloc() failed");
    strcpy(add_string, add.dump().c_str());

    char *request = compute_post_request(SERVER, BOOKS, PAYLOAD,
                                         add_string, NULL, data->token);
    send_to_server(data->sockfd, request);
    free(request);

    char *response = receive_from_server(data->sockfd);

    if (strstr(response, "{")) {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout << r_auth.value("error", "") << "\n";
    } else {
        printf("Successfully added book %s.\n", title);
    }

    free(add_string);
    free(response);
}

void delete_book(struct instance_data *data) {
    char id[LINE] = { 0 }, buff[LINE];

    printf("id=");
    fgets(buff, LINE, stdin);
    memcpy(id, buff, strlen(buff) - 1);

    char *path = (char *)malloc(strlen(BOOK) + strlen(id) + 1);
    DIE(!path, "malloc() failed");
    strcpy(path, BOOK);
    strcat(path, id);

    char *request = compute_delete_request(SERVER, path, NULL, data->token);
    send_to_server(data->sockfd, request);
    free(path);
    free(request);

    char *response = receive_from_server(data->sockfd);

    if (strstr(response, "{")) {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout << r_auth.value("error", "") << "\n";
    } else {
        printf("Successfully deleted book %s.\n", id);
    }

    free(response);
}

void logout(struct instance_data *data) {
    char *request = compute_get_request(SERVER, LOGOUT, data->cookie, NULL);
    send_to_server(data->sockfd, request);
    free(request);

    char *response = receive_from_server(data->sockfd);

    if (strstr(response, "{")) {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout << r_auth.value("error", "") << "\n";
    } else {
        printf("Successfully logged out user.\n");
    }

    free(data->cookie);
    free(data->token);
    data->cookie = data->token = NULL;
    free(response);
}