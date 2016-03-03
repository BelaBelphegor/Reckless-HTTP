#include <reckless/http.h>
#include <reckless.h>

t_http_response			*create_http_response(void)
{
	t_http_response		*thr;

	thr = NULL;
	if (!(thr = (t_http_response *)malloc(sizeof(t_http_response))))
		return (NULL);
	bzero(thr, sizeof(t_http_response));
	return (thr);
}

int						init_http_response(t_http_response *thr, t_http_request *rqst)
{
	(void)thr;
	(void)rqst;
	return (0);
}

int						build_http_response(t_http_response *thr, t_http_request *rqst, char *buffer)
{
	(void)thr;
	(void)rqst;
	(void)buffer;
	return (0);
}

void					destroy_http_response(t_http_response *thr)
{
	if (!thr)
		return;
	if (thr->http_version)
		free(thr->http_version);
	if (thr->reason_phrase)
		free(thr->reason_phrase);
	free(thr);
	thr = NULL;
}
