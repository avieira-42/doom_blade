#include "cub_structs.h"
#include "cub_utils.h"

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

// the result of integer division might make it unrenderable
static
void	stt_grid_draw(t_mat32 frame, t_map map, t_vec2 grid_pos, t_quad cell)
{
	size_t			y;
	size_t			x;
	uint32_t		block;

	y = 0;
	while (y < map.height)
	{
		x = 0;
		while (x < map.width)
		{
			block = map.tex_index[x + y * map.width];
			if (block == 129)
			{
				cell.pos = (t_vec2){.x.i = grid_pos.x.i + cell.size.x.i * x,
					.y.i = grid_pos.y.i + cell.size.y.i * y};
				quad_draw(frame, cell);
			}
			x++;
		}
		y++;
	}
}

// for funcitonality separation
static
void	stt_player_icon_draw(t_mat32 frame, t_map map, t_vec2 grid_pos)
{
	map.icon_quad.pos = (t_vec2){.x.i = map.radar_sprite_pos.x.i
		+ map.icon_quad.pos.x.i - grid_pos.x.i - map.icon_quad.size.x.i / 2,
		.y.i = map.radar_sprite_pos.y.i + map.icon_quad.pos.y.i
		- grid_pos.y.i - map.icon_quad.size.y.i / 2};
	map.icon_quad.center = (t_vec2){.x.i = map.icon_quad.pos.x.i + map.icon_quad.radius,
		.y.i = map.icon_quad.pos.y.i + map.icon_quad.radius};
	printf("PLAYER:\nx: %i\ny: %i\n", map.icon_quad.pos.x.i, map.icon_quad.pos.y.i);
	quad_draw(frame, map.icon_quad);
}

static
void	stt_enemies_icon_draw(t_mat32 frame, t_map map, t_vec2 grid_pos, t_enemy *enemies)
{
	int32_t	i;

	i = 0;
	map.icon_quad.color = rgb_red;
	while (i < NUM_ENEMIES)
	{
		if (enemies[i].state & e_dead)
		{
			i++;
			continue;
		}
		printf("ENEMY_ORIGINAL:\nx: %f\ny: %f\n",enemies[i].cam.pos.x.f, enemies[i].cam.pos.y.f);
		map.icon_quad.pos = (t_vec2){.x.i = grid_pos.x.i
			+ enemies[i].cam.pos.x.f * map.radar_quad.size.x.i,
		.y.i = grid_pos.y.i + enemies[i].cam.pos.y.f * map.radar_quad.size.y.i};
		map.icon_quad.center = (t_vec2){.x.i = map.icon_quad.pos.x.i + map.icon_quad.radius,
			.y.i = map.icon_quad.pos.y.i + map.icon_quad.radius};
		//map.icon_quad.size = (t_vec2){.x.i = 30, .y.i = 30};
		printf("ENEMY %i:\nx: %i\ny: %i\n", i, map.icon_quad.pos.x.i, map.icon_quad.pos.y.i);
		/*map.icon_quad.pos = (t_vec2){.x.i = enemies[i].cam.pos.x.f
			* map.radar_quad.size.x.i,
				.y.i = enemies[i].cam.pos.y.f * map.radar_quad.size.y.i};
		map.icon_quad.pos = (t_vec2){.x.i = map.radar_sprite_pos.x.i
			+ map.icon_quad.pos.x.i - grid_pos.x.i - map.icon_quad.size.x.i / 2,
				.y.i = map.radar_sprite_pos.y.i + map.icon_quad.pos.y.i
					- grid_pos.y.i - map.icon_quad.size.y.i / 2};
		map.icon_quad.center = (t_vec2){.x.i = map.icon_quad.pos.x.i + map.icon_quad.radius,
			.y.i = map.icon_quad.pos.y.i + map.icon_quad.radius};
		map.icon_quad.size = (t_vec2){.x.i = 30, .y.i = 30};*/
		quad_draw(frame, map.icon_quad);
		i++;
	}
}

// TODO: draw enemies icons
// TODO: draw player foc representation
	static
void	stt_draw_radar(t_game *game, t_map map, long dt)
{
	t_vec2	grid_offset;
	t_vec2	g_pos;

	map.icon_quad.pos = (t_vec2){.x.i
		= game->player.cam.pos.x.f * map.radar_quad.size.x.i,
			.y.i = game->player.cam.pos.y.f * map.radar_quad.size.y.i};
	grid_offset = (t_vec2){.x.i = map.icon_quad.pos.x.i
		- game->player.hands.radar_l0.texture.width / 2,
			.y.i = map.icon_quad.pos.y.i
				- game->player.hands.radar_l0.texture.height / 2};
	grid_offset.x.i = ft_imax(grid_offset.x.i, 0);
	grid_offset.x.i = ft_imin(grid_offset.x.i, map.radar_size.x.i
			- game->player.hands.radar_l0.texture.width);
	grid_offset.y.i = ft_imax(grid_offset.y.i, 0);
	grid_offset.y.i = ft_imin(grid_offset.y.i, map.radar_size.y.i
			- game->player.hands.radar_l0.texture.height);
	g_pos = (t_vec2){.x.i = map.radar_sprite_pos.x.i - grid_offset.x.i,
			.y.i = map.radar_sprite_pos.y.i - grid_offset.y.i};
	stt_draw_layer(game->frame.render,
			&game->player.hands.radar_l0, map.radar_sprite_pos, dt);
	stt_grid_draw(game->frame.render, game->map, g_pos, game->map.radar_quad);
	stt_enemies_icon_draw(game->frame.render, map, g_pos, game->enemies);
	stt_player_icon_draw(game->frame.render, map, grid_offset);
	stt_draw_layer(game->frame.render,
			&game->player.hands.radar_l1, map.radar_sprite_pos, dt);
}

void	cub_draw_radar(t_game *game, t_mat32 render,
		t_hands *hands, long dt)
{
	t_mat32	texture;

	if (game->player.map & st_checking) // This should be in actions. Pipeline is: actions determine what sheet to load;
	{									// The sheet is then used by draw hands to draw the texture
		hands->radar.index = 0;
		texture = hands->radar.texture;
		texture.ptr = hands->radar.texture.ptr
			+ hands->radar.frame_size * (hands->radar.count -1);
		cub_draw_texture(render, texture, 0, 195);
		stt_draw_radar(game, game->map, dt);
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
