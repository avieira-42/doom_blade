#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

typedef struct s_render
{
	uint32_t	line_height;
	uint32_t	slice_index;
}	t_render;

// See if FLTO inlines
// Convert textures to power of two
static inline
void	stt_texture_lerp(t_rayhit *hit, uint32_t *render_col, int32_t draw_start, int32_t draw_end)
{
	int32_t			x;
	float			tex_pos;
	const float		dx = (double)hit->texture.cols / (double)hit->line_height;	// cols represents height here because the texture is transposed

	tex_pos = (draw_start - (RENDER_HEIGHT / 2) + (hit->line_height / 2)) * dx;
	x = draw_start;
	while (x < draw_end)
	{
		render_col[x] = hit->texture.ptr[(size_t)tex_pos];
		tex_pos += dx;
		x++;
	}
}

static inline
void	stt_column_render(t_rayhit *hit, uint32_t *render_col, t_block *blocks)
{
	int32_t	draw_start;
	int32_t	draw_end;

	hit->line_height = (float) RENDER_HEIGHT / hit->perp_dist;
	draw_start = ft_imax(0, (RENDER_HEIGHT / 2) - (hit->line_height / 2));
	draw_end = ft_imin(RENDER_HEIGHT, (RENDER_HEIGHT / 2) + (hit->line_height / 2));
	hit->texture.ptr += (size_t)(hit->x_pos_texture * hit->texture.rows) * hit->texture.cols;
	stt_texture_lerp(hit, render_col, draw_start, draw_end);
	hit->texture = blocks[0].north;
	stt_texture_lerp(hit, render_col, 0, draw_start);
	hit->texture = blocks[0].south;
	stt_texture_lerp(hit, render_col, draw_end, RENDER_HEIGHT);
}

// Blocks contains transposed rows for sequential memory access
// Everything is done in cols by rows, and then tranposed for the rendering
void	render_image(t_view *cam, t_mat8 *map, t_block *blocks, t_mat32 render_frame)
{
	size_t		x;
	t_rayhit	hit;
	float		camera_x;
	const float	dx = 2.0 / (double) RENDER_WIDTH;

	x = 0;
	camera_x = -1.0f;
	while (x < RENDER_WIDTH)
	{
		hit = raycast(camera_x, cam, map, blocks);	// if block_index is a specific number, do not render
		stt_column_render(&hit, render_frame.ptr, blocks);
		render_frame.ptr += render_frame.cols;
		camera_x += dx;
		x++;
	}
}
