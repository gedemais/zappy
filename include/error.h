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
	ERR_MAX
};

typedef	struct	s_error
{
	char			msg[64]; // Error message
	bool	usage; // Does usage needs to be displayed
}				t_error;

static const t_error	errors[ERR_MAX] = {
	[ERR_MALLOC_FAILED] = {"malloc() call failed\n", false},
	[ERR_MISSING_ARGUMENT] = {"Missing argument for option\n", true},
	[ERR_INVALID_OPT] = {"Invalid option\n", true},
	[ERR_INVALID_PORT_NUMBER] = {"Invalid port number\n", true}
	//[] = {"", false}
	//[] = {"", false}
	//[] = {"", false}
};


#endif
