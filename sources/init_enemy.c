#include <X11/X.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

#define ENEMY_WALK "assets/sprites/xpm/characters/zombie/enemy_walking_front"
#define ENEMY_ATK "assets/sprites/xpm/characters/zombie/enemy_shooting"
#define ENEMY_HIT "assets/sprites/xpm/characters/zombie/enemy_shot"
#define ENEMY_DEATH "assets/sprites/xpm/characters/zombie/enemy_exploding"

void	enemy_init(t_game *game, t_enemy enemies[NUM_ENEMIES])
{
	size_t	i;

	i = 0;
	ft_memset(enemies, 0, sizeof(t_enemy) * NUM_ENEMIES);
	enemies[0].running = cub_readsheet(game, ENEMY_WALK, 4, ANIM_TIME * 3);
	enemies[0].shooting = cub_readsheet(game, ENEMY_ATK, 2, ANIM_TIME * 3);
	enemies[0].shot = cub_readsheet(game, ENEMY_HIT, 1, ANIM_TIME * 3);
	enemies[0].dying = cub_readsheet(game, ENEMY_DEATH, 12, ANIM_TIME * 1);
	enemies[0].state = e_idle;
	enemies[0].health = 100;	// Enemies spawn dead
	enemies[0].cam.pos = random_valid_pos(&game->map);
	while (i < NUM_ENEMIES)
	{
		enemies[i] = enemies[0];	// Now enemies all draw from the same texture
		enemies[i].cam.pos = random_valid_pos(&game->map);
		i++;
	}
}
