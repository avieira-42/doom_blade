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
float	stt_dist(t_form *form, t_mat32 *frame, t_view player, t_vec2 enemy_pos)
{
	float		enemy_dist;
	float		horz_dist;
	t_vec2		rel_pos;
	const float	inv_det = 1.0f / (player.plane.x.f * player.dir.y.f - player.dir.x.f * player.plane.y.f);

	rel_pos.x.f = enemy_pos.x.f - player.pos.x.f;
	rel_pos.y.f = enemy_pos.y.f - player.pos.y.f;
	horz_dist = inv_det * (player.dir.y.f * rel_pos.x.f - player.dir.x.f * rel_pos.y.f);
	enemy_dist = inv_det * (-player.plane.y.f * rel_pos.x.f + player.plane.x.f * rel_pos.y.f);
	if (enemy_dist > NEAR_RADIUS)
	{
		form->draw_pos.x.i = (frame->width * 0.5f) * (1.0f + horz_dist / enemy_dist);
		form->draw_pos.y.i = frame->height * 0.5f;
	}
	return (enemy_dist);
}

static inline
float	stt_init(t_form *form, t_mat32 *frame, t_player *player, t_enemy *enemy)
{
	t_vec2			new_size;
	int32_t			unclipped;
	const float		enemy_dist = stt_dist(form, frame, player->cam, enemy->cam.pos);
	float			scale;

	if (enemy_dist <= NEAR_RADIUS)
		return (enemy_dist);
	scale = 1.0 / enemy_dist;
	new_size.x.i = scale * enemy->texture.width;
	new_size.y.i = scale * enemy->texture.height;
	form->delta.x.f = 1.0 / new_size.x.i;
	form->delta.y.f = 1.0 / new_size.y.i;

	unclipped = (int)form->draw_pos.x.i - (int)new_size.x.i / 2;
	form->left = ft_iclamp(unclipped, 0, frame->width);
	form->norm_offset.x.f = ((int)form->left - unclipped) * form->delta.x.f;	// Clipped start
	form->right = ft_iclamp((int)form->draw_pos.x.i + (int)new_size.x.i / 2, 0, frame->width);

	unclipped = (int)form->draw_pos.y.i - (int)new_size.y.i / 2;
	form->top = ft_iclamp(unclipped, 0, frame->height);
	form->norm_offset.y.f = ((int)form->top - unclipped) * form->delta.y.f;		// Clipped start
	form->bottom = ft_iclamp((int)form->draw_pos.y.i + (int)new_size.y.i / 2, 0, frame->height);
	return (enemy_dist);
}

// Returns true if the enemy is within a NxN pixel grid on the center of the screen
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

// TODO: Merge bilinear scailing with cub draw relative, they're both the same function except
// in the way they save the result
bool	cub_draw_relative(t_mat32 frame, t_rayhit *rays, t_player *player, t_enemy *enemy)
{
	uint32_t	x;
	uint32_t	y;
	t_form		form;
	t_vec2		norm_pos;
	const float	enemy_dist = stt_init(&form, &frame, player, enemy);	// Could be dist
	uint32_t	*ptr;
	uint32_t	c;

	if (enemy_dist <= NEAR_RADIUS)
		return (false);
	x = form.left;
	norm_pos.x.f = form.norm_offset.x.f;
	while (x < form.right)
	{
		if (enemy_dist < rays[x].perp_dist)	// Do not draw if wall column is ahead of enemy
		{
			y = form.top;
			norm_pos.y.f = form.norm_offset.y.f;
			ptr = frame.ptr + frame.stride * x;
			while (y < form.bottom)
			{
				c = ft_bilerp_argb_t(&enemy->texture, norm_pos);	// Bilerp takes a normalized range to sample from
				if (c != 0xFF000000)		// TODO: Proper alpha blend
					ptr[y] = c;
				norm_pos.y.f += form.delta.y.f;
				y++;
			}
		}
		norm_pos.x.f += form.delta.x.f;
		x++;
	}
	return (stt_hitreg(&form));
}

// TODO: Assumption used to be scale to fit, review algorithm accordingly and add pos draw
void	ft_bilinear_scaling(const t_mat32 *src, t_mat32 *dst, t_vec2 scale, t_vec2 draw_pos)
{
	t_form		form;
	t_vec2		new_size;
	int32_t		unclipped_left;
	int32_t		unclipped_top;
	uint32_t	x;
	uint32_t	y;
	t_vec2		norm_pos;

	new_size.x.i = src->width * scale.x.f;
	new_size.y.i = src->height * scale.y.f;
	form.delta.x.f = 1.0 / new_size.x.i;
	form.delta.y.f = 1.0 / new_size.y.i;
	unclipped_left = (int)draw_pos.x.i;
	unclipped_top = (int)draw_pos.y.i;
	form.left = ft_iclamp(unclipped_left, 0, dst->width);
	form.top = ft_iclamp(unclipped_top, 0, dst->height);
	form.right = ft_iclamp((int)draw_pos.x.i + (int)new_size.x.i, 0, dst->width);
	form.bottom = ft_iclamp((int)draw_pos.y.i + (int)new_size.y.i, 0, dst->height);
	form.norm_offset.x.f = ((int)form.left - unclipped_left) * form.delta.x.f;		// Clipped start
	form.norm_offset.y.f = ((int)form.top - unclipped_top) * form.delta.y.f;		// Clipped start
	x = form.left;
	norm_pos.x.f = form.norm_offset.x.f;
	while (x < form.right)
	{
		y = form.top;
		norm_pos.y.f = form.norm_offset.y.f;
		while (y < form.bottom)
		{
			dst->ptr[y * dst->stride + x] = ft_bilerp_argb(src, norm_pos);	// Bilerp takes a normalized range to sample from
			norm_pos.y.f += form.delta.y.f;
			y++;
		}
		norm_pos.x.f += form.delta.x.f;
		x++;
	}
}

void	ft_bilinear_scaling_t(const t_mat32 *src, t_mat32 *dst)
{
	t_form		form;
	t_vec2		new_size;
	int32_t		unclipped_left;
	int32_t		unclipped_top;
	uint32_t	x;
	uint32_t	y;
	t_vec2		norm_pos;

	new_size.x.i = dst->width;
	new_size.y.i = dst->height;
	form.delta.x.f = 1.0 / new_size.x.i;
	form.delta.y.f = 1.0 / new_size.y.i;
	unclipped_left = (int)0;
	unclipped_top = (int)0;
	form.left = ft_iclamp(unclipped_left, 0, dst->width);
	form.top = ft_iclamp(unclipped_top, 0, dst->height);
	form.right = ft_iclamp((int)0 + (int)new_size.x.i, 0, dst->width);
	form.bottom = ft_iclamp((int)0 + (int)new_size.y.i, 0, dst->height);
	form.norm_offset.x.f = ((int)form.left - unclipped_left) * form.delta.x.f;		// Clipped start
	form.norm_offset.y.f = ((int)form.top - unclipped_top) * form.delta.y.f;		// Clipped start
	x = form.left;
	norm_pos.x.f = form.norm_offset.x.f;
	while (x < form.right)
	{
		y = form.top;
		norm_pos.y.f = form.norm_offset.y.f;
		while (y < form.bottom)
		{
			dst->ptr[y * dst->stride + x] = ft_bilerp_argb_t(src, norm_pos);	// Bilerp takes a normalized range to sample from
			norm_pos.y.f += form.delta.y.f;
			y++;
		}
		norm_pos.x.f += form.delta.x.f;
		x++;
	}
}