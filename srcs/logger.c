/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiboitel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/26 19:10:55 by tiboitel          #+#    #+#             */
/*   Updated: 2016/02/26 19:43:09 by tiboitel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <reckless.h>

void		logger(t_log_type type, char *s1, char *s2, int num)
{
	int		fd;
	char	logbuffer[BUFFER_SIZE * 2];

	fd = 0;
	bzero(logbuffer, BUFFER_SIZE * 2);
	if (type == L_ERROR)
		sprintf(logbuffer, "[ERROR]: %s : %s Errno:%d Exist PID:%d", s1, s2, errno, getpid());
	else if (type == L_SORRY)
	{
		sprintf(logbuffer, "<html><body><h1>Reckless HTTP</h1><p>Sorry, %s%s</p></body></html>\r\n", s1, s2);
		write(num, logbuffer, strlen(logbuffer));
		sprintf(logbuffer, "[SORRY]: %s : %s", s1, s2);
	}
	else if (type == L_LOG)
		sprintf(logbuffer, "[LOG] %s : %s", s1, s2);
	else
		sprintf(logbuffer, "[UNKNOWN] %s : %s\n", s1, s2);
	if ((fd = open("reckless.log", O_CREAT | O_WRONLY | O_APPEND, 0777)) >= 0)
	{
		write(fd, logbuffer, strlen(logbuffer));
		write(fd, "\n", 1);
		close(fd);
	}
	if (type == L_ERROR || type == L_SORRY)
		exit(3);
}
