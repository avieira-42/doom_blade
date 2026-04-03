#include "cub_structs.h"
#include "cub_utils.h"

static
void	stt_draw_player_circle(t_game *game, t_vec2 p_pos,
		t_vec2 map_center, int bound)
{
	int32_t	radius;
	t_vec2	d;
	t_vec2	p;
	int32_t	dist;

	radius = 2;
	d.y.i = -radius;
	while (d.y.i <= radius)
	{
		d.x.i = -radius;
		while (d.x.i <= radius)
		{
			if (d.x.i * d.x.i + d.y.i * d.y.i <= radius * radius)
			{
				p.x.i = p_pos.x.i + d.x.i;
				p.y.i = p_pos.y.i + d.y.i;
				dist = vec2_idist(map_center,
						(t_vec2){.x.i = p.x.i, .y.i = p.y.i});
				if (dist <= bound * bound)
					pixel_put(game->frame.render, p.x.i, p.y.i, 0x440088);
			}
			d.x.i++;
		}
		d.y.i++;
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
void	stt_grid_render(t_game *game, t_grid grid, int32_t x, int32_t y)
{
	t_quad	q;

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
		quad_draw(game, q);
	}
	if (game->map.tex_index[x + game->map.width * y] == 130)
	{
		q.color = 0xFFFFFF;
		quad_draw(game, q);
	}
	if (game->map.tex_index[x + game->map.width * y] == 2)
	{
		q.color = 0xFFFFFF;
		quad_draw(game, q);
	}
}

static
void	stt_blocks_render(t_game *game, t_vec2 pos,
		int32_t bound, t_vec2 map_center)
{
	int32_t	y;
	int32_t	x;
	t_grid	grid;

	grid.map_center = map_center;
	grid.bound = bound;
	grid.pos = pos;
	grid.size.x.i = game->player.hands.radar_l0.texture.width / 12;
	grid.size.y.i = game->player.hands.radar_l0.texture.height / 12;
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
void	stt_draw_radar(t_game *game)
{
	t_vec2	first_pixel_pos;
	t_vec2	last_pixel_pos;
	t_vec2	bound;
	t_vec2	map_center;
	t_vec2	p_pos;

	first_pixel_pos.x.i = 110;
	first_pixel_pos.y.i = 240;
	stt_draw_layer(game->frame.render,
		game->player.hands.radar_l0, first_pixel_pos);
	last_pixel_pos.x.i = first_pixel_pos.x.i
		+ game->player.hands.radar_l0.texture.width;
	last_pixel_pos.y.i = first_pixel_pos.y.i
		+ game->player.hands.radar_l0.texture.height;
	bound.x.i = (last_pixel_pos.x.i - first_pixel_pos.x.i) / 2;
	bound.y.i = (last_pixel_pos.y.i - first_pixel_pos.y.i) / 2;
	map_center.x.i = first_pixel_pos.x.i + bound.x.i;
	map_center.y.i = first_pixel_pos.y.i + bound.y.i;
	stt_blocks_render(game, first_pixel_pos, bound.x.i, map_center);
	p_pos.x.i = map_center.x.i - game->player.hands.radar_l0.texture.width / 24 + 4;
	p_pos.y.i = map_center.y.i - game->player.hands.radar_l0.texture.height / 24 + 4;
	stt_draw_player_circle(game, p_pos, map_center, bound.x.i);
	stt_draw_layer(game->frame.render,
		game->player.hands.radar_l1, first_pixel_pos);
}

	// // RADAR TMP >>>
	// if (player->map & st_raising)
	// 	cub_advance_animation(&drawbuf->radar, dt);
	// cub_advance_animation(&drawbuf->radar_l0, dt);
	// cub_advance_animation(&drawbuf->radar_l1, dt);
	// // <<< RADAR TMP

void	cub_draw_radar(t_game *game, t_mat32 render, t_hands *hands)
{
	t_mat32	texture;

	if (game->player.map & st_checking)
	{
		hands->radar.index = 0;
		texture = hands->radar.texture;
		texture.ptr = hands->radar.texture.ptr
			+ hands->radar.frame_size * (hands->radar.count -1);
		cub_draw_texture(render, texture, 0, 195);
		draw_radar(game);
	}
	else if (game->player.map & st_raising)
	{
		texture = hands->radar.texture;
		texture.ptr += hands->radar.index
			* hands->radar.frame_size;
		cub_draw_texture(render, texture, 0, 195);
		if (hands->radar.index == hands->radar.count - 2)
			game->player.map |= st_checking;
	}
}
