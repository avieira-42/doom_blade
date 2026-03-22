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

typedef struct s_transform
{
	t_vec2		draw_pos;
	t_vec2		delta;
	t_vec2		norm_offset;	// Clip start to normalized range
	float		enemy_dist;
	uint32_t	top;
	uint32_t	bottom;
	uint32_t	left;
	uint32_t	right;
}	t_form;

static inline
uint32_t	stt_lerp_argb(uint32_t p0, uint32_t p1, uint8_t alpha)
{
	uint32_t	rb;
	uint32_t	ga;

	rb = p0 & 0x00FF00FF;									// Masks red and blue channel
	ga = (p0 >> 8) & 0x00FF00FF;							// Shifts green and alpha channel then masks
	rb += (((p1 & 0x00FF00FF) - rb) * alpha) >> 8;			// 
	ga += ((((p1 >> 8) & 0x00FF00FF) - ga) * alpha) >> 8;
	return (rb & 0x00FF00FF) | ((ga & 0x00FF00FF) << 8);
}

static inline
uint32_t	stt_bilerp_argb(t_vec4 sample, float uu, float vv)
{
	const uint8_t	u = uu * 255;
	const uint8_t	v = vv * 255;
	uint32_t		row1;
	uint32_t		row2;
	uint32_t		result;

	row1 = stt_lerp_argb(sample.x.u, sample.y.u, u);
	row2 = stt_lerp_argb(sample.z.u, sample.w.u, u);
	result = stt_lerp_argb(row1, row2, v);
	return (result);
}

static inline
uint32_t	stt_bilerp(const t_mat32 *src, t_vec2 norm_pos)
{
	t_vec2		src_pos;
	float		u;
	float		v;
	t_vec4		index;
	t_vec4		sample;

	src_pos.x.f = norm_pos.x.f * (src->cols - 1);
	src_pos.y.f = norm_pos.y.f * (src->rows - 1);
	index.x.u = src_pos.y.f;
	index.y.u = src_pos.y.f + (index.x.u < (src->rows - 1));
	index.z.u = src_pos.x.f;
	index.w.u = src_pos.x.f + (index.z.u < (src->cols - 1));
	sample.x.u = src->ptr[index.x.u * src->cols + index.z.u];
	sample.y.u = src->ptr[index.x.u * src->cols + index.w.u];
	sample.z.u = src->ptr[index.y.u * src->cols + index.z.u];
	sample.w.u = src->ptr[index.y.u * src->cols + index.w.u];
	u = src_pos.x.f - (float) index.z.u;							// Normalizes to 0-1, u coordinate
	v = src_pos.y.f - (float) index.x.u;							// Normalizes to 0-1, v coordinate
	return (stt_bilerp_argb(sample, u, v));
}

static inline
int	stt_dist(t_form *form, t_frame *frame, t_view player, t_vec2 enemy_pos)
{
	float		enemy_dist;
	float		horz_dist;
	t_vec2		rel_pos;
	const float	inv_det = 1.0f / (player.plane.x.f * player.dir.y.f - player.dir.x.f * player.plane.y.f);

	rel_pos.x.f = enemy_pos.x.f - player.pos.x.f;
	rel_pos.y.f = enemy_pos.y.f - player.pos.y.f;
	horz_dist = inv_det * (player.dir.y.f * rel_pos.x.f - player.dir.x.f * rel_pos.y.f);
	enemy_dist = inv_det * (-player.plane.y.f * rel_pos.x.f + player.plane.x.f * rel_pos.y.f);

	form->enemy_dist = enemy_dist;
	if (enemy_dist < 0.001f || horz_dist < 0.001f)
		return (-1);
	form->draw_pos.x.u = (frame->display.cols * 0.5f) * (1.0f + horz_dist / enemy_dist);
	form->draw_pos.y.u = frame->display.rows * 0.5f;
	return (0);
}

static inline
t_form	stt_init(t_frame *frame, t_entity *player, t_entity *enemy)
{
	t_form			form;
	t_vec2			new_size;
	int32_t			unclipped;
	float			scale;
	
	if (stt_dist(&form, frame, player->cam, enemy->cam.pos))
		return (form);
	scale = 1.0 / form.enemy_dist;
	new_size.x.u = scale * enemy->texture.cols;
	new_size.y.u = scale * enemy->texture.rows;
	form.delta.x.f = 1.0 / new_size.x.u;
	form.delta.y.f = 1.0 / new_size.y.u;

	unclipped = (int)form.draw_pos.x.u - (int)new_size.x.u / 2;
	form.left = ft_imax(0, unclipped);
	form.norm_offset.x.f = ((int)form.left - unclipped) * form.delta.x.f;	// Clipped start
	form.right = ft_imin(frame->display.cols, (int)form.draw_pos.x.u + (int)new_size.x.u / 2);

	unclipped = (int)form.draw_pos.y.u - (int)new_size.y.u / 2;
	form.top = ft_imax(0, unclipped);
	form.norm_offset.y.f = ((int)form.top - unclipped) * form.delta.y.f;	// Clipped start
	form.bottom = ft_imin(frame->display.rows, (int)form.draw_pos.y.u + (int)new_size.y.u / 2);
	return (form);
}

void	cub_draw_relative(t_frame *frame, t_entity *player, t_entity *enemy)
{
	uint32_t	x;
	uint32_t	y;
	t_form		form;
	t_vec2		norm_pos;

	form = stt_init(frame, player, enemy);	// TODO: Figure out draw pos of enemy
	if (form.enemy_dist < 0.001f)
		return ;
	x = form.left;
	norm_pos.x.f = form.norm_offset.x.f;
	while (x < form.right)
	{
		if (form.enemy_dist < frame->zbuffer[x / 2])	// Do not draw if wall column is ahead of enemy
		{
			y = form.top;
			norm_pos.y.f = form.norm_offset.y.f;
			while (y < form.bottom)
			{
				frame->display.ptr[y * frame->display.cols + x] = stt_bilerp(&enemy->texture, norm_pos);	// Bilerp takes a normalized range to sample from
				norm_pos.y.f += form.delta.y.f;
				y++;
			}
		}
		norm_pos.x.f += form.delta.x.f;
		x++;
	}
}
