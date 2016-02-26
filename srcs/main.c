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
	t_http_request	*thr;

	ret = read(fd, buffer, BUFFER_SIZE);
	if (ret == 0 || ret == -1)
		logger(L_ERROR, "Unable to read HTTP Request on client socket", "", 0);
	if (ret > 0 && ret < BUFFER_SIZE)
		buffer[ret] = 0;
	i = 0;
	
	thr = create_http_request();
	init_http_request(thr, buffer);
	destroy_http_request(thr);

	while (i < ret)
	{
		if (buffer[i] == '\r' || buffer[i] == '\n')
			buffer[i] = '*';
		i++;
	}	
	// Method used on query.
	if (strncmp(buffer, "GET ", 4) && strncmp(buffer, "get ", 4))
		logger(L_SORRY, "Failed only get is supported actually", "", 0);
	
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
	int			curr;

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
	start(argv);
	return (0);
}
