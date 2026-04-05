#include <X11/X.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

#define ZOMBIE_WALK "assets/sprites/xpm/characters/zombie/enemy_walking_front"
#define ZOMBIE_ATK "assets/sprites/xpm/characters/zombie/enemy_shooting"
#define ZOMBIE_HIT "assets/sprites/xpm/characters/zombie/enemy_shot"
#define ZOMBIE_DEATH "assets/sprites/xpm/characters/zombie/enemy_exploding"

static
void	stt_zombie_init(t_game *game, t_enemy enemies[NUM_ENEMIES])
{
	size_t	i;

	i = 0;
	ft_memset(enemies, 0, sizeof(t_enemy) * NUM_ENEMIES);
	enemies[0].running = cub_readsheet(game, ZOMBIE_WALK, 4, ANIM_TIME * 3);
	enemies[0].shooting = cub_readsheet(game, ZOMBIE_ATK, 2, ANIM_TIME * 3);
	enemies[0].shot = cub_readsheet(game, ZOMBIE_HIT, 1, ANIM_TIME * 3);
	enemies[0].dying = cub_readsheet(game, ZOMBIE_DEATH, 12, ANIM_TIME * 1);
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

#define GUNMAN_WALK "assets/sprites/xpm/characters/gentleman/gentleman_running"
#define GUNMAN_ATK "assets/sprites/xpm/characters/gentleman/gentleman_shooting"
#define GUNMAN_HIT "assets/sprites/xpm/characters/gentleman/gentleman_hurt"
#define GUNMAN_DEATH "assets/sprites/xpm/characters/gentleman/gentleman_dying"

static
void	stt_gunman_init(t_game *game, t_enemy enemies[NUM_ENEMIES])
{
	size_t	i;

	i = 0;
	ft_memset(enemies, 0, sizeof(t_enemy) * NUM_ENEMIES);
	enemies[0].running = cub_readsheet(game, GUNMAN_WALK, 4, ANIM_TIME * 3);
	enemies[0].shooting = cub_readsheet(game, GUNMAN_ATK, 3, ANIM_TIME * 4);
	enemies[0].shot = cub_readsheet(game, GUNMAN_HIT, 1, ANIM_TIME * 3);
	enemies[0].dying = cub_readsheet(game, GUNMAN_DEATH, 4, ANIM_TIME * 2);
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

void	enemy_init(t_game *game, t_enemy enemies[NUM_ENEMIES])
{
	if (CONTEXT == a_hell)
		stt_zombie_init(game, enemies);
	else if (CONTEXT == a_castle)
		stt_gunman_init(game, enemies);
}
