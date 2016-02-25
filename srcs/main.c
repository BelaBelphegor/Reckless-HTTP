#include <reckless.h>

static void		ft_usage(void)
{
	dprintf(1, "Usage: ./reckless [PORT]\n");
}

static void		web(int fd, int hit)
{
	static char		buffer[BUFFER_SIZE + 1];
	int				ret;
	int				file_fd;

	ret = read(fd, buffer, BUFFER_SIZE);
	if (ret == 0 || ret == -1)
		dprintf(2, "Failed to read browser request.\n");
	if (ret > 0 && ret < BUFFER_SIZE)
		buffer[ret] = 0;
	if ((file_fd = open("index.html", O_RDONLY)) == -1)
		dprintf(2, "Failed to open file\n");
	(void)sprintf(buffer, "HTTP/1.0 200 OK\r\nServer:Reckless/0.0.1\r\nContent-Type: %s\r\n\r\n", "text/html");
	write(fd, buffer, strlen(buffer));
	
	while ((ret = read(file_fd, buffer, BUFFER_SIZE)) > 0)
		write(fd, buffer, ret);
	sleep(1);	
	close(file_fd);
	(void)exit(1);
}

int				main(int argc, char **argv)
{
	int							pid;
	int							listen_fd;
	int							socket_fd;
	int							port;
	static struct sockaddr_in	cli_addr;
	static struct sockaddr_in	serv_addr;
	int							curr;
	unsigned int				socklen;
	(void)argv;
	if (argc != 2)
	{
		ft_usage();
		exit(-1);
	}

	curr = 0;
	while (curr < 32)
	{
		(void)close(curr);
		curr++;
	}
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		dprintf(2, "Unable to set socet.\n");
	port = atoi(argv[1]);
	if (port < 0 || port > 60000)
		dprintf(2, "Invalid port number\n");
	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(port);
	if ((bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
		dprintf(2, "Unable to bind server socket\n");
	if (listen(listen_fd, 64) < 0)
		dprintf(2, "Unable to listen on server socket\n");

	curr = 1;
	while (curr)
	{
		socklen = sizeof(cli_addr);
		if ((socket_fd = accept(listen_fd, (struct sockaddr *)&cli_addr, &socklen)) < 0)
		{
			dprintf(2, "Unable to accept client\n");
			perror(NULL);
		}
		if ((pid = fork()) < 0)
		{
			dprintf(2, "Unable to fork new client\n");
			(void)exit(-1);
		}
		else
		{
			if (pid == 0)
			{
				close(listen_fd);
				web(socket_fd, curr);
			}
			else
			{
				close(socket_fd);
			}
		}
		curr++;
	}
	return (0);
}
