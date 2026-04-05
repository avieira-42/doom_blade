#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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


void	enemy_update_anim(t_enemy *e, long dt, t_player *player)
{
	uint8_t	r;

	if (e->state & e_dying)
	{
		r = cub_advance_animation(&e->dying, dt);
		if (r & (1 << 2))
			e->state = e_dead;
		return ;
	}
	if (e->state & e_dead)
		return ;
	if (e->state & e_hit)
	{
		r = cub_advance_animation(&e->shot, dt);
		if (r & (1 << 2))
		{
			e->state &= ~(size_t) e_hit;
			e->state |= e_running;
		}
		return ;
	}
	if (e->state & e_shooting)
	{
		r = cub_advance_animation(&e->shooting, dt);
		if (r & (1 << 2))
		{
			e->state &= ~(size_t)e_shooting;
			e->state |= e_running;
			e->shooting.index = 0;
			e->shooting.frame_dt = 0;
		}
		return ;
	}
	if (e->health <= 0)
	{
		e->state = e_dying;
		e->dying.index = 0;
		e->dying.frame_dt = 0;
		return ;
	}
	cub_advance_animation(&e->running, dt);
	if (stt_is_enemy_shooting(e))
	{
		e->state &= ~(size_t)e_running;
		e->state |= e_shooting;
		e->shooting.index = 0;
		e->shooting.frame_dt = 0;
		player->health -= ENEMY_ATTACK_POWER;
		player->last_damage_time = get_time();
		player->regen_cd = REGEN_CD;
		e->state &= ~(size_t)e_seen;
	}
}



void	cub_draw_enemies(t_game *game, long dt)
{
	bool	hit;
	size_t	i;
	t_enemy	*enemy;

	i = 0;
	while (i < NUM_ENEMIES)
	{
		enemy = &game->enemies[i];
		if (!(enemy->state & e_dead))
		{
			hit = draw_enemy(&game->frame, game->frame.rays,
					&game->player, enemy);
			enemy_update_anim(enemy, dt, &game->player);
			if ((game->player.state & st_shot) && hit)
			{
				if (!(enemy->state & e_dying))
				{
					enemy->health -= 25;
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
		i++;
	}
}
