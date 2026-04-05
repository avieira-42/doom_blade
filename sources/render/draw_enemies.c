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

t_mat32	enemy_get_frame(t_enemy *enemy)
{
	t_sheet	*sheet;
	t_mat32	frame;
	size_t	offset;

	if (enemy->state & e_dying)
		sheet = &enemy->dying;
	else if (enemy->state & e_hit)
		sheet = &enemy->shot;
	else if (enemy->state & e_shooting)
		sheet = &enemy->shooting;
	else
		sheet = &enemy->running;
	offset = (size_t)sheet->index * (size_t)sheet->frame_size;
	frame = sheet->texture;
	frame.ptr = sheet->texture.ptr + offset;
	return (frame);
}

void	enemy_update_anim(t_enemy *e, long dt, t_player *player)
{
	uint8_t	r;

	if (e->state & e_dying)
	{
		r = cub_advance_animation(&e->dying, dt);
		if (r & (1 << 2))
		{
			player->health += HEAL_VALUE;
			e->state = e_dead;
		}
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
		e->state &= ~(size_t)e_seen;
	}
}

static
void	stt_enemy_update_stats(t_game *game, t_enemy *enemy, bool hit)
{
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

void	cub_draw_enemies(t_game *game, long dt)
{
	bool	hit;
	size_t	i;
	t_enemy	*enemy;
	t_mat32	tex;

	i = 0;
	while (i < NUM_ENEMIES)
	{
		enemy = &game->enemies[i];
		if (!(enemy->state & e_dead))
		{
			tex = enemy_get_frame(enemy);
			hit = draw_enemy(&game->frame, &game->player, enemy, tex);
			enemy_update_anim(enemy, dt, &game->player);
			stt_enemy_update_stats(game, enemy, hit);
		}
		i++;
	}
}
