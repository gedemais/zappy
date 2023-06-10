#include "main.h"

uint8_t	cat_spaced_number(t_env *env, int n, bool newline)
{
	char	*s;

	if (!(s = ft_itoa(n)))
		return (ERR_MALLOC_FAILED);

	strcat(env->buffers.response, s);
	strcat(env->buffers.response, newline ? "\n" : " ");
}
