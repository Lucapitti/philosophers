#include "philosopher.h"

size_t	get_curr_time()
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return(time.tv_sec * 100 + time.tv_usec / 100);
}