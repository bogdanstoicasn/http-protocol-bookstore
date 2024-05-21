#include "cmd.h"
#include "utils.h"
#include "json.hpp"
#include "req.h"
#include "connection.h"

using json = nlohmann::json;

// Function that reads the username and password from stdin
static int helper(char *username, char *password) {
    char buff[LINE];
    int err = 0;

    printf("username=");
    fgets(buff, LINE, stdin);
    buff[strlen(buff) - 1] = '\0';

    for (size_t i = 0; i < strlen(buff); i++) {
        if (buff[i] == ' ' || buff[i] == '\t') {
            err = 1;
            break;
        }
    }      

    strcpy(username, buff);
    username[strlen(username)] = '\0';

    memset(buff, 0, LINE);

    printf("password=");
    fgets(buff, LINE, stdin);
    buff[strlen(buff) - 1] = '\0';

    for (size_t i = 0; i < strlen(buff); i++) {
        if (buff[i] == ' ' || buff[i] == '\t') {
            err = 1;
            break;
        }
    }

    strcpy(password, buff);
    password[strlen(password)] = '\0';

    if (err) {
        std::cout << "ERROR:Username and password must not contain spaces or tabs.\n";
        return 1;
    }

    return 0;
}

// Function that sends a GET request to the server and receives the response
static char *get_req_helper(int sockfd, const char *url, char *p1, char *p2) {
    char *req = string_get_request(SERVER, url, p1, p2);
    send_to_server(sockfd, req);
    free(req);

    return receive_from_server(sockfd);
}

// Function that sends a POST request to the server and receives the response
static char *post_req_helper(int sockfd, json auth, const char *url, char *pp) {
    char *msg = (char *)malloc(auth.dump().length() + 1);
    DIE(msg == NULL, "malloc() failed");

    strcpy(msg, auth.dump().c_str());

    char *req = string_post_request(SERVER, url, PAYLOAD, msg, NULL, pp);
    send_to_server(sockfd, req);

    free(req);
    free(msg);

    return receive_from_server(sockfd);
}

void authentification(struct instance_data *data) {
    char username[LINE], password[LINE];

    if (helper(username, password))
        return;

    json auth = {
        {"username", username},
        {"password", password}
    };

    char *response = NULL;
    response = post_req_helper(data->sockfd, auth, REGISTER, NULL);

    if (!response)
        return;

    if (!strstr(response, "{"))
        std::cout << "SUCCESS:Successfully registered user " << username << ".\n";
    else {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout<<"ERROR:" << r_auth.value("error", "") << "\n";
    }

    free(response);
}

void login(int sockfd, char **cookies) {
    char username[LINE], password[LINE];

    if (helper(username, password))
        return;

    json auth = {
        {"username", username},
        {"password", password}
    };

    char *response;
    response = post_req_helper(sockfd, auth, LOGIN, NULL);

    if (!response)
        return;

    if (!strstr(response, "{")) {
        char *tmp = strtok(strstr(response, "connect.sid"), ";");
        free(*cookies);
        *cookies = (char *)malloc(strlen(tmp) + 1);
        DIE(!(*cookies), "malloc() failed");
        memcpy(*cookies, tmp, strlen(tmp) + 1);
        std::cout << "SUCCESS:Successfully logged in user " << username << ".\n";
    } else {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout<<"ERROR:" << r_auth.value("error", "") << "\n";
    }

    free(response);
}

void library(struct instance_data *data) {
    char *response = NULL;
    response = get_req_helper(data->sockfd, LIBRARY, data->cookie, NULL);

    if (!response)
        return;

    json r_enter = json::parse(strstr(response, "{"));

    if (!r_enter.contains("error")) {
        std::string tmp;
        r_enter.at("token").get_to(tmp);

        if (data->token)
            free(data->token);

        data->token = (char *)malloc(tmp.length() + 1);
        DIE(!data->token, "malloc() failed");
        strcpy(data->token, tmp.c_str());

        std::cout << "SUCCESS:Successfully entered library.\n";
    } else
        std::cout<<"ERROR:" << r_enter.value("error", "") << "\n";
    free(response);
}

void get_books(struct instance_data *data) {
    char *response = NULL;
    response = get_req_helper(data->sockfd, BOOKS, data->cookie, data->token);

    if (!response)
        return;

    if (strstr(response, "["))
        std::cout << json::parse(strstr(response, "[")).dump(4) << "\n";
    else {
        json r_get = json::parse(strstr(response, "{"));
        std::cout<<"ERROR:" << r_get.value("error", "") << "\n";
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

    char *request = string_get_request(SERVER, path, NULL, data->token);
    send_to_server(data->sockfd, request);

    free(path);
    free(request);

    char *response = receive_from_server(data->sockfd);

    json r_get = json::parse(strstr(response, "{"));

    if (!r_get.contains("error"))
        std::cout << r_get.dump(4) << "\n";
    else
        std::cout<<"ERROR:" << r_get.value("error", "") << "\n";

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
            std::cout << "ERROR:Page count must be a number.\n";
            return;
        }
    }

    json add = {{"title", title},
                {"author", author},
                {"publisher", publisher},
                {"genre", genre},
                {"page_count", atoi(page_count)},
                };

    char *response = NULL;
    response = post_req_helper(data->sockfd, add, BOOKS, data->token);

    if (!response)
        return;

    if (!strstr(response, "{"))
        std:: cout << "SUCCESS:Successfully added book " << title << ".\n";
    else {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout<<"ERROR:" << r_auth.value("error", "") << "\n";
    }

    free(response);
}

void delete_book(struct instance_data *data) {
    char buff[LINE];
    char id[LINE] = { 0 };

    printf("id=");
    fgets(buff, LINE, stdin);
    memcpy(id, buff, strlen(buff) - 1);

    char *path = (char *)malloc(strlen(BOOK) + strlen(id) + 1);
    DIE(!path, "malloc() failed");
    strcpy(path, BOOK);
    strcat(path, id);

    char *request = string_delete_request(SERVER, path, NULL, data->token);
    send_to_server(data->sockfd, request);

    free(path);
    free(request);

    char *response = receive_from_server(data->sockfd);

    if (!strstr(response, "{")) {
        std::cout << "SUCCESS:Successfully deleted book " << id << ".\n";
    } else {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout<<"ERROR:" << r_auth.value("error", "") << "\n";
    }

    free(response);
}

void logout(struct instance_data *data) {
    char *response = NULL;
    response = get_req_helper(data->sockfd, LOGOUT, data->cookie, NULL);

    if (!response)
        return;

    if (!strstr(response, "{")) {
        std::cout << "SUCCESS:Successfully logged out.\n";
    } else {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout<<"ERROR:" << r_auth.value("error", "") << "\n";
    }

    free(response);
    free(data->cookie);
    free(data->token);

    data->cookie = data->token = NULL;
}