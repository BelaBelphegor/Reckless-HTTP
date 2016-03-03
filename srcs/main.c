#include <reckless.h>
#include <reckless/http.h>

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
	t_http_response	*response;

	/*
	 * Read raw HTTP Request.
	 */
	ret = read(fd, buffer, BUFFER_SIZE);
	if (ret == 0 || ret == -1)
		logger(L_ERROR, "Unable to read HTTP Request on client socket", "", 0);
	if (ret > 0 && ret < BUFFER_SIZE)
		buffer[ret] = 0;
	i = 0;
	
	thr = create_http_request();
	response = create_http_response();
	init_http_request(thr, buffer);
	init_http_response(response, thr);

	/* Convert no filename "/" to index file. Next check the filetype and review if we support it. */
	size_t		buffer_length;
	size_t		len;
	char		*fstr;
	char		filepath[256];

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
		perror("File extension type not supported yet.");
	
	/*
	 * HTTP - Response.
	 * 1. Build filepath.
	 */
	bzero(filepath, 256);
	strcat(filepath, APPS_DIRECTORY);
	strcat(filepath, thr->request_uri);
	if ((file_fd = open(filepath, O_RDONLY)) == -1)
	{
		(void)sprintf(buffer, "%s 404 Not Found\r\nServer:Reckless/0.0.1\r\nKeep-Alive: timeout=30, max=98\r\nConnection: Keep-Alive\r\nContent-type: text/html; charset=iso-8859-1\r\n\r\n", thr->http_version);	
		file_fd = open(LAYOUTS_DIRECTORY"404.html", O_RDONLY);
	}
	else
		(void)sprintf(buffer, "%s 200 OK\r\nServer:Reckless/0.0.1\r\nContent-Type: %s\r\n\r\n", thr->http_version, fstr);

	write(fd, buffer, strlen(buffer));
	while ((ret = read(file_fd, buffer, BUFFER_SIZE)) > 0)
		write(fd, buffer, ret);	
	/*
	 * Release allocated structures.
	 */
	destroy_http_request(thr);
	destroy_http_response(response);
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
