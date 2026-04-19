#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "game_defines.h"
#include "game_types.h"
#include "game_prototypes.h"

// Aliasing is caused by differing line heights. This will reduce the effect
// static inline
// void	stt_filter(t_rayhit *rays)
// {
// 	size_t	x;
// 	float	next;
// 	float	cur;

// 	x = 0;
// 	while (x < r_width - 1)
// 	{
// 		next = rays[x + 1].perp_dist;
// 		cur = rays[x].perp_dist;
// 		if (fabsf(next) - fabsf(cur) < 0.25f)
// 			next = cur;
// 		x++;
// 	}
// }

static inline
void	stt_texture_sample(t_mat32 texture, float line_height, uint32_t *render_col, float unclamped_start, float unclamped_end)
{
	int32_t			y;
	int32_t			tex_pos;
	float			draw_start;
	float			draw_end;
	const int32_t	dy = 65536.0f * texture.height / line_height;

	draw_start = MAX(0, unclamped_start);
	draw_end = MIN(R_HEIGHT, unclamped_end);
	tex_pos = dy * (draw_start - unclamped_start);
	y = draw_start;
	while (y < draw_end)
	{
		render_col[y] = texture.ptr[tex_pos >> 16];
		tex_pos += dy;
		y++;
	}
}

// X is start, Y is end
static inline
void	stt_column_render(t_rayhit hit, uint32_t *render_col, t_block *blocks, int32_t offset)
{
	t_mat32	texture;
	float	line_height;
	float	unclamped_start;
	float	unclamped_end;

	texture = blocks[hit.tex_index].index[hit.tex_dir];	// THIS NEEDS TO CHECK IF TEX_INDEX > 0
	texture.ptr += (size_t)(hit.x_pos * texture.width) * texture.stride;	// REVIEW
	line_height = MAX(1.0f, R_HEIGHT / hit.perp_dist);
	unclamped_start = 0.5f * (R_HEIGHT - line_height) - offset;
	unclamped_end = 0.5f * (R_HEIGHT + line_height) - offset;
	stt_texture_sample(texture, line_height, render_col, unclamped_start, unclamped_end);
}

// Blocks contains transposed rows for sequential memory access
// Everything is done in cols by rows, and then tranposed for the rendering
void	cub_draw_world(t_game *game)
{
	size_t		x;
	uint32_t	*ptr;

	x = 0;
	ptr = game->frame.render.ptr;
	planecast(&game->frame, game->blocks[0].south, game->blocks[0].north, &game->player.cam);
	raycast(&game->player.cam, &game->map, game->frame.rays);
	while (x < R_WIDTH)
	{
		stt_column_render(game->frame.rays[x], ptr,
			game->blocks, game->frame.offset);
		ptr += game->frame.render.stride;
		x++;
	}
}
