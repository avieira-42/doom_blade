#include "cub_structs.h"
#include "cub_utils.h"

static
void	stt_draw_layer(t_mat32 frame, t_sheet *layer,
		t_vec2 first_pixel_pos, long dt)
{
	t_mat32	texture;

	texture = layer->texture;
	texture.ptr += layer->index * layer->frame_size;
	cub_draw_texture(frame, texture, first_pixel_pos.x.i, first_pixel_pos.y.i);
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

static
void	stt_fov_draw(t_mat32 frame, t_player player, t_quad p_icon)
{
	const t_vec2	p_pos = (t_vec2){.x.f
		= (float)p_icon.center.x.i,
			.y.f = (float)p_icon.center.y.i};
	t_vec2			line_dst;

	line_dst = vec2_sum(p_pos,
			vec2_mult(player.cam.dir, 5));
	line_draw(frame, p_pos, line_dst, RADAR_FOV_COLOR);
}

// for funcitonality separation
static
void	stt_player_icon_draw(t_mat32 frame, t_map map,
		t_vec2 grid_pos, t_player player)
{
	map.icon_quad.pos = (t_vec2){.x.i = map.radar_sprite_pos.x.i
		+ map.icon_quad.pos.x.i - grid_pos.x.i - map.icon_quad.size.x.i / 2,
		.y.i = map.radar_sprite_pos.y.i + map.icon_quad.pos.y.i
			- grid_pos.y.i - map.icon_quad.size.y.i / 2};
	map.icon_quad.center = (t_vec2){.x.i = map.icon_quad.pos.x.i + map.icon_quad.radius,
		.y.i = map.icon_quad.pos.y.i + map.icon_quad.radius};
	stt_fov_draw(frame, player, map.icon_quad);
	quad_draw(frame, map.icon_quad);
}

static
void	stt_enemies_icon_draw(t_mat32 frame, t_map map, t_vec2 grid_pos, t_enemy *enemies)
{
	int32_t	i;

	i = 0;
	map.icon_quad.color = RADAR_ENEMY_COLOR;
	while (i < NUM_ENEMIES)
	{
		if (enemies[i].state & e_dead)
		{
			i++;
			continue;
		}
		map.icon_quad.pos = (t_vec2){.x.i = grid_pos.x.i
			+ enemies[i].cam.pos.x.f * map.radar_quad.size.x.i,
				.y.i = grid_pos.y.i + enemies[i].cam.pos.y.f * map.radar_quad.size.y.i};
		map.icon_quad.center = (t_vec2){.x.i = map.icon_quad.pos.x.i + map.icon_quad.radius,
			.y.i = map.icon_quad.pos.y.i + map.icon_quad.radius};
		if (vec2_idist(map.radar_quad.center, map.icon_quad.pos)
				< map.radar_quad.radius * map.radar_quad.radius)
			quad_draw(frame, map.icon_quad);
		i++;
	}
}

void	cub_draw_radar(t_game *game, t_map map, long dt)
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
	stt_player_icon_draw(game->frame.render, map, grid_offset, game->player);
	stt_draw_layer(game->frame.render,
			&game->player.hands.radar_l1, map.radar_sprite_pos, dt);
}
