/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiboitel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/27 04:57:03 by tiboitel          #+#    #+#             */
/*   Updated: 2016/02/27 06:38:47 by tiboitel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RECKLESS_HTTP_H
# define RECKLESS_HTTP_H

#include <reckless.h>

typedef enum					e_connection_state
{
	C_CLOSE, C_KEEP_ALIVE
}								t_connection_state;

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

/*
 * 1. RECKLESS_HTTP_PROTOCOL
 *
 * 1.1 HTTP_REQUEST
 */
t_http_request					*create_http_request(void);
int								init_http_request(t_http_request *thr, char *buffer);
int								read_header_http_request(t_http_request *thr, char *buffer);
void							show_http_request(t_http_request *thr);
void							destroy_http_request(t_http_request *thr);
/*
 * 1.2 HTTP_RESPONSE
 */

#endif
