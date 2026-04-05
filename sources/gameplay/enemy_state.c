#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

static
int	stt_is_enemy_shooting(t_enemy *enemy)
{
	if (ft_randf() > ENEMY_ATTACK_AGRESS &&
			ft_abs(enemy->dist) < ENEMY_ATTACK_DIST
			&& enemy->state & e_seen)  // DEFINE ENEMY_AGGRESSIVENES and DIST
		return (1);
	return (0);
}

static
void	stt_update_state(t_enemy *enemy, long dt, t_player *player)
{
	uint8_t	rvalue;

	if (enemy->state & e_dying)
	{
		rvalue = cub_advance_animation(&enemy->dying, dt);
		if (rvalue & (1 << 2))
		{
			player->health += HEAL_VALUE;
			enemy->state = e_dead;
		}
		return ;
	}
	if (enemy->state & e_dead)
		return ;
	if (enemy->state & e_hit)
	{
		rvalue = cub_advance_animation(&enemy->shot, dt);
		if (rvalue & (1 << 2))
		{
			enemy->state &= ~(size_t) e_hit;
			enemy->state |= e_running;
		}
		return ;
	}
	if (enemy->state & e_shooting)
	{
		rvalue = cub_advance_animation(&enemy->shooting, dt);
		if (rvalue & (1 << 2))
		{
			enemy->state &= ~(size_t)e_shooting;
			enemy->state |= e_running;
			enemy->shooting.index = 0;
			enemy->shooting.frame_dt = 0;
		}
		return ;
	}
	if (enemy->health <= 0)
	{
		enemy->state = e_dying;
		enemy->dying.index = 0;
		enemy->dying.frame_dt = 0;
		return ;
	}
	cub_advance_animation(&enemy->running, dt);
	if (stt_is_enemy_shooting(enemy))
	{
		enemy->state &= ~(size_t)e_running;
		enemy->state |= e_shooting;
		enemy->shooting.index = 0;
		enemy->shooting.frame_dt = 0;
		player->health -= ENEMY_DAMAGE;
		enemy->state &= ~(size_t)e_seen;
	}
}

void	update_enemy_state(t_game *game, t_enemy *enemy, long dt, bool hit)
{
	stt_update_state(enemy, dt, &game->player);
	if ((game->player.state & st_shot) && hit)
	{
		if (!(enemy->state & e_dying))
		{
			enemy->health -= PLAYER_DAMAGE;
			if (enemy->health <= 0)
			{
				enemy->state = e_dying;
				enemy->dying.index = 0;
				enemy->dying.frame_dt = 0;
			}
			else if (!(enemy->state & e_hit))
			{
				enemy->state = e_hit;
				enemy->shot.index = 0;
				enemy->shot.frame_dt = 0;
			}
		}
	}
}
