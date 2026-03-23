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

// 1) How will you determine clipping?
// 2) How will you determine where to draw? 
// One alternative is to choose a center position, scale the rectangle, and just clip to screen

#define NEAR_RADIUS 0.1f

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
float	stt_init(t_form *form, t_mat32 *frame, t_entity *player, t_entity *enemy)
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

// TODO: Merge bilinear scailing with cub draw relative, they're both the same function except
// in the way they save the result. 
void	cub_draw_relative(t_mat32 frame, float *zbuffer, t_entity *player, t_entity *enemy)
{
	uint32_t	x;
	uint32_t	y;
	t_form		form;
	t_vec2		norm_pos;
	const float	enemy_dist = stt_init(&form, &frame, player, enemy);	// Could be dist
	uint32_t	*ptr;
	uint32_t	c;

	if (enemy_dist <= NEAR_RADIUS)
		return ;
	x = form.left;
	norm_pos.x.f = form.norm_offset.x.f;
	while (x < form.right)
	{
		if (enemy_dist < zbuffer[x])	// Do not draw if wall column is ahead of enemy
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
}
