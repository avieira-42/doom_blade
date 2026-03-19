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
void	stt_texture_lerp(t_rayhit *hit, uint32_t *render_col, int32_t draw_start, int32_t draw_end)
{
	int32_t			x;
	float			tex_pos;
	const float		dx = (double)hit->texture.cols / (double)hit->line_height;	// cols represents height here because the texture is transposed

	tex_pos = dx * (draw_start - (RENDER_HEIGHT / 2) + (hit->line_height / 2));
	x = draw_start;
	while (x < draw_end)
	{
		render_col[x] = hit->texture.ptr[(size_t)tex_pos];
		tex_pos += dx;
		x++;
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
void	stt_column_render(t_rayhit *hit, uint32_t *render_col, t_block *blocks)
{
	int32_t	draw_start;
	int32_t	draw_end;

	draw_start = ft_imax(0, (RENDER_HEIGHT / 2) - (hit->line_height / 2));
	draw_end = ft_imin(RENDER_HEIGHT, (RENDER_HEIGHT / 2) + (hit->line_height / 2));
	stt_texture_lerp(hit, render_col, draw_start, draw_end);
	stt_draw_tmp(render_col, draw_start, draw_end);	// TODO: floor and ceiling texture mapping
}

// Blocks contains transposed rows for sequential memory access
// Everything is done in cols by rows, and then tranposed for the rendering
void	render_image(t_view *cam, t_mat8 *map, t_block *blocks, t_frame *frame)
{
	size_t		x;
	t_rayhit	hit;
	float		camera_x;
	uint32_t	*ptr;
	const float	dx = 2.0 / (double) RENDER_WIDTH;

	x = 0;
	camera_x = -1.0f;
	ptr = frame->render.ptr;
	while (x < RENDER_WIDTH)
	{
		hit = raycast(camera_x, cam, map, blocks);	// if block_index is a specific number, do not render
		frame->zbuffer[x] = hit.perp_dist;
		stt_column_render(&hit, ptr, blocks);
		ptr += frame->render.cols;
		camera_x += dx;
		x++;
	}
}
