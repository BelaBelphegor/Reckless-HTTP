#ifndef RECKLESS_H
# define RECKLESS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define CRLF "\r\n"
#define BUFFER_SIZE 8192

typedef enum					e_http_method
{
	E_GET, E_HEAD, E_POST, E_PUT, E_DELETE
}								t_http_method;

typedef struct					s_http_request
{
	t_http_method				method;
	char						*request_uri;
	char						*http_version;

}								t_http_request;

typedef struct					s_http_response
{
	char						*http_version;
	int							statut_code;
	char						*reason_phrase;
}								t_http_response;

typedef struct					s_extensions
{
	char						*ext;
	char						*filetype;
}								t_extensions;

typedef struct					s_reckless_server
{
	int							pid;
	int							listen_fd;
	int							socket_fd;
	int							port;
	struct sockaddr_in			cli_addr;
	struct sockaddr_in			serv_addr;
	int							hit;
	unsigned int				socklen;
	char						is_running;
}								t_reckless_server;

void							start(char **argv);
void							web(int fd, int hit);
# endif
