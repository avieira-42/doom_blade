#include <X11/Xutil.h>
#include <X11/keysym.h>
#include "cub_structs.h"
#include "cub_utils.h"

static
void	stt_move(t_game *game)
{
	float	spd_forward;
	float	spd_sideway;

	spd_forward = !!(game->state.key & key_w) - !!(game->state.key & key_s);
	spd_sideway = !!(game->state.key & key_d) - !!(game->state.key & key_a);
	game->player.spd_forward = spd_forward;
	game->player.spd_sideway = spd_sideway;
	if (game->state.key & key_shift)
		game->player.spd_max = PLAYER_WALK_SPEED;
	else
		game->player.spd_max = PLAYER_RUN_SPEED;
	if (game->state.key & key_left)
	{
		game->player.cam.dir = vec2_rotate(game->player.cam.dir, -PI_DEG);
		game->player.cam.plane = vec2_rotate(game->player.cam.plane, -PI_DEG);
	}
	if (game->state.key & key_right)
	{
		game->player.cam.dir = vec2_rotate(game->player.cam.dir, PI_DEG);
		game->player.cam.plane = vec2_rotate(game->player.cam.plane, PI_DEG);
	}
}

void	input_handler(t_game *game)
{
	if ((game->state.key & key_rmb) && game->player.state != st_shooting)
	{
		if (game->player.ammo == 0)
		{
			Mix_PlayChannel(ch_no_ammo, game->assets.audio.no_ammo, 0);
			game->state.key &= ~(size_t)key_rmb;
		}
		else
		{
			game->player.ammo--;
			game->drawbuf.hands = game->assets.shoot;
			game->player.state = (st_shooting | st_shot);
		}
	}
	if ((game->state.key & key_r) && !(game->player.state & (st_reloading | st_shooting)) && game->player.ammo < AMMO_COUNT)
	{
		game->player.state = (st_reloading);
		game->drawbuf.hands = game->assets.reload;
		game->drawbuf.hands.index = game->player.ammo * RELOAD_CYCLE;
		game->player.map &= ~(size_t)st_raising;
		game->player.map &= ~(size_t)st_checking;
	}
	if ((game->state.key & key_e) && !(game->player.state & (st_reloading | st_shooting)))
	{
		game->player.state = st_interacting;
		game->state.key &= ~(size_t) key_e;
	}
	if (game->state.key & key_tab)
	{
		if (!(game->player.state & (st_reloading | st_shooting))
				&& !(game->player.map & (st_raising | st_checking)))
			game->player.map |= (size_t)st_raising;
	}
	else if (!(game->state.key & key_tab))
	{
		game->drawbuf.radar.index = 0;
		game->player.map &= ~(size_t)st_raising;
		game->player.map &= ~(size_t)st_checking;
	}
	stt_move(game);
}
