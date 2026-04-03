#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cub_structs.h"
#include "cub_utils.h"

int	is_enemy_shooting(void)
{
	const int	options[3] = {0, 1, 2};

	if (options[rand() % 1] == 2)
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
		r = stt_render_animation(&e->dying, dt);
		if (r & (1 << 2))
			e->state = e_dead;
		return ;
	}
	if (e->state & e_dead)
		return ;
	if (e->state & e_hit)
	{
		r = stt_render_animation(&e->shot, dt);
		if (r & (1 << 2))
		{
			e->state &= ~e_hit;
			e->state |= e_running;
		}
		return ;
	}
	if (e->state & e_shooting)
	{
		r = stt_render_animation(&e->shooting, dt);
		if (r & (1 << 2))
		{
			e->state &= ~e_shooting;
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
	stt_render_animation(&e->running, dt);
	if (is_enemy_shooting())
	{
		e->state &= ~e_running;
		e->state |= e_shooting;
		e->shooting.index = 0;
		e->shooting.frame_dt = 0;
		player->health -= 20;
	}
}

static inline
void	stt_clip(t_form *form, t_vec2 new_size)
{
	const int32_t	ux = form->draw_pos.x.i - new_size.x.i / 2;
	const int32_t	uy = form->draw_pos.y.i - new_size.y.i / 2;

	form->left = ft_iclamp(ux, 0, RENDER_WIDTH);
	form->right = ft_iclamp(form->draw_pos.x.i + new_size.x.i / 2, 0,
			RENDER_WIDTH);
	form->top = ft_iclamp(uy, 0, RENDER_HEIGHT);
	form->bottom = ft_iclamp(form->draw_pos.y.i + new_size.y.i / 2, 0,
			RENDER_HEIGHT);
	form->norm_offset.x.f = ((int)form->left - ux) * form->delta.x.f;
	form->norm_offset.y.f = ((int)form->top - uy) * form->delta.y.f;
}

static inline
float	stt_init(t_form *form, t_frame *frame, t_view *p, t_enemy *enemy)
{
	t_vec2			new_size;
	float			enemy_dist;
	float			horz_dist;
	t_vec2			rel_pos;
	float			invd;
	const t_mat32	tex = enemy_get_frame(enemy);

	invd = 1.0f / (p->plane.x.f * p->dir.y.f - p->dir.x.f * p->plane.y.f);
	rel_pos.x.f = enemy->cam.pos.x.f - p->pos.x.f;
	rel_pos.y.f = enemy->cam.pos.y.f - p->pos.y.f;
	enemy_dist = (-p->plane.y.f * rel_pos.x.f + p->plane.x.f * rel_pos.y.f);
	enemy_dist *= invd;
	if (enemy_dist <= NEAR_RADIUS)
		return (enemy_dist);
	horz_dist = invd * (p->dir.y.f * rel_pos.x.f - p->dir.x.f * rel_pos.y.f);
	invd = 1.0f / enemy_dist;
	form->draw_pos.x.i = (RENDER_WIDTH * 0.5f) * (1.0f + horz_dist * invd);
	form->draw_pos.y.i = RENDER_HEIGHT * 0.5f - frame->offset + 15;
	new_size.x.i = tex.width * invd;
	new_size.y.i = tex.height * invd;
	form->delta.x.f = 1.0 / new_size.x.i;
	form->delta.y.f = 1.0 / new_size.y.i;
	stt_clip(form, new_size);
	return (enemy_dist);
}

static inline
void	stt_draw_col(t_vec2 norm_pos, t_form *form, uint32_t *ptr, t_mat32 *texture)
{
	uint32_t	y;
	uint32_t	c;

	y = form->top;
	norm_pos.y.f = form->norm_offset.y.f;
	while (y < form->bottom)
	{
		c = ft_bilerp_argb_t(texture, norm_pos);
		if (c != 0xFF000000 && c != 2228223 && c != 1441791)
			ptr[y] = c;
		norm_pos.y.f += form->delta.y.f;
		y++;
	}
}

static inline
bool	stt_hitreg(t_form *form)
{
	return (form->left < ((RENDER_WIDTH  + HITREG_AREA) / 2)
		&& form->right > ((RENDER_WIDTH  - HITREG_AREA) / 2)
		&& form->top < ((RENDER_HEIGHT + HITREG_AREA) / 2)
		&& form->bottom > ((RENDER_HEIGHT - HITREG_AREA) / 2));
}

static
bool	stt_draw_enemy(t_frame *frame, t_rayhit *rays,
		t_player *player, t_enemy *enemy)
{
	t_form		form;
	t_vec2		norm_pos;
	uint32_t	*ptr;
	uint32_t	x;
	t_mat32		tex;
	float		enemy_dist;

	tex = enemy_get_frame(enemy);
	enemy_dist = stt_init(&form, frame, &player->cam, enemy);
	if (enemy_dist <= NEAR_RADIUS)
		return (false);
	x = form.left;
	norm_pos.x.f = form.norm_offset.x.f;
	while (x < form.right)
	{
		if (enemy_dist < rays[x].perp_dist)
		{
			ptr = frame->render.ptr + frame->render.stride * x;
			stt_draw_col(norm_pos, &form, ptr, &tex);
		}
		norm_pos.x.f += form.delta.x.f;
		x++;
	}
	return (stt_hitreg(&form));
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
		enemy_update_anim(enemy, dt, &game->player);
		if (!(enemy->state & e_dead))
		{
			hit = stt_draw_enemy(&game->frame, game->frame.rays,
					&game->player, enemy);
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
	game->player.state &= ~(size_t)st_shot;
}
