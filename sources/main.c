#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cmlx_base.h"
#include "cmlx_draw.h"
#include "cub_structs.h"
#include "cub_utils.h"

bool	cub_draw_relative(t_mat32 frame, t_rayhit *rays, t_player *player, t_enemy *enemy);
void	cub_update_state(t_player *player, t_audio *audio, t_game *game, long dt);

static
void	stt_draw_enemies(t_game *game)
{
	size_t		i;
	t_enemy	*enemy;

	i = 0;
	while (i < NUM_ENEMIES)
	{
		enemy = game->enemies + i;
		enemy->hit = cub_draw_relative(game->frame.render, game->frame.rays, &game->player, game->enemies);
		i++;
	}
}

void	stt_draw_hud(t_mat32 frame, t_drawbuf *drawbuf)
{
	t_mat32	texture;

	texture = drawbuf->ammo.texture;
	texture.ptr += drawbuf->ammo.index * drawbuf->ammo.frame_size;
	cub_draw_texture(frame, texture, (t_vec2){.x.u = 15, .y.u = 15});		// Ammo
	texture = drawbuf->health.texture;
	texture.ptr += drawbuf->health.index * drawbuf->health.frame_size;
	cub_draw_texture(frame, texture, (t_vec2){.x.u = 90 + 15, .y.u = 15});	// Health
	texture = drawbuf->pill.texture;
	texture.ptr += drawbuf->pill.index * drawbuf->pill.frame_size;
	cub_draw_texture(frame, texture, (t_vec2){.x.u = 180 + 15, .y.u = 15});	// Pill
	texture = drawbuf->hands.texture;
	texture.ptr += drawbuf->hands.index * drawbuf->hands.frame_size;		// Hands
	cub_draw_texture(frame, texture, (t_vec2){.x.i = SCREEN_WIDTH / 5, .y.i = SCREEN_HEIGHT / 2.5});
	// .. Radar
}

int	cmlx_loop(t_game *game)
{
	static long	last_frame = 0;
	const long	dt = get_time();

	if (game->state.paused == false)
	{
		last_frame += dt;
		if (last_frame > 10000)
		{
			cub_update_pos(game, (double)dt * 0.000001);	// tmp
			last_frame = 0;
		}
		cub_update_state(&game->player, &game->assets.audio, game, dt);
		ft_memset(game->frame.display.ptr, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
		ft_memset(game->frame.render.ptr, 0, RENDER_HEIGHT * RENDER_WIDTH * sizeof(uint32_t));
		render_image(&game->player.cam, &game->map, game->blocks, &game->frame);
		stt_draw_enemies(game);
		ft_integer_scaling_t(game->frame.render, game->frame.display, UPSCALING_FACTOR);
		stt_draw_hud(game->frame.display, &game->drawbuf);
		mlx_put_image_to_window(game->mlx, game->mlx->win_list, game->frame.img, 0, 0);
		cmlx_mousemove(game);
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
	game.enemies[0].cam.pos = game.player.cam.pos;
	mlx_loop(game.mlx);
}
