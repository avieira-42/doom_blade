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
void	stt_draw_layer(t_mat32 frame, t_sheet *layer,
		t_vec2 first_pixel_pos, long dt)
{
	t_mat32	texture;

	texture = layer->texture;
	texture.ptr += layer->index * layer->frame_size;
	cub_draw_texture(frame, texture, first_pixel_pos.x.i,
		first_pixel_pos.y.i);
	cub_advance_animation(layer, dt);
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

// to place on utils instead of the other quad draw
static
void	stt_quad_draw(t_mat32 frame, t_vec2 pos, const t_vec2 quad_size, int32_t color)
{
	int32_t			y;
	int32_t			x;

	y = 0;
	while (y < quad_size.y.i)
	{
		x = 0;
		while(x < quad_size.x.i)
		{
			pixel_put(frame, pos.x.i + x, pos.y.i + y, color);
			x++;
		}
		y++;
	}
}

// this function is working properly, but it needs a review since if the grid is too tiny
// the result of integer division might make it unrenderable
void	stt_grid_draw(t_mat32 frame, t_map map, t_vec2 grid_pos, t_vec2 grid_size)
{
	size_t			y;
	size_t			x;
	uint32_t		block;
	t_vec2			quad_pos;
	const t_vec2	quad_size = (t_vec2){.x.i = grid_size.x.i / map.width,
		.y.i = grid_size.y.i / map.height};

	y = 0;
	while (y < map.height)
	{
		x = 0;
		while(x < map.width)
		{
			block = map.tex_index[x + y * map.width];
			if (block == 129)
			{
				quad_pos = (t_vec2){.x.i = grid_pos.x.i + quad_size.x.i * x,
					.y.i = grid_pos.y.i + quad_size.y.i * y};
				stt_quad_draw(frame, quad_pos, quad_size, 0xFF00FF);
			}
			x++;
		}
		y++;
	}
}

static
void	stt_player_icon_draw(t_mat32 frame, t_player player, t_vec2 grid_pos, t_vec2 quad_size)
{
}

static
void	stt_draw_radar(t_game *game, long dt)
{
	t_vec2	last_pixel_pos;
	t_vec2	bound;
	t_vec2	map_center;
	t_vec2	p_pos;


	// to rework | attempting to draw a pseudo so long but just in a tiny area
	// what i need is to keep pushing the position of the grid and the grid will
	// only be visible within a certain distance from the center of the sprite NOT
	// the player position

	// DRAW GRID // WORKING PROPERLY FOR NOW
	const t_vec2	radar_pos = (t_vec2){.x.i = 110, .y.i = 240}; // also grid_pos
	const t_vec2	grid_size =
		(t_vec2){.x.i = game->player.hands.radar_l0.texture.width * 2,
			.y.i = game->player.hands.radar_l0.texture.height * 2};
	const t_vec2	quad_size = (t_vec2){.x.i = grid_size.x.i / game->map.width,
		.y.i = grid_size.y.i / game->map.height};
	t_vec2	player_pos;
	t_vec2			grid_pos;
	t_vec2			icon_size;
	t_vec2			icon_pos;

	player_pos = (t_vec2){.x.i =  game->player.cam.pos.x.f * quad_size.x.i
		, .y.i = game->player.cam.pos.y.f * quad_size.y.i};
	grid_pos = (t_vec2){.x.i =
		player_pos.x.i - game->player.hands.radar_l0.texture.width / 2,
		.y.i = player_pos.y.i - game->player.hands.radar_l0.texture.height / 2};
	icon_size = (t_vec2){.x.i = quad_size.x.i - 2,
		.y.i = quad_size.y.i - 2};
	icon_size.x.i = ft_imax(icon_size.x.i, 1);
	icon_size.y.i = ft_imax(icon_size.y.i, 1);
	grid_pos.x.i = ft_imax(grid_pos.x.i, 0);
	grid_pos.x.i = ft_imin(grid_pos.x.i, grid_size.x.i
			- game->player.hands.radar_l0.texture.width);
	grid_pos.y.i = ft_imax(grid_pos.y.i, 0);
	grid_pos.y.i = ft_imin(grid_pos.y.i, grid_size.y.i
			- game->player.hands.radar_l0.texture.height);
	player_pos = (t_vec2){.x.i = radar_pos.x.i
		+ player_pos.x.i - grid_pos.x.i,
			.y.i = radar_pos.y.i + player_pos.y.i - grid_pos.y.i};
	stt_draw_layer(game->frame.render,
		&game->player.hands.radar_l0, radar_pos, dt);

	stt_grid_draw(game->frame.render, game->map,
			(t_vec2){.x.i = radar_pos.x.i - grid_pos.x.i,
			.y.i = radar_pos.y.i - grid_pos.y.i}, grid_size);
	
	stt_quad_draw(game->frame.render, player_pos, icon_size, 0x00FF00);

	stt_draw_layer(game->frame.render,
		&game->player.hands.radar_l1, radar_pos, dt);

	//stt_player_icon_draw(game->frame.render, game->player, radar_pos, quad_size);
	/*last_pixel_pos.x.i = first_pixel_pos.x.i
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
	stt_draw_player_circle(game, p_pos, map_center, bound.x.i);*/

}

	// // RADAR TMP >>>
	// if (player->map & st_raising)
	// 	cub_advance_animation(&drawbuf->radar, dt);
	// cub_advance_animation(&drawbuf->radar_l0, dt);
	// cub_advance_animation(&drawbuf->radar_l1, dt);
	// // <<< RADAR TMP

void	cub_draw_radar(t_game *game, t_mat32 render, t_hands *hands, long dt)
{
	t_mat32	texture;

	if (game->player.map & st_checking)
	{
		hands->radar.index = 0;
		texture = hands->radar.texture;
		texture.ptr = hands->radar.texture.ptr
			+ hands->radar.frame_size * (hands->radar.count -1);
		cub_draw_texture(render, texture, 0, 195);
		stt_draw_radar(game, dt);
	}
	else if (game->player.map & st_raising)
	{
		texture = hands->radar.texture;
		texture.ptr += hands->radar.index
			* hands->radar.frame_size;
		cub_draw_texture(render, texture, 0, 195);
		if (hands->radar.index == hands->radar.count - 2)
		{
			game->player.map &= ~(size_t)st_raising;
			game->player.map |= (size_t)st_checking;
		}
		cub_advance_animation(&hands->radar, dt);
	}
}
