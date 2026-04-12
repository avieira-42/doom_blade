#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <cub_defines.h>
#include "cub_defines.h"
#include "cub_structs.h"

// Todo: add state change function with hysteresis for audio
void	cub_play_audio(t_player *player, t_audio *audio, t_game *game, long dt)
{
	static void		*prev_move_state = NULL;
	static int32_t	prev_ammo = AMMO_COUNT;
	void			*new_move_state;
	const float		new_spd = fmaxf(fabsf(player->speed.x.f), fabsf(player->speed.y.f));

	if (new_spd < HALT_THR)
		new_move_state = NULL;
	else if (new_spd > SPEED_THR)
		new_move_state = game->assets.audio.step_fast;
	else
		new_move_state = game->assets.audio.step;
	if (player->ammo < prev_ammo)
		Mix_PlayChannel(ch_shot, audio->shot, 0);
	else if (player->ammo > prev_ammo)
		Mix_PlayChannel(ch_reload, audio->reload, 0);
	if (new_move_state != prev_move_state)
	{
		Mix_HaltChannel(ch_steps);
		Mix_PlayChannel(ch_steps, new_move_state, -1);
	}
	if (game->player.ammo == 0 && (game->state.key & key_rmb))
	{
		Mix_PlayChannel(ch_no_ammo, game->assets.audio.no_ammo, 0);
		game->state.key &= ~(size_t)key_rmb;
	}
	prev_move_state = new_move_state;
	prev_ammo = player->ammo;
}
