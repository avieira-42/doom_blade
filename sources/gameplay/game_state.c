#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

void	cub_update_pos(t_game *game, float dt);
void	update_enemy_state(t_player *player, t_enemy *enemy, long dt);

void	cub_update_game(t_game *game, long dt)
{
	size_t	i;
	t_enemy	*enemy;

	cub_update_pos(game, (double)dt * 0.000001);
	i = 0;
	while (i < NUM_ENEMIES)
	{
		enemy = game->enemies + i;
		update_enemy_state(&game->player, enemy, dt);
		i++;
	}
}
