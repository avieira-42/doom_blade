#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_structs.h"
#include "cub_utils.h"

static
float	stt_clamp(float value)
{
	if (value >= (1.0f / 32))
		return ((1.0f / 32));
	else if (value <= -(1.0f / 32))
		return (-(1.0f / 32));
	return (value);
}

static
void	stt_update_pos(t_game *game)
{
	const float xdir = !!(game->key & key_a) - !!(game->key & key_d);
	const float ydir = !!(game->key & key_w) - !!(game->key & key_s);

	game->player.move.speed.x.f += xdir * (1.0f / 128);
	game->player.move.speed.y.f += ydir * (1.0f / 128);
	game->player.move.speed.x.f = stt_clamp(game->player.move.speed.x.f);
	game->player.move.speed.y.f = stt_clamp(game->player.move.speed.y.f);
	if (xdir == 0.0f)
		game->player.move.speed.x.f *= 0.8f;
	if (ydir == 0.0f)
		game->player.move.speed.y.f *= 0.8f;
	game->player.cam.pos.x.f += game->player.move.speed.x.f;
	game->player.cam.pos.y.f += game->player.move.speed.y.f;
}

int	cmlx_loop(t_game *game)
{
	static long last_frame = 0;
	const long	dt = get_time();

	last_frame += dt;
	if (last_frame > 10000)
	{
		stt_update_pos(game);
		last_frame = 0;	
	}
	ft_memset(game->display_frame.ptr, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
	ft_memset(game->render_frame.ptr, 0, RENDER_HEIGHT * RENDER_WIDTH * sizeof(uint32_t));
	render_image(&game->player.cam, &game->map, game->blocks, game->render_frame);
	ft_integer_scaling_t(game->render_frame, game->display_frame, 4);
	mlx_put_image_to_window(game->mlx, game->mlx->win_list, game->img, 0, 0);
	return (1);
}
