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

void	input_handler(t_game *game, t_player *player)
{
	if ((game->state.key & key_rmb) && !(player->state & st_shooting) && player->ammo > 0)
	{
		player->ammo--;
		player->state = (st_shooting | st_shot);
	}
	if ((game->state.key & key_r) && !(player->state & (st_reloading | st_shooting)) && player->ammo < AMMO_COUNT)
	{
		player->state = (st_reloading);
		player->hands.reload.index = player->ammo * RELOAD_CYCLE;
		player->map &= ~(size_t)st_raising;
		player->map &= ~(size_t)st_checking;
	}
	if ((game->state.key & key_e) && !(player->state & (st_reloading | st_shooting)))
	{
		player->state = st_interacting;
		game->state.key &= ~(size_t) key_e;
	}
	if	(game->state.key & key_tab)
	{
		game->player.map |= st_raising;
	}
	stt_move(game);
}

// if (game->state.key & key_tab)
// {
// 	if (!(player->state & (st_reloading | st_shooting))
// 			&& !(player->map & (st_raising | st_checking)))
// 		player->map |= (size_t)st_raising;
// }
// else if (!(game->state.key & key_tab))
// {
// 	game->player.hands.radar.index = 0;
// 	player->map &= ~(size_t)st_raising;
// 	player->map &= ~(size_t)st_checking;
// }
