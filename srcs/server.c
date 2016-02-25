#include <reckless.h>

static t_reckless_server	*create_reckles_server(void)
{
	t_reckless_server	*server;

	if (!(server = (t_reckless_server *)malloc(sizeof(t_reckless_server))))
	{
		perror("Unable to start reckless service : ");
		exit(-1);
	}
	bzero(server, sizeof(t_reckless_server));
	return (server);
}

static void					init_reckless_server(t_reckless_server *server,
	char **argv)
{
	if ((server->listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		dprintf(2, "Unable to set socket.\n");
	server->port = atoi(argv[1]);
	if (server->port < 0 || server->port > 60000)
		dprintf(2, "Invalid port number\n");
	server->serv_addr.sin_family = AF_INET;
	server->serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server->serv_addr.sin_port = htons(server->port);
	if ((bind(server->listen_fd, (struct sockaddr *)&(server->serv_addr), 
		sizeof(server->serv_addr))) < 0)
		dprintf(2, "Unable to bind server socket\n");
	if (listen(server->listen_fd, 64) < 0)
		dprintf(2, "Unable to listen on server socket\n");
}

static void		listening(t_reckless_server *server)
{
	server->is_running = 1;
	while (server->is_running)
	{
		server->socklen = sizeof(server->cli_addr);
		if ((server->socket_fd = accept(server->listen_fd,
			(struct sockaddr *)&(server->cli_addr), &(server->socklen))) < 0)
		{
			dprintf(2, "Unable to accept client\n");
			perror(NULL);
		}
		if ((server->pid = fork()) < 0)
		{
			dprintf(2, "Unable to fork new client\n");
			(void)exit(-1);
		}
		else
		{
			if (server->pid == 0)
			{
				close(server->listen_fd);
				web(server->socket_fd, server->hit);
			}
			else
			{
				close(server->socket_fd);
			}
		}
		server->hit++;
	}
}

void		start(char **argv)
{
	t_reckless_server	*this;

	this = create_reckles_server();
	init_reckless_server(this, argv);
	listening(this);
}

