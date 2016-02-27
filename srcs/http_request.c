/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiboitel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/26 18:00:27 by tiboitel          #+#    #+#             */
/*   Updated: 2016/02/27 05:05:57 by tiboitel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <reckless.h>
#include <reckless/http.h>

t_http_request		*create_http_request(void)
{
	t_http_request		*thr;

	if (!(thr = (t_http_request	*)malloc(sizeof(t_http_request))))
		return (NULL);
	bzero(thr, sizeof(t_http_request));
	return (thr);
}

int				init_http_request(t_http_request *thr, char *buffer)
{
	if (!thr)
	{
		logger(L_ERROR, "Unable to init HTTP Request. Argument null given.", "", 0);
		return (-1);
	}
	read_header_http_request(thr, buffer);
	return (1);
}

int				read_header_http_request(t_http_request *thr, char *buffer)
{
	int		i;
	int		j;

	i = 0;
	if (!thr || !buffer)
	{
		logger(L_ERROR, "Unable to read HTTP Header. Argument null given", "", 0);
		return (-1);
	}
	/*
	 * Convert CRLF on *
	 */
	while (i < strlen(buffer))
	{
		if (buffer[i] == '\r' || buffer[i] == '\n')
			buffer[i] = '*';
		i++;
	}
	/*
	 * Get method claim on query.
	 */
	if (!strncmp(buffer, "GET ", 4) || !strncmp(buffer, "get ", 4))
		thr->method = E_GET;
	else
	{
		logger(L_LOG, "The method claim by client is unavailable ", buffer, 0);
		return (-1);
	}
	/*
	 * Get HTTP Version.
	 */
	i = 0;
	while (strncmp(buffer + i, "HTTP/", strlen("HTTP/")) && i < strlen(buffer))
		i++;
	thr->http_version = strndup(buffer + i, 8);
	/*
	 * Get position on the second space.
	 */
	i = 4;
	while ((i < strlen(buffer)) && (buffer[i] != ' '))
		i++;
	j = 0;
	while (j < i - 1)
	{
		if (buffer[j] == '.' && buffer[j + 1] == '.')
		{
			logger(L_LOG, "Parent directory path is not supported ", &buffer[j], 0);
			return (-1);
		}
		j++;
	}
	/*
	 * Convert no filename to index file.
	 */
	if (!strncmp(&buffer[0], "GET / ", 6) || !strncmp(&buffer[0], "GET / ", 6))
		thr->request_uri = strdup("index.html");
	else
		thr->request_uri = strndup(buffer + 5, j - 4);
	return (1);
}

void				show_http_request(t_http_request *thr)
{
	if (!thr)
		return;
	logger(L_LOG, "#### SHOW_HTTP_REQUEST ####", "\n", 0);
	if (thr->http_version)
		logger(L_LOG, "HTTP Version" , thr->http_version, 0);
	if (thr->request_uri)
		logger(L_LOG, "HTTP Request uri", thr->request_uri, 0);
}

void				destroy_http_request(t_http_request *thr)
{
	if (!thr)
		return;
	if (thr->http_version)
		free(thr->http_version);
	if (thr->request_uri)
		free(thr->request_uri);
	free(thr);
}
