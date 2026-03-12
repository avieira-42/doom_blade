#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

// See if FLTO inlines
// Convert textures to power of two
static
void	stt_column_render(t_ray_hit hit, uint32_t *render_col)
{
	size_t			x;
	const int32_t	line_height = (float) RENDER_HEIGHT / (float) hit.perp_dist;
	const uint32_t	draw_start = ft_imax(0, (RENDER_HEIGHT / 2) - (line_height / 2));
	const uint32_t	draw_end = ft_imin(RENDER_HEIGHT, (RENDER_HEIGHT / 2) + (line_height / 2));
	const float		dx = (double)hit.texture.cols / (double)line_height;	// cols represents height here because the texture is transposed
	float			tex_pos;

	hit.texture.ptr += (size_t)(hit.x_pos_texture * hit.texture.rows) * hit.texture.cols;
	tex_pos = (draw_start - (RENDER_HEIGHT / 2) + (line_height / 2)) * dx;
	x = draw_start;
	while (x < draw_end)
	{
		render_col[x] = hit.texture.ptr[(size_t)tex_pos];
		tex_pos += dx;
		x++;
	}
}

// Blocks contains transposed rows for sequential memory access
// Everything is done in cols by rows, and then tranposed for the rendering
void	render_image(t_view *cam, t_mat8 *map, t_block *blocks, t_mat32 render_frame)
{
	size_t		x;
	t_ray_hit	hit;
	float		camera_x;
	const float	dx = 2.0 / (double) RENDER_WIDTH;

	x = 0;
	camera_x = -1.0f;
	while (x < RENDER_WIDTH)
	{
		hit = raycast(camera_x, cam, map, blocks);	// if block_index is a specific number, do not render
		stt_column_render(hit, render_frame.ptr);
		render_frame.ptr += render_frame.cols;
		camera_x += dx;
		x++;
	}
}
