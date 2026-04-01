#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

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

static inline
void	stt_texture_sample(t_mat32 texture, double line_height, uint32_t *render_col, int32_t draw_start, int32_t draw_end, int32_t unclamped_start)
{
	int32_t			y;
	float			tex_pos;
	const float		dy = (double)texture.height / line_height;

	tex_pos = dy * (draw_start - unclamped_start);
	y = draw_start;
	while (y < draw_end)
	{
		render_col[y] = texture.ptr[(size_t)tex_pos];
		tex_pos += dy;
		y++;
	}
}

static inline
void	stt_column_render(t_rayhit hit, uint32_t *render_col, t_block *blocks, float offset)
{
	t_mat32	texture;
	double	line_height;
	double	unclamped_start;
	double	unclamped_end;
	int32_t	draw_start;
	int32_t	draw_end;

	texture = blocks[hit.tex_index].index[hit.tex_dir];		// THIS NEEDS TO CHECK IF TEX_INDEX > 0
	texture.ptr += (size_t)(hit.x_pos * texture.width) * texture.stride;	// REVIEW
	line_height = fmax(1.0, (double) RENDER_HEIGHT / hit.perp_dist);
	unclamped_start = 0.5 * ((double)RENDER_HEIGHT - line_height) - offset;
	unclamped_end = 0.5 * ((double)RENDER_HEIGHT + line_height) - offset;
	//draw_start = 0.5 * ((double)RENDER_HEIGHT - line_height);
	//draw_end = 0.5 * ((double)RENDER_HEIGHT + line_height);
	draw_start = ft_imax(0, unclamped_start);
	draw_end = ft_imin(RENDER_HEIGHT, unclamped_end);
	// draw_start = ft_imax(0, (RENDER_HEIGHT / 2) - (line_height / 2));
	// draw_end = ft_imin(RENDER_HEIGHT, (RENDER_HEIGHT / 2) + (line_height / 2));
	stt_texture_sample(texture, line_height, render_col, draw_start, draw_end, (int32_t)unclamped_start);
}

// Blocks contains transposed rows for sequential memory access
// Everything is done in cols by rows, and then tranposed for the rendering
void	render_image(t_game *game)
{
	size_t		x;
	uint32_t	*ptr;

	x = 0;
	ptr = game->frame.render.ptr;
	planecast(game->frame, game->blocks[0].south, game->blocks[0].north, game->player.cam);
	raycast(&game->player.cam, &game->map, game->frame.rays);
	// stt_filter(frame->rays);
	while (x < RENDER_WIDTH)
	{
		stt_column_render(game->frame.rays[x], ptr, game->blocks, game->frame.offset);
		ptr += game->frame.render.stride;
		x++;
	}
}
