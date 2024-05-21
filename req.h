#ifndef REQ_H
#define REQ_H

/*
	Computes the GET request.
	Returns the request.
*/
char *string_get_request(const char *host, const char *url,
						  char *cookies, char *token);

/*
	Computes the POST request.
	Returns the request.
*/
char *string_post_request(const char *host, const char *url,
						   const char *content_type,
						   const char *body_data,
						   char *cookies, char *token);

/*
	Computes the DELETE request.
	Returns the request.
*/
char *string_delete_request(const char *host, const char *url,
						  	 char *cookies, char *token);

#endif