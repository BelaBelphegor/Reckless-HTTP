/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_url_encode.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiboitel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 01:09:17 by tiboitel          #+#    #+#             */
/*   Updated: 2016/03/04 01:11:57 by tiboitel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <reckless.h>
#include <reckless/http.h>

/*
 * Convert hex character to base 10 value.
 */
char		xtoc(char c)
{
	return (isdigit(c) ? c - '0' : tolower(c) - 'a' + 10);
}

/*
 * Convert base 10 value to hex character.
 */
char		ctox(char c)
{
	static char hex[] = "0123456789abcdef";
	return (hex[c & 15]);
}

char		*url_encode(char *str)
{
	char	*tmp;
	char	*buffer;
	char	*ptr;

	if (!str || !(buffer = (char *)malloc(sizeof(char) * strlen(str) * 3 + 1)))
		return (NULL);
	tmp = str;
	ptr = buffer;
	while (tmp)
	{
		if (isalnum(*tmp) || *tmp == '-' || *tmp == '_' || *tmp == '.' || *tmp == '~')
			*ptr++ = *tmp;
		else if (*tmp == ' ')
			*ptr++ = '+';
		else
		{
			*ptr++ = '%';
			*ptr++ = ctox(*tmp >> 4);
			*ptr++ = ctox(*tmp & 15);
		}
		tmp++;
	}
	*ptr = '\0';
	return (buffer);
}

char		*url_decode(char *str)
{
	char	*tmp;
	char	*buffer;
	char	*ptr;

	if (!str || !(buffer = (char *)malloc(sizeof(char) * strlen(str) + 1)))
		return (NULL);
	tmp = str;
	ptr = buffer;
	while (tmp)
	{
		if (*tmp == '%')
		{
			if (tmp[1] && tmp[2])
			{
				*ptr++ = xtoc(tmp[1]) << 4 | xtoc(tmp[2]); 
				tmp += 2;
			}
		} else if (*tmp == '+')
			*ptr++ = ' ';
		else
			*ptr++ = *tmp;
		tmp++;
	}
	*ptr = '\0';
	return (buffer);
}
