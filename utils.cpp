#include "utils.h"
#include "cmd.h"

// Function that parses a string and returns the number of arguments
int whitespace_parse(char *buff, char **argv)
{
	int argc = 0;
	char *p = strtok(buff, " \n\t");
	while (p) {
		argv[argc++] = p;
		p = strtok(NULL, " \n\t");
	}
	return argc;
}

// Function that initializes the instance data
void init_instance_data(struct instance_data *data) {
	data->cookie = NULL;
	data->token = NULL;
	data->sockfd = -1;
}

// Function that reads a command from stdin and executes it
int handle_command(struct instance_data *data, char *buffer) {
	// read command from stdin
	memset(buffer, 0, LINE);
    fgets(buffer, LINE - 1, stdin);

	// parse command, check if it is valid and execute it
	int argc = 0;
	char *argv[LINE];
	argc = whitespace_parse(buffer, argv);

	if (argc > 1) {
		std::cout << "Invalid command\n";
		return 0;
	}
	if (!strcmp(argv[0], "exit")) {
		return 1;
	}
	if (!strcmp(argv[0], "register")) {
		authentification(data);
		return 0;
	}
	if (!strcmp(argv[0], "login")) {
		login(data->sockfd, &data->cookie);
		return 0;
	}
	if (!strcmp(argv[0], "enter_library")) {
		library(data);
		return 0;
	}
	if (!strcmp(argv[0], "get_books")) {
		get_books(data);
		return 0;
	}
	if (!strcmp(argv[0], "get_book")) {
		get_book(data);
		return 0;
	}
	if (!strcmp(argv[0], "add_book")) {
		add_book(data);
		return 0;
	}
	if (!strcmp(argv[0], "delete_book")) {
		delete_book(data);
		return 0;
	}
	if (!strcmp(argv[0], "logout")) {
		logout(data);
		return 0;
	}
	return 0;
}