#include "connection.h"
#include "utils.h"
#include "json.hpp"

using json = nlohmann::json;

// open a connection to a server
int open_connection(const char *hostip, int port, int typeip, int typesocket, int flag) {
    struct sockaddr_in serv_addr;
    int sockfd = socket(typeip, typesocket, flag);
    DIE(sockfd < 0, "socket() failed");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = typeip;
    serv_addr.sin_port = htons(port);
    inet_aton(hostip, &serv_addr.sin_addr);

    /* connect the socket */
    int rc = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    DIE(rc < 0, "connect() failed");

    return sockfd;
}

// send a message to the server
void send_to_server(int sockfd, char *message) {
    int err, sentbytes = 0;
    int totalbytes = strlen(message);

    do {
        err = write(sockfd, message + sentbytes, totalbytes - sentbytes);
        DIE(err < 0, "write() failed");

        if (err == 0) {
            break;
        }

        sentbytes += err;
    } while (sentbytes < totalbytes);
}

// case-insensitive string search
int strncasestr(const char *haystack, const char *needle, size_t haystack_len, size_t needle_len) {
    if (needle_len > haystack_len) return -1;
    size_t last_pos = haystack_len - needle_len + 1;

    for (size_t i = 0; i < last_pos; ++i) {
        size_t j;
        for (j = 0; j < needle_len; ++j) {
            if (tolower(haystack[i + j]) != tolower(needle[j])) {
                break;
            }
        }
        if (j == needle_len) return i;
    }
    return -1;
}

// receive a message from the server
char *receive_from_server(int sockfd) {
    char response[BUFLEN];
    char *data = NULL;
    size_t total_size = 0;
    int header_end = -1;
    int content_length = 0;

    while (1) {
        int bytes = read(sockfd, response, BUFLEN);
        DIE(bytes < 0, "read() failed");
        if (bytes == 0) break;

        // +1 for null-terminator
        data = (char *)realloc(data, total_size + bytes + 1);
        DIE(!data, "realloc() failed");
        memcpy(data + total_size, response, bytes);
        total_size += bytes;
        data[total_size] = '\0';

        if (header_end == -1) {
            // check if the header is complete
            header_end = strncasestr(data, HEADER_END, total_size, HEADER_END_SIZE);
            if (header_end >= 0) {
                header_end += HEADER_END_SIZE;
                int content_length_start = strncasestr(data, CONTENT_LENGTH, total_size, CONTENT_LENGTH_SIZE);
                if (content_length_start >= 0) {
                    content_length_start += CONTENT_LENGTH_SIZE;
                    content_length = strtol(data + content_length_start, NULL, 10);
                }
            }
        }

        // check if the message is complete
        if (header_end >= 0 && (total_size - header_end) >= (size_t)content_length) {
            break;
        }
    }

    // ensure the data is null-terminated
    if (data) {
        data[total_size] = '\0';
    }

    return data;
}