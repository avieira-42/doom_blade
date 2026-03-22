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

#define NEAR_RADIUS 0.1f

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

	src_pos.x.f = norm_pos.x.f * (src->width - 1);
	src_pos.y.f = norm_pos.y.f * (src->height - 1);
	index.x.u = src_pos.y.f;
	index.y.u = src_pos.y.f + (index.x.u < (src->height - 1));
	index.z.u = src_pos.x.f;
	index.w.u = src_pos.x.f + (index.z.u < (src->width - 1));
	sample.x.u = src->ptr[index.x.u * src->stride + index.z.u];
	sample.y.u = src->ptr[index.x.u * src->stride + index.w.u];
	sample.z.u = src->ptr[index.y.u * src->stride + index.z.u];
	sample.w.u = src->ptr[index.y.u * src->stride + index.w.u];
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
	if (enemy_dist < NEAR_RADIUS)
		return (-1);
	form->draw_pos.x.i = (frame->display.width * 0.5f) * (1.0f + horz_dist / enemy_dist);
	form->draw_pos.y.i = frame->display.height * 0.5f;
	return (0);
}

static inline
uint32_t	stt_clamp(int32_t value, int32_t min, int32_t max)
{
	if (value > max)
		value = max;
	else if (value < min)
		value = min;
	return (value);
}

static inline
int	stt_init(t_form *form, t_frame *frame, t_entity *player, t_entity *enemy)
{
	t_vec2			new_size;
	int32_t			unclipped;
	float			scale;
	
	if (stt_dist(form, frame, player->cam, enemy->cam.pos))
		return (-1);
	scale = 1.0 / form->enemy_dist;
	new_size.x.i = scale * enemy->texture.width;
	new_size.y.i = scale * enemy->texture.height;
	form->delta.x.f = 1.0 / new_size.x.i;
	form->delta.y.f = 1.0 / new_size.y.i;

	unclipped = (int)form->draw_pos.x.i - (int)new_size.x.i / 2;
	form->left = stt_clamp(unclipped, 0, frame->display.width);
	form->norm_offset.x.f = ((int)form->left - unclipped) * form->delta.x.f;	// Clipped start
	form->right = stt_clamp((int)form->draw_pos.x.i + (int)new_size.x.i / 2, 0, frame->display.width);

	unclipped = (int)form->draw_pos.y.i - (int)new_size.y.i / 2;
	form->top = stt_clamp(unclipped, 0, frame->display.height);
	form->norm_offset.y.f = ((int)form->top - unclipped) * form->delta.y.f;		// Clipped start
	form->bottom = stt_clamp((int)form->draw_pos.y.i + (int)new_size.y.i / 2, 0, frame->display.height);
	return (0);
}

void	cub_draw_relative(t_frame *frame, t_entity *player, t_entity *enemy)
{
	uint32_t	x;
	uint32_t	y;
	t_form		form;
	t_vec2		norm_pos;

	if (stt_init(&form, frame, player, enemy))
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
				frame->display.ptr[y * frame->display.stride + x] = stt_bilerp(&enemy->texture, norm_pos);	// Bilerp takes a normalized range to sample from
				norm_pos.y.f += form.delta.y.f;
				y++;
			}
		}
		norm_pos.x.f += form.delta.x.f;
		x++;
	}
}
