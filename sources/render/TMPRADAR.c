#include "cub_structs.h"
#include "cub_utils.h"

static
void    stt_pixel_swap(t_mat32 frame, int32_t x, int32_t y, uint32_t color)
{
    uint32_t    *dst;

    if (x < 0 || x >= frame.width || y < 0 || y >= frame.height)
        return ;
    dst = frame.ptr + x * frame.stride + y;
	printf("*dst: %i\n", *dst);
    if (color == 0x000000)
    {
        if (*dst == 2693401)
            color = 0xd5213f;
        else if (*dst == 2299157)
            color = 0xd72a55;
    }
    else if (color == 0xFFFFFF)
    {
        if (*dst == 2693401)
            color = 0xe6c84b;
    }
    *dst = color;
}

void    stt_quad_draw(t_game *game, t_quad q)
{
    t_vec2  limit;
    int32_t y;
    int32_t x;

    limit.x.i = q.pos.x.i + q.size.x.i;
    limit.y.i = q.pos.y.i + q.size.y.i;
    y = q.pos.y.i;
    while (y <= limit.y.i)
    {
        x = q.pos.x.i;
        while (x <= limit.x.i)
        {
            if (vec2_idist(q.map_center, (t_vec2){.x.i = x, .y.i = y})
                <= q.bound * q.bound)
                stt_pixel_swap(game->frame.render, x, y, q.color);
            x++;
        }
        y++;
    }
}

static
void	stt_grid_render(t_game *game, t_grid grid, int32_t x, int32_t y)
{
	t_quad  q;

	grid.draw_pos.x.i = x - game->player.cam.pos.x.f + 6;
	grid.draw_pos.y.i = y - game->player.cam.pos.y.f + 6;
	grid.map_pos.x.i = grid.pos.x.i + grid.draw_pos.x.i * grid.size.x.i + 8;
	grid.map_pos.y.i = grid.pos.y.i + grid.draw_pos.y.i * grid.size.y.i + 8;
	q.pos = grid.map_pos;
	q.size = grid.size;
	q.bound = grid.bound;
	q.map_center = grid.map_center;
	if (game->map.tex_index[x + game->map.width * y] == 0)
	{
		q.color = 0x000000;
		stt_quad_draw(game, q);
	}
	if (game->map.tex_index[x + game->map.width * y] == 130)
	{
		q.color = 0xFFFFFF;
		stt_quad_draw(game, q);
	}
	if (game->map.tex_index[x + game->map.width * y] == 2)
	{
		q.color = 0xFFFFFF;
		stt_quad_draw(game, q);
	}
}

	static
void    stt_blocks_render(t_game *game, t_vec2 pos,
		int32_t bound, t_vec2 map_center)
{
	int32_t y;
	int32_t x;
	t_grid	grid;

	grid.map_center = map_center;
	grid.bound = bound;
	grid.pos = pos;
	grid.size.x.i = game->assets.radar_l0.texture.width / 12;
	grid.size.y.i = game->assets.radar_l0.texture.height / 12;
	y = game->player.cam.pos.y.f - 6.f;
	while (y <= game->player.cam.pos.y.f + 6.f)
	{
		x = game->player.cam.pos.x.f - 6.f;
		while (x <= game->player.cam.pos.x.f + 6.f)
		{
			if (x >= 0 && x < (int32_t)game->map.width
					&& y >= 0 && y < (int32_t)game->map.height)
				stt_grid_render(game, grid, x, y);
			x++;
		}
		y++;
	}
}

	static
void    stt_draw_player_circle(t_game *game, t_vec2 p_pos,
		t_vec2 map_center, int bound)
{
	int32_t radius;
	int32_t dy;
	int32_t dx;
	int32_t px;
	int32_t py;

	radius = 2;
	dy = -radius;
	while (dy <= radius)
	{
		dx = -radius;
		while (dx <= radius)
		{
			if (dx * dx + dy * dy <= radius * radius)
			{
				px = p_pos.x.i + dx;
				py = p_pos.y.i + dy;
				if (vec2_idist(map_center,
							(t_vec2){.x.i = px, .y.i = py})
						<= bound * bound)
					frame_pixel_put(game->frame.render, px, py, 0x440088);
			}
			dx++;
		}
		dy++;
	}
}

	static
void	stt_draw_layer(t_mat32 frame, t_sheet layer, t_vec2 first_pixel_pos)
{
	layer.texture.ptr += layer.index * layer.frame_size;
	cub_draw_texture(frame, layer.texture, first_pixel_pos.x.i,
			first_pixel_pos.y.i);
}

	static
void    stt_map_render(t_game *game)
{
	t_vec2  first_pixel_pos;
	t_vec2  last_pixel_pos;
	t_vec2  bound;
	t_vec2  map_center;
	t_vec2  p_pos;

	first_pixel_pos.x.i = 110;
	first_pixel_pos.y.i = 240;
	stt_draw_layer(game->frame.render,
			game->drawbuf.radar_l0, first_pixel_pos);
	last_pixel_pos.x.i = first_pixel_pos.x.i + game->assets.radar_l0.texture.width;
	last_pixel_pos.y.i = first_pixel_pos.y.i + game->assets.radar_l0.texture.height;
	bound.x.i = (last_pixel_pos.x.i - first_pixel_pos.x.i) / 2;
	bound.y.i = (last_pixel_pos.y.i - first_pixel_pos.y.i) / 2;
	map_center.x.i = first_pixel_pos.x.i + bound.x.i;
	map_center.y.i = first_pixel_pos.y.i + bound.y.i;
	stt_blocks_render(game, first_pixel_pos, bound.x.i, map_center);
	p_pos.x.i = map_center.x.i - game->assets.radar_l0.texture.width / 24 + 4;
	p_pos.y.i = map_center.y.i - game->assets.radar_l0.texture.height / 24 + 4;
	stt_draw_player_circle(game, p_pos, map_center, bound.x.i);
	stt_draw_layer(game->frame.render,
			game->drawbuf.radar_l1, first_pixel_pos);
}

void    animate_hud(t_game *game)
{
	input_handler(game);
	if (game->player.map & st_checking)
		stt_map_render(game);
	if (game->player.map & st_raising
			&& !(game->player.map & st_checking))
	{
		printf("hello\n");
		game->player.map |= st_checking;
	}
}
