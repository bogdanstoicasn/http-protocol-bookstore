#### Copyright 2024 Stoica Mihai-Bogdan 325CA (bogdanstoicasn@yahoo.com)

# Project Web Client - REST API Communication

## Overview

The project consists of a web client that communicates with a REST API.
The user sends HTTP requests to the server and receives responses about the
status of the request.
The user has access to an online library where it can view, add and delete
books. The library is unique for each user and is stored in a database.
In addition to this, the user uses cookies and tokens to ensure the security
of the user's data.

## Table of contents

- [Overview](#overview)
- [Table of contents](#table-of-contents)
- [Fast run](#fast-run)
- [Client](#client)
- [Commands](#commands)
- [Libraries](#libraries)
- [Hierarchy](#hierarchy)
- [Acknowledgements](#acknowledgements)
- [Resources](#resources)

## Fast run


Packages needed:
* `g++`
* `make`

To run the program basic tests, clone my repo and type the following command in the terminal:

- `make script`

## Client

Request == Command

The user can make requests indefinitely until it decides to exit the program.
That's why for every request, the program opens a new connection to the server
to avoid any problems that may occur(timeouts, etc).
The request is sent to the server(through command line) and the response is
received and processed according to the request made by the user.

## Commands

The user can make the following requests:

- **`register`** - it's followed by a prompt to put the username and password.

  **Exceptions:** spaces/tabs in the username or password, username already exists.

- **`login`** - it's followed by a prompt to put the username and password.

    **Exceptions:** spaces/tabs in the username or password, wrong username or password.

- **`enter_library`** - enters the library of the user.

    **Exceptions:** user is not logged in.

- **`get_books`** - gets the books from the library.

    **Exceptions:** user is not logged in/not in the library.

- **`get_book`** - it's followed by a prompt to put the id of the book.

    **Exceptions:** user is not logged in/ not in the library/ book doesn't exist.

- **`add_book`** - it's followed by a prompt to put the title, author, genre, publisher and page count of the book.

    **Exceptions:** user is not logged in/ not in the library.

- **`delete_book`** - it's followed by a prompt to put the id of the book.

    **Exceptions:** user is not logged in/ not in the library/ book doesn't exist.

- **`logout`** - logs out the user(deletes the cookies and the token).

    **Exceptions:** user is not logged in.

- **`exit`** - exits the program.

    **Exceptions:** none.

We save the cookies from the login session.
We save the token from the enter_library session.
Every command returns feedback to the user(ERROR, SUCCESS, etc).


## Libraries

We use the **nlohmann/json** library to parse the json responses from the server.
I decided to use cpp because the library is very easy to use and it's very well documented.
Every command has its own function that sends the request to the server and receives the response.
The the bytes we receive are parsed using the json library and then printed(ERROR, SUCCESS, etc) to the user's screen.
The required command is processed by a command handler.


## Hierarchy

```
├── client.cpp - main program
├── cmd.cpp - user -> server commands
├── cmd.h 
├── connection.cpp - connect/send/receive data
├── connection.h
├── json.hpp - json library
├── Makefile
├── README.md
├── req.cpp - builder for HTTP requests
├── req.h
├── utils.cpp - utility functions(cmd_handler, etc)
└── utils.h - macros
```

## Acknowledgements

I would like to thank the `UPB PCOM` team for providing me with the necessary test cases to implement the project. I used the labs to make some of the functions.


## Resources

- [nlohmann/json](https://github.com/nlohmann/json)
- [HTTP/1.1](https://tools.ietf.org/html/rfc2616)
