#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cmlx_base.h"
#include "cmlx_draw.h"
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

bool	cub_draw_relative(t_frame *frame, t_rayhit *rays, t_player *player, t_enemy *enemy);
void	cub_update_state(t_player *player, t_audio *audio, t_game *game, long dt);

static
void	stt_draw_enemies(t_game *game, long dt)
{
	size_t	i;
	bool	hit;
	t_enemy	*enemy;

	i = 0;
	while (i < NUM_ENEMIES)
	{
		enemy = game->enemies + i;
		if (enemy->health > 0)
		{
			hit = cub_draw_relative(&game->frame, game->frame.rays, &game->player, enemy);
			if ((game->player.state & st_shot) && hit == true)
				enemy->health -= 26;
		}
		else
		{
			enemy->respawn_timer += dt;
			if (enemy->respawn_timer > RESPAWN_TIMER)
			{
				enemy->respawn_timer = 0;
				enemy->cam.pos = random_valid_pos(&game->map);
				enemy->health = 100;
			}
		}
		i++;
	}
	game->player.state &= ~(size_t)st_shot;	// Clears the just shot flag
}

void	stt_draw_hud(t_mat32 frame, t_drawbuf *drawbuf)
{
	t_mat32	texture;

	texture = drawbuf->hands.texture;
	texture.ptr += drawbuf->hands.index * drawbuf->hands.frame_size;		// Hands
	size_t	x_tmp = (RENDER_WIDTH - texture.width) / 2;
	size_t	y_tmp = RENDER_HEIGHT - texture.height;
	cub_draw_texture(frame, texture, x_tmp, y_tmp);
	// .. Radar
	// TMP RADAR >>>>
	texture = drawbuf->radar.texture;
	texture.ptr += drawbuf->radar.index * drawbuf->radar.frame_size;
	cub_draw_texture(frame, texture, 0, 195);
	/*texture = drawbuf->radar_l0.texture;
	texture.ptr += drawbuf->radar.index * drawbuf->radar_l0.frame_size;
	cub_draw_texture(frame, texture, 110, 240);
	texture = drawbuf->radar_l1.texture;
	texture.ptr += drawbuf->radar.index * drawbuf->radar_l1.frame_size;
	cub_draw_texture(frame, texture, 110, 240);*/
	// <<<< TMP RADAR
}

size_t	stt_first_neighbor(t_vec2 pos, t_vec2 dir, t_map *map)
{
	int		cx;
	int		cy;
	float	tx;
	float	ty;

	cx = (int)floorf(pos.x.f);
	cy = (int)floorf(pos.y.f);
	tx = FLT_MAX;
	ty = FLT_MAX;
	if (dir.x.f > 0.0f)
		tx = ((cx + 1.0f) - pos.x.f) / dir.x.f;
	else if (dir.x.f < 0.0f)
		tx = (cx - pos.x.f) / dir.x.f;
	if (dir.y.f > 0.0f)
		ty = ((cy + 1.0f) - pos.y.f) / dir.y.f;
	else if (dir.y.f < 0.0f)
		ty = (cy - pos.y.f) / dir.y.f;
	if (tx < ty)
		cx += (dir.x.f > 0.0f ? 1 : -1);
	else
		cy += (dir.y.f > 0.0f ? 1 : -1);
	return (cy * map->width + cx);
}

int	cub_actions(t_game *game)
{
	size_t	index;

	if (game->player.state & st_interacting)
	{
		index = stt_first_neighbor(game->player.cam.pos, game->player.cam.dir, &game->map);
		if (game->map.tex_index[index] == 130)
		{
			game->map.tex_index[index] &= 127;
			game->map.state[index] = 0;
		}
		else if (game->map.tex_index[index] == 2)
		{
			game->map.tex_index[index] |= 128;
			game->map.state[index] = 255;
		}
		game->player.state = st_idle;
	}
	return (0);
}

int	cmlx_loop(t_game *game)
{
	static long	last_frame = 0;
	const long	dt = get_time();

	if (game->state.paused == false)
	{
		last_frame += dt;
		cub_update_pos(game, (double)dt * 0.000001);	// tmp	
		cub_actions(game);
		cub_update_state(&game->player, &game->assets.audio, game, dt);
		ft_memset(game->frame.display.ptr, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
		ft_memset(game->frame.render.ptr, 0, RENDER_HEIGHT * RENDER_WIDTH * sizeof(uint32_t));
		render_image(game);
		stt_draw_enemies(game, dt);
		stt_draw_hud(game->frame.render, &game->drawbuf);
		// TMP RADAR >>>
		animate_hud(game);
		// <<< TMP RADAR
		ft_integer_scaling_t(game->frame.render, game->frame.display, UPSCALING_FACTOR);
		mlx_put_image_to_window(game->mlx, game->mlx->win_list, game->frame.img, 0, 0);
		cmlx_mousemove(game);
		last_frame = 0;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	static t_game	game;
	static t_memory	memory;

	if (argc != 2)
		return (1);
	if (cub_init(argv[1], &game, &memory) == -1)
		return (1);
	mlx_loop(game.mlx);
}
