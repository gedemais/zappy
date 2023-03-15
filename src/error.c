#include "main.h"

const char *usage = "Usage: ./server -p <port> -x <width> -y <height> -n <team> [<team>] [<team>] ... -c <nb> -t <t>\n\
-p port number\n\
-x world width\n\
-y world height\n\
-n team\\_name\\_1 team\\_name\\_2 ...\n\
-c number of clients authorized at the beginning of the game\n\
-t time unit divider (the greater t is, the faster the game will go\n";

uint32_t	error(t_env *env, unsigned char code)
{
	int		id;

	id = (int)code;
	write(2, "========== ERROR ==========\n", 28);
	write(2, errors[id].msg, strlen(errors[id].msg));
	fflush(stderr);

	if (errors[id].errno_display)
	{
		write(2, "\n", 1);
		write(2, strerror(errno), strlen(strerror(errno)));
		write(2, "\n", 1);
	}

	if (errors[id].usage)
		write(2, usage, strlen(usage));

	fflush(stderr);

	free_env(env);
	return (id);
}
