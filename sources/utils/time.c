#include <sys/time.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_structs.h"

// uint64_t 	time_get()
// {
// 	struct timeval	tv;

// 	if (gettimeofday(&tv, NULL) == -1)
// 		free_displays();
// 	return ((uint64_t)(tv.tv_sec * 1000000) + (tv.tv_usec));
// }

// void	time_delta_get(t_game *game)
// {
// 	game->dt = (time_get() - game->t0) / 1000000.0f;
// 	game->t0 = time_get();
// }

long	get_time(void)
{
	static long		prev_time = 0;
	long			dt;
	struct timeval	now;

	dt = -prev_time;
	gettimeofday(&now, NULL);
	prev_time = now.tv_sec * 1000000 + now.tv_usec;
	dt += prev_time;
	return (dt);
}

long	get_time_v2(void)
{
	static long		prev_time[8];	// Stores the result of the last N measurements
	static size_t	index = 0;
	long			dt;
	struct timeval	now;

	gettimeofday(&now, NULL);
	dt = -prev_time[index];
	index = (index + 1) % 8;
	prev_time[index] = now.tv_sec * 1000000 + now.tv_usec;
	dt += prev_time[index];
	return (dt);
}
