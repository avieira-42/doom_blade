#include "cub_utils.h"
#include "cub_structs.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

enum e_enemy_state
{
	e_idle = 0,
	e_running = 1 << 0,
	e_shooting = 1 << 1,
	e_dying = 1 << 2,
	e_dead = 1 << 3,
};

// to call on init
void	enemy_init(t_game *game)
{
	size_t	i;
	
	srand(time(NULL));
	i = 0;
	while (i < NUM_ENEMIES)
	{
	}
}

// to call every time the enemy shoots
int	is_enemy_shooting(void)
{
	const int	options[3] = {0, 1, 2};

	if (options[rand()% 3] == 2)
		return (1);
	return (0);
}

// for the update function
void	enemy_update_state(void)
{
}

// debug
/*int	main(void)
{
	int	i = 20;

	enemy_init();
	while (i--)
		printf("is_enemy_shooting = %i\n", is_enemy_shooting());
}*/
