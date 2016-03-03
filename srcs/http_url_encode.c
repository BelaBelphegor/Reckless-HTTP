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


}

char		*url_decode(char *str)
{
}
