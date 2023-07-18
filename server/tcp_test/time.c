#include <stdio.h>
#include <sys/time.h>

int	main(void)
{
	struct timeval tv;

	while(1)
	{
		gettimeofday(&tv, NULL);
		printf("%ld\n", tv.tv_usec);
	}
}
