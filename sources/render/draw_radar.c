#include "cub_structs.h"
#include "cub_utils.h"

// to rework and reuse instead of the quad
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

// to place on utils instead of the other quad draw
// TODO: to remove one parameter (FUCK NORMINETTE REALLY) might remove the color....
static
void	stt_quad_draw(t_mat32 frame, t_vec2 pos,
		const t_vec2 quad_size, int32_t color, int32_t radius, t_vec2 sprite_center)
{
	int32_t	y;
	int32_t	x;
	t_vec2	dst;

	y = 0;
	while (y < quad_size.y.i)
	{
		x = 0;
		while(x < quad_size.x.i)
		{
			dst = (t_vec2){.x.i = pos.x.i + x, .y.i = pos.y.i + y};
			if (vec2_idist(sprite_center, dst) <= radius * radius)
				pixel_put(frame, dst.x.i, dst.y.i, color);
			x++;
		}
		y++;
	}
}

// this function is working properly, but it needs a review since if the grid is too tiny
// the result of integer division might make it unrenderable
void	stt_grid_draw(t_mat32 frame, t_map map, t_vec2 grid_pos, t_vec2 grid_size,
		int32_t radius, t_vec2 sprite_center)
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
				stt_quad_draw(frame, quad_pos, quad_size, 0xFF00FF, radius, sprite_center);
			}
			x++;
		}
		y++;
	}
}

// for funcitonality separation
static
void	stt_player_icon_draw(t_mat32 frame, t_player player, t_vec2 grid_pos, t_vec2 quad_size)
{
}

static
void	stt_draw_radar(t_game *game, long dt)
{
	// to rework | attempting to draw a pseudo so long but just in a tiny area
	// what i need is to keep pushing the position of the grid and the grid will
	// only be visible within a certain distance from the center of the sprite NOT
	// the player position
	// TODO for the icon: get center of player to draw from it, NOT the corner

	// DRAW GRID // WORKING PROPERLY FOR NOW
	const t_vec2	radar_pos = (t_vec2){.x.i = 110, .y.i = 240}; // also grid_pos
	const t_vec2	grid_size =
		(t_vec2){.x.i = game->player.hands.radar_l0.texture.width * 2,
			.y.i = game->player.hands.radar_l0.texture.height * 2};
	const t_vec2	quad_size = (t_vec2){.x.i = grid_size.x.i / game->map.width,
		.y.i = grid_size.y.i / game->map.height};
	const int32_t	radius = game->player.hands.radar_l0.texture.width / 2;
	t_vec2			player_pos;
	t_vec2			grid_pos;
	const t_vec2	icon_size = (t_vec2){.x.i = 2, .y.i = 2};

	player_pos = (t_vec2){.x.i =  game->player.cam.pos.x.f * quad_size.x.i
		, .y.i = game->player.cam.pos.y.f * quad_size.y.i};
	grid_pos = (t_vec2){.x.i =
		player_pos.x.i - game->player.hands.radar_l0.texture.width / 2,
		.y.i = player_pos.y.i - game->player.hands.radar_l0.texture.height / 2};
	grid_pos.x.i = ft_imax(grid_pos.x.i, 0);
	grid_pos.x.i = ft_imin(grid_pos.x.i, grid_size.x.i
			- game->player.hands.radar_l0.texture.width);
	grid_pos.y.i = ft_imax(grid_pos.y.i, 0);
	grid_pos.y.i = ft_imin(grid_pos.y.i, grid_size.y.i
			- game->player.hands.radar_l0.texture.height);
	player_pos = (t_vec2){.x.i = radar_pos.x.i
		+ player_pos.x.i - grid_pos.x.i - icon_size.x.i / 2,
			.y.i = radar_pos.y.i + player_pos.y.i - grid_pos.y.i - icon_size.y.i / 2};
	stt_draw_layer(game->frame.render,
		&game->player.hands.radar_l0, radar_pos, dt);

	const t_vec2 sprite_center = (t_vec2){
		.x.i = radar_pos.x.i + game->player.hands.radar_l0.texture.width / 2,
			.y.i = radar_pos.y.i + game->player.hands.radar_l0.texture.height / 2};

	stt_grid_draw(game->frame.render, game->map,
			(t_vec2){.x.i = radar_pos.x.i - grid_pos.x.i,
			.y.i = radar_pos.y.i - grid_pos.y.i}, grid_size, radius,
			sprite_center);
	
	stt_quad_draw(game->frame.render, player_pos, icon_size, 0x00FF00,
			radius, sprite_center);

	stt_draw_layer(game->frame.render,
		&game->player.hands.radar_l1, radar_pos, dt);
}

void	cub_draw_radar(t_game *game, t_mat32 render, t_hands *hands, long dt)
{
	t_mat32	texture;

	if (game->player.map & st_checking)	// This should be in actions. Pipeline is: actions determine what sheet to load;
	{									// The sheet is then used by draw hands to draw the texture
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
