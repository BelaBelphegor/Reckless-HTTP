#include <reckless.h>

t_extensions	extensions[] = 
{
	{"gif", "image/gif" },
	{"jpg", "image/jpeg" },
	{"jpeg", "image/jpeg" },
	{"png", "image/png" },
	{"zip", "image/zip" },
	{"gz", "image/gz" },
	{"tar","image/tar" },
	{"htm", "text/html" },
	{"html", "text/html" },
	{"css","text/css"},
	{0, 0} 
};

static void		ft_usage(void)
{
	dprintf(1, "Usage: ./reckless [PORT]\n");
}

void		web(int fd, int hit)
{
	static char		buffer[BUFFER_SIZE + 1];
	int				ret;
	int				file_fd;
	int				i;
	int				j;

	ret = read(fd, buffer, BUFFER_SIZE);
	if (ret == 0 || ret == -1)
		dprintf(2, "Failed to read browser request.\n");
	if (ret > 0 && ret < BUFFER_SIZE)
		buffer[ret] = 0;
	i = 0;
	while (i < ret)
	{
		if (buffer[i] == '\r' || buffer[i] == '\n')
			buffer[i] = '*';
		i++;
	}
	
	// Method used on query.
	if (strncmp(buffer, "GET ", 4) && strncmp(buffer, "get ", 4))
		dprintf(2, "Failed only get is supported actually");
	
	/* Add (null terminated) after the second space to ignore extra stuff. */
	i = 4;
	while (i < BUFFER_SIZE)
	{
		if (buffer[i] == ' ')
			buffer[i] = 0;
		i++;
	}
	j = 0;
	while (j < i - 1)
	{
		if (buffer[j] == '.' && buffer[j + 1] == '.')
		{
			perror("Parent directory path names not supported");
			(void)exit(-1);
		}
		j++;
	}

	/* Convert no filename "/" to index file. Next check the filetype and review if we support it. */
	size_t		buffer_length;
	size_t		len;
	char		*fstr;

	if (!strncmp(&buffer[0], "GET /\0", 6) || !strncmp(&buffer[0], "get /\0", 6))
	(void)strcpy(buffer, "GET /index.html");

	buffer_length = strlen(buffer);
	fstr = NULL;
	i = 0;
	while (extensions[i].ext != 0)
	{
		len = strlen(extensions[i].ext);
		if (!strncmp(&buffer[buffer_length - len], extensions[i].ext, len))
		{
			fstr = extensions[i].filetype;
			break;
		}
		i++;
	}
	if (!fstr)
	{
		perror("File extension type not supported yet.");
	}
	if ((file_fd = open(&buffer[5], O_RDONLY)) == -1)
		dprintf(2, "Failed to open file\n");
	(void)sprintf(buffer, "HTTP/1.0 200 OK\r\nServer:Reckless/0.0.1\r\nContent-Type: %s\r\n\r\n", fstr);
	write(fd, buffer, strlen(buffer));
	
	while ((ret = read(file_fd, buffer, BUFFER_SIZE)) > 0)
		write(fd, buffer, ret);
	sleep(1);	
	close(file_fd);
	(void)exit(1);
}


int				main(int argc, char **argv)
{
/*	int							pid;
	int							listen_fd;
	int							socket_fd;
	int							port;
	static struct sockaddr_in	cli_addr;
	static struct sockaddr_in	serv_addr:*/
	int							curr;
	/*unsigned int				socklen; */
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
	/*if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		dprintf(2, "Unable to set socket.\n");
	port = atoi(argv[1]);
	if (port < 0 || port > 60000)
		dprintf(2, "Invalid port number\n");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
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
	}*/
	start(argv);
	return (0);
}
