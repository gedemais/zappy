#include "main.h"

bool	is_only_number(char *s)
{
	for (unsigned int i = 0; s[i]; i++)
	{
		if (isdigit(s[i]) == 0)
			return (false);
	}
	return (true);
}

uint8_t	cat_spaced_number(t_env *env, int n, bool newline)
{
	char	*s;

	if (!(s = ft_itoa(n)))
		return (ERR_MALLOC_FAILED);

	strcat(env->buffers.gresponse, s);
	strcat(env->buffers.gresponse, newline ? "\n" : " ");
	return (ERR_NONE);
}
