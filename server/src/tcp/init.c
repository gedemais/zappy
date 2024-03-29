#include "main.h"

// Creation of the server socket
static uint8_t	create_socket(t_tcp *tcp)
{
	const int	option = 1;
	int			opts = 0;

	// Socket creation
    if ((tcp->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror(strerror(errno));
		return (ERR_SOCKET_FAILED);
	}

	// Setting new socket options
    if (setsockopt(tcp->server_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) != 0
		|| setsockopt(tcp->server_fd, SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option)) != 0)
	{
		perror(strerror(errno));
        return (ERR_SETSOCKOPT_FAILED);
    }
	return (ERR_NONE);
}

static uint8_t	setup_tcp(uint16_t max_connections, t_tcp *tcp)
{
    tcp->address.sin_family = AF_INET;
    tcp->address.sin_addr.s_addr = INADDR_ANY;
    tcp->address.sin_port = htons(tcp->server_port); // Assign port to listen on later

	// Binds the server connection fd to its address
    if (bind(tcp->server_fd, (struct sockaddr*)&tcp->address, sizeof(tcp->address)) < 0)
	{
		perror(strerror(errno));
		return (ERR_BIND_FAILED);
    }

	// Listen on the specified port
	if (listen(tcp->server_fd, max_connections))
	{
		perror(strerror(errno));
		return (ERR_LISTEN_FAILED);
	}
	return (ERR_NONE);
}

static uint8_t	create_players_connections(t_env *env)
{
	size_t	size;

	size = sizeof(int) * 1024;
	if (!(env->buffers.connections = (int*)malloc(size))
		|| !(env->buffers.request = (char*)malloc(sizeof(char) * (REQUEST_BUFF_SIZE + 1))))
		return (ERR_MALLOC_FAILED);

	for (uint32_t i = 0; i < 1024; i++)
		env->buffers.connections[i] = -1;

	env->buffers.connections[0] = env->tcp.server_fd;
	return (ERR_NONE);
}

static uint8_t	create_pending_team(t_env *env)
{
	t_team	*new;

	new = &env->world.pending;
	if (dynarray_init(&new->players, sizeof(t_player), env->settings.max_connections) != 0)
		return (ERR_MALLOC_FAILED);
	return (ERR_NONE);
}

uint8_t			init_tcp(t_env *env)
{
	t_tcp	*tcp;
	uint8_t	code;

	tcp = &env->tcp;
	if ((code = create_socket(tcp))
		|| (code = setup_tcp(env->settings.max_connections, tcp))
		|| (code = create_pending_team(env))
		|| (code = create_players_connections(env)))
		return (code);

	return (ERR_NONE);
}
