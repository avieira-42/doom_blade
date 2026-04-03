#include "cub_utils.h"
#include "cub_structs.h"
#include <stdlib.h>
#include <stdio.h>

// to call on init (already exists)
void	enemy_init(t_game *game)
{
	size_t	i;
	
	i = 0;
	while (i < NUM_ENEMIES)
	{
	}
}

// to call on update state
void	enemy_update_state(t_game *game)
{
}

// to call every time the enemy shoots
int	is_enemy_shooting(void)
{
	const int	options[3] = {0, 1, 2};

	if (options[ft_rand() % 3] == 2)
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
