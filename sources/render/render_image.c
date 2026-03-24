#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

// See if FLTO inlines
// Convert textures to power of two
static inline
void	stt_texture_lerp(t_mat32 texture, int32_t line_height, uint32_t *render_col, int32_t draw_start, int32_t draw_end)
{
	int32_t			y;
	float			tex_pos;
	const float		dy = (double)texture.height / (double)line_height;

	tex_pos = dy * (draw_start - (RENDER_HEIGHT / 2) + (line_height / 2));
	y = draw_start;
	while (y < draw_end)
	{
		render_col[y] = texture.ptr[(size_t)tex_pos];
		tex_pos += dy;
		y++;
	}
}

static inline
void	stt_draw_tmp(uint32_t *render_col, int32_t draw_start, int32_t draw_end)
{
	size_t	x;

	x = 0;
	while (x < draw_start)
	{
		render_col[x] = rgb_gray;
		x++;
	}
	x = draw_end;
	while (x < RENDER_HEIGHT)
	{
		render_col[x] = rgb_brown;
		x++;
	}
}

static inline
void	stt_column_render(t_rayhit hit, uint32_t *render_col, t_block *blocks)
{
	t_mat32	texture;
	int32_t	line_height;
	int32_t	draw_start;
	int32_t	draw_end;

	texture = blocks[hit.tex_index].index[hit.tex_dir];
	texture.ptr += (size_t)(hit.tex_offset) * texture.stride;
	line_height = (float) RENDER_HEIGHT / hit.perp_dist;
	draw_start = ft_imax(0, (RENDER_HEIGHT / 2) - (line_height / 2));
	draw_end = ft_imin(RENDER_HEIGHT, (RENDER_HEIGHT / 2) + (line_height / 2));
	stt_texture_lerp(texture, line_height, render_col, draw_start, draw_end);
	stt_draw_tmp(render_col, draw_start, draw_end);	// TODO: floor and ceiling texture mapping
}

// Aliasing is caused by differing line heights. This will reduce the effect
static inline
void	stt_filter(t_rayhit *rays)
{
	size_t	x;

	x = 0;
	while (x < RENDER_WIDTH - 1)
	{
		float next = rays[x + 1].perp_dist;
		float cur = rays[x].perp_dist;
		if (fabsf(next) - fabsf(cur) < 0.25f)
			next = cur;
		x++;
	}
}

// Blocks contains transposed rows for sequential memory access
// Everything is done in cols by rows, and then tranposed for the rendering
void	render_image(t_view *cam, t_mat8 *map, t_block *blocks, t_frame *frame)
{
	size_t		x;
	uint32_t	*ptr;

	x = 0;
	ptr = frame->render.ptr;
	raycast(cam, map, frame);
	// stt_filter(frame->rays);
	while (x < RENDER_WIDTH)
	{
		stt_column_render(frame->rays[x], ptr, blocks);
		ptr += frame->render.stride;
		x++;
	}
}
