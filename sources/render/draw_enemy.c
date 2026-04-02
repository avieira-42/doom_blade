#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_structs.h"
#include "cub_utils.h"

// Draw masked receives a source sprite, the position on the screen where it will be drawn,
// the scale that will determine its new size, and a frame struct that contains the zbuffer and screen dimensions.
// First we figure out the size of the new sprite using the scale, that will also determine the rectangle area of the screen it will occupy
// With this area defined, we step through X and Y with normalized ranges from 0 to 1 floats to be used in bilerp
// But only if the column being drawn has a wall perp_dist (zbuffer) higher than the enemy perp_dist

static inline
void	stt_clip(t_form *form, t_vec2 new_size)
{
	int32_t	unclipped_x;
	int32_t	unclipped_y;

	unclipped_x = form->draw_pos.x.i - new_size.x.i / 2;
	unclipped_y = form->draw_pos.y.i - new_size.y.i / 2;
	form->left = ft_iclamp(unclipped_x, 0, RENDER_WIDTH);
	form->right = ft_iclamp(form->draw_pos.x.i + new_size.x.i / 2, 0, RENDER_WIDTH);
	form->top = ft_iclamp(unclipped_y, 0, RENDER_HEIGHT);
	form->bottom = ft_iclamp(form->draw_pos.y.i + new_size.y.i / 2, 0, RENDER_HEIGHT);
	form->norm_offset.x.f = ((int)form->left - unclipped_x) * form->delta.x.f;
	form->norm_offset.y.f = ((int)form->top - unclipped_y) * form->delta.y.f;
}

static inline
float	stt_init(t_form *form, t_frame *frame, t_view *p, t_enemy *enemy)
{
	t_vec2	new_size;
	float	enemy_dist;
	float	horz_dist;
	t_vec2	rel_pos;
	float	invd;

	invd = 1.0f / (p->plane.x.f * p->dir.y.f - p->dir.x.f * p->plane.y.f);
	rel_pos.x.f = enemy->cam.pos.x.f - p->pos.x.f;
	rel_pos.y.f = enemy->cam.pos.y.f - p->pos.y.f;
	enemy_dist = (-p->plane.y.f * rel_pos.x.f + p->plane.x.f * rel_pos.y.f);
	enemy_dist *= invd;
	if (enemy_dist <= NEAR_RADIUS)
		return (enemy_dist);
	horz_dist = invd * (p->dir.y.f * rel_pos.x.f - p->dir.x.f * rel_pos.y.f);
	invd = 1.0f / enemy_dist;	// Scale
	form->draw_pos.x.i = (RENDER_WIDTH * 0.5f) * (1.0f + horz_dist * invd);
	form->draw_pos.y.i = RENDER_HEIGHT * 0.5f - frame->offset;
	new_size.x.i = enemy->texture.width * invd;
	new_size.y.i = enemy->texture.height * invd;
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
		c = ft_bilerp_argb_t(texture, norm_pos);	// Bilerp takes a normalized range to sample from
		if (c != 0xFF000000)		// TODO: Proper alpha blend
			ptr[y] = c;
		norm_pos.y.f += form->delta.y.f;
		y++;
	}
}

// Returns true if the enemy is within a NxN pixel grid on the center of the screen
// Could add a check to see if the pixels belong to ignored alpha color, but id need to map screen space coordinate
// to texture space, and that sounds like too much work. its not a bug its a feature
static inline
bool	stt_hitreg(t_form *form)
{
	bool	value;

	value = form->left < ((RENDER_WIDTH + HITREG_AREA) / 2)
		&& form->right > ((RENDER_WIDTH - HITREG_AREA) / 2)
		&& form->top < ((RENDER_HEIGHT + HITREG_AREA) / 2)
		&& form->bottom > ((RENDER_HEIGHT - HITREG_AREA) / 2);
	return (value);
}

static
bool	stt_draw_enemy(t_frame *frame, t_rayhit *rays, t_player *player, t_enemy *enemy)
{
	uint32_t	x;
	t_form		form;
	t_vec2		norm_pos;
	uint32_t	*ptr;
	const float	enemy_dist = stt_init(&form, frame, &player->cam, enemy);	// Could be dist

	if (enemy_dist <= NEAR_RADIUS)
		return (false);
	x = form.left;
	norm_pos.x.f = form.norm_offset.x.f;
	while (x < form.right)
	{
		if (enemy_dist < rays[x].perp_dist)	// Do not draw if wall column is ahead of enemy
		{
			ptr = frame->render.ptr + frame->render.stride * x;
			stt_draw_col(norm_pos, &form, ptr, &enemy->texture);
		}
		norm_pos.x.f += form.delta.x.f;
		x++;
	}
	return (stt_hitreg(&form));
}

void	cub_draw_enemies(t_game *game, long dt)
{
	size_t	i;
	bool	hit;
	t_enemy	*enemy;

	i = 0;
	while (i < NUM_ENEMIES)
	{
		enemy = game->enemies + i;
		if (enemy->health > 0)
		{
			hit = stt_draw_enemy(&game->frame, game->frame.rays, &game->player, enemy);
			if ((game->player.state & st_shot) && hit == true)
				enemy->health -= 50;
		}
		else
		{
			enemy->respawn_timer += dt;
			if (enemy->respawn_timer > RESPAWN_TIMER)
			{
				enemy->respawn_timer = 0;
				enemy->cam.pos = random_valid_pos(&game->map);
				enemy->health = 100;
			}
		}
		i++;
	}
	game->player.state &= ~(size_t)st_shot;	// Clears the just shot flag
}