#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_structs.h"
#include "cub_utils.h"

static inline
float	stt_init(t_form *form, t_frame *frame, t_view *p, t_enemy *enemy)
{
	t_vec2	new_size;
	float	horz_dist;
	t_vec2	rel_pos;
	float	invd;

	invd = 1.0f / (p->plane.x.f * p->dir.y.f - p->dir.x.f * p->plane.y.f);
	rel_pos.x.f = enemy->cam.pos.x.f - p->pos.x.f;
	rel_pos.y.f = enemy->cam.pos.y.f - p->pos.y.f;
	enemy->dist = (-p->plane.y.f * rel_pos.x.f + p->plane.x.f * rel_pos.y.f);
	enemy->dist *= invd;
	if (enemy->dist <= NEAR_RADIUS)
		return (enemy->dist);
	horz_dist = invd * (p->dir.y.f * rel_pos.x.f - p->dir.x.f * rel_pos.y.f);
	invd = (1.0f / enemy->dist) * ENEMY_SCALE; // Scale (TODO: check if enemy scale doesnt break math)
	form->draw_pos.x.i = (render_width * 0.5f) * (1.0f + horz_dist * invd);
	form->draw_pos.y.i = render_height * 0.5f - frame->offset;
	new_size.x.i = enemy->running.texture.width * invd;	// tex.width / enemy_dist
	new_size.y.i = enemy->running.texture.height * invd;
	form->delta.x.u = 65536.0f * enemy->dist / enemy->running.texture.width;	// REVIEW: These can be constants
	form->delta.y.u = 65536.0f * enemy->dist / enemy->running.texture.height;	// TODO: HACK
	form->bounds = cub_center_clip(frame->render, form->draw_pos, new_size);
	form->norm_offset.x.u = (form->bounds.left - form->bounds.x) * form->delta.x.u;
	form->norm_offset.y.u = (form->bounds.top - form->bounds.y) * form->delta.y.u;
	return (enemy->dist);
}

static inline
void	stt_draw_col(t_vec2 norm_pos, t_form *form, uint32_t *ptr, t_mat32 *src)
{
	uint32_t	y;
	uint32_t	c;
	uint32_t	xsrc;
	uint32_t	ysrc;

	y = form->bounds.top;
	norm_pos.y.u = form->norm_offset.y.u;
	while (y < form->bounds.bottom)
	{
		xsrc = (norm_pos.x.u * (src->width - 1)) >> 16;
		ysrc = (norm_pos.y.u * (src->height - 1)) >> 16;
		c = src->ptr[ysrc + src->stride * xsrc];
		if (c != 0)	// TODO: Check if a blend would be expensive (blend could be cheap like overwrite if not 0)
			ptr[y] = c;
		norm_pos.y.u += form->delta.y.u;
		y++;
	}
}

// Returns true if the enemy is within a NxN pixel grid on the center of the screen
// Could add a check to see if the pixels belong to ignored alpha color, but id need to map screen space coordinate
// to texture space, and that sounds like too much work. its not a bug its a feature
static inline
bool	stt_hitreg(t_sides *bounds)
{
	return (bounds->left < ((render_width  + HITREG_AREA) / 2)
		&& bounds->right > ((render_width  - HITREG_AREA) / 2)
		&& bounds->top < ((render_height + HITREG_AREA) / 2)
		&& bounds->bottom > ((render_height - HITREG_AREA) / 2));
}

static
bool	stt_draw_enemy(t_frame *frame, t_player *player, t_enemy *enemy)
{
	t_form		form;
	t_vec2		norm_pos;
	uint32_t	*ptr;
	uint32_t	x;

	if (stt_init(&form, frame, &player->cam, enemy) <= NEAR_RADIUS)
		return (false);
	x = form.bounds.left;
	norm_pos.x.u = form.norm_offset.x.u;
	while (x < form.bounds.right)
	{
		if (enemy->dist < frame->rays[x].perp_dist)	// Do not draw if wall column is ahead of enemy
		{
			ptr = frame->render.ptr + frame->render.stride * x;
			stt_draw_col(norm_pos, &form, ptr, &enemy->texture);
			enemy->state |= e_seen;		// REVIEW: is there anything that turns this off?
		}
		norm_pos.x.u += form.delta.x.u;
		x++;
	}
	return (stt_hitreg(&form.bounds));
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
			hit = stt_draw_enemy(&game->frame, &game->player, enemy);
			if (hit == true)
				enemy->state |= e_hit;
			else
				enemy->state &= ~(size_t) e_hit;
		}
		i++;
	}
}
