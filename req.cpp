#include "utils.h"
#include "req.h"

char *string_get_request(const char *host, const char *url, char *cookies, char *token) {
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    DIE(!message, "calloc() failed");

    snprintf(message, BUFLEN, "GET %s HTTP/1.1\r\nHost: %s\r\n", url, host);

    if (cookies) {
        strncat(message, "Cookie: ", BUFLEN - strlen(message) - 1);
        strncat(message, cookies, BUFLEN - strlen(message) - 1);
        strncat(message, "\r\n", BUFLEN - strlen(message) - 1);
    }

    if (token) {
        strncat(message, "Authorization: Bearer ", BUFLEN - strlen(message) - 1);
        strncat(message, token, BUFLEN - strlen(message) - 1);
        strncat(message, "\r\n", BUFLEN - strlen(message) - 1);
    }

    strncat(message, "\r\n", BUFLEN - strlen(message) - 1);

    return message;
}

char *string_post_request(const char *host, const char *url,
                           const char *content_type, const char *body_data,
                           char *cookies, char *token) {
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    DIE(!message, "calloc() failed");

    snprintf(message, BUFLEN, "POST %s HTTP/1.1\r\n", url);
    strncat(message, "Host: ", BUFLEN - strlen(message) - 1);
    strncat(message, host, BUFLEN - strlen(message) - 1);
    strncat(message, "\r\n", BUFLEN - strlen(message) - 1);

    snprintf(message + strlen(message), BUFLEN - strlen(message), 
             "Content-Type: %s\r\n", content_type);

    snprintf(message + strlen(message), BUFLEN - strlen(message), 
             "Content-Length: %lu\r\n", strlen(body_data));

    if (cookies) {
        strncat(message, "Cookie: ", BUFLEN - strlen(message) - 1);
        strncat(message, cookies, BUFLEN - strlen(message) - 1);
        strncat(message, "\r\n", BUFLEN - strlen(message) - 1);
    }

    if (token) {
        strncat(message, "Authorization: Bearer ", BUFLEN - strlen(message) - 1);
        strncat(message, token, BUFLEN - strlen(message) - 1);
        strncat(message, "\r\n", BUFLEN - strlen(message) - 1);
    }

    strncat(message, "\r\n", BUFLEN - strlen(message) - 1);
    strncat(message, body_data, BUFLEN - strlen(message) - 1);

    return message;
}

char *string_delete_request(const char *host, const char *url,
						  	 char *cookies, char *token) {
	char *message = (char *)calloc(BUFLEN, sizeof(char));
    DIE(!message, "calloc() failed");

    snprintf(message, BUFLEN, "DELETE %s HTTP/1.1\r\n", url);
    strncat(message, "Host: ", BUFLEN - strlen(message) - 1);
    strncat(message, host, BUFLEN - strlen(message) - 1);
    strncat(message, "\r\n", BUFLEN - strlen(message) - 1);

    if (cookies) {
        strncat(message, "Cookie: ", BUFLEN - strlen(message) - 1);
        strncat(message, cookies, BUFLEN - strlen(message) - 1);
        strncat(message, "\r\n", BUFLEN - strlen(message) - 1);
    }

    if (token) {
        strncat(message, "Authorization: Bearer ", BUFLEN - strlen(message) - 1);
        strncat(message, token, BUFLEN - strlen(message) - 1);
        strncat(message, "\r\n", BUFLEN - strlen(message) - 1);
    }

    strncat(message, "\r\n", BUFLEN - strlen(message) - 1);

    return message;	
}