#ifndef ERROR_H
# define ERROR_H

# include <stdbool.h>

enum	e_error_id
{
	ERR_NONE,
    ERR_MALLOC_FAILED,
    ERR_MISSING_ARGUMENT,
	ERR_INVALID_OPT,
    ERR_INVALID_PORT_NUMBER,
	ERR_INVALID_MAP_DIMS,
	ERR_INVALID_TIME_SETTING,
	ERR_INVALID_CONNECTION_ARG,
	ERR_DUPLICATE_OPT,
	ERR_MISSING_OPT,
	ERR_CMD_NOT_FOUND,
	ERR_SOCKET_FAILED,
	ERR_SETSOCKOPT_FAILED,
	ERR_BIND_FAILED,
	ERR_LISTEN_FAILED,
	ERR_ACCEPT_FAILED,
	ERR_RECV_FAILED,
	ERR_SELECT_FAILED,
	ERR_TOO_MANY_TEAMS,
	ERR_MAX
};

typedef	struct	s_error
{
	char	msg[64]; // Error message
	bool	usage; // Does usage needs to be displayed
	bool	errno_display; // Does errno interpretation needs to be displayed
}				t_error;

static const t_error	errors[ERR_MAX] = {
	[ERR_MALLOC_FAILED] = {"malloc() call failed\n", false, true},
	[ERR_MISSING_ARGUMENT] = {"Missing argument for option\n", true, false},
	[ERR_INVALID_OPT] = {"Invalid option\n", true, false},
	[ERR_INVALID_PORT_NUMBER] = {"Invalid port number\n", true, false},
	[ERR_INVALID_MAP_DIMS] = {"Invalid map dimensions\n", true, false},
	[ERR_INVALID_TIME_SETTING] = {"Invalid time setting\n", true, false},
	[ERR_INVALID_CONNECTION_ARG] = {"Invalid max number of connection\n", true, false},
	[ERR_DUPLICATE_OPT] = {"Duplicate option in command line\n", true, false},
	[ERR_MISSING_OPT] = {"Missing option in command line\n", true, false},
	[ERR_CMD_NOT_FOUND] = {"Command not found\n", false, false},
	[ERR_SOCKET_FAILED] = {"Socket creation failed\n", false, true},
	[ERR_SETSOCKOPT_FAILED] = {"setsockopt() failed\n", false, true},
	[ERR_BIND_FAILED] = {"bind() failed\n", false, true},
	[ERR_LISTEN_FAILED] = {"listen() failed\n", false, true},
	[ERR_ACCEPT_FAILED] = {"accept() failed\n", false, true},
	[ERR_RECV_FAILED] = {"recv() failed\n", false, true},
	[ERR_SELECT_FAILED] = {"select() failed\n", false, true},
	[ERR_TOO_MANY_TEAMS] = {"Too many teams\n", true, false}
	//[] = {"", false}
	//[] = {"", false}
	//[] = {"", false}
	//[] = {"", false}
};


#endif
