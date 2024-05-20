#include "json.hpp"
#include "utils.h"
#include "connection.h"
#include "req.h"
#include "command.h"

int main() {
    char buffer[LINE];

    struct instance_data data;

    init_instance_data(&data);

    for (;;) {
        data.sockfd = open_connection(IP, PORT, AF_INET, SOCK_STREAM, 0);

        if (handle_command(&data, buffer)) {
            break;
        }
        close (data.sockfd);
    }
    close (data.sockfd);
    if (data.cookie != NULL)
        free(data.cookie);
    if (data.token != NULL)
        free(data.token);
    return 0;
}