#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cmlx_base.h"
#include "cmlx_draw.h"
#include "cub_structs.h"
#include "cub_utils.h"

// Animation system pipeline:
// 1) Input is processed, and player states are determined (shooting, reloading, etc). This also changes the animation sheet to be loaded.
// 2) The animation render takes care of the frame advancing, and returns whether the animation is done or not
// 3) The return is processed, and player states are determined accordingly.
// Example: 
// 1) R is pressed, and if the player isnt shooting and is below max ammo, he goes into the reloading state. Reloading hands are loaded into hands
// 2) Animation gets rendered, and returns that it was the first rendered frame of a reload cycle
// 3) Given that return, ammo count is increased. Suppose that it returned end of animation, then the player state gets changed to idle and idle hands are loaded back

// Is faster, and always advanced animation by one frame only
// In low framerates, will slow down the animation
// TODO: Insert the concept of frame period
// 1) First Render Call, 2) Updated, 4) End
uint8_t	stt_render_animation(t_sheet *sheet, long dt)
{
	uint8_t	rvalue;

	rvalue = (sheet->index == 0) && (sheet->frame_dt == 0);
	sheet->frame_dt += dt;
	if (sheet->frame_dt < sheet->frame_time)
		return (rvalue);
	rvalue |= (1 << 1);
	sheet->index++;
	if (sheet->index >= sheet->count)	// check if >= or >
	{
		sheet->index = 0;
		rvalue |= (1 << 2);
	}
	sheet->frame_dt = 0;
	return (rvalue);
}

// Map needs to hold the collision state of each block
// It is not possible to encode this info in sheet, otherwise all doors would move
// Player interacts with a door, function saves a pointer to the 
void	stt_update_hud(t_player *player, t_assets *assets, t_drawbuf *drawbuf, long dt)
{
	uint8_t	rvalue;

	rvalue = stt_render_animation(&drawbuf->hands, dt);
	if (player->state == st_reloading && rvalue >= 2 && (drawbuf->hands.index % RELOAD_CYCLE == 0))
		player->ammo++;
	if (rvalue >= 4)
	{
		drawbuf->hands = assets->walk;
		player->state = st_idle;
	}
	// RADAR TMP >>>
	if (player->map & st_raising)
		stt_render_animation(&drawbuf->radar, dt);
	stt_render_animation(&drawbuf->radar_l0, dt);
	stt_render_animation(&drawbuf->radar_l1, dt);
	// <<< RADAR TMP
}

// Todo: add state change function with hysteresis for audio
void	cub_update_state(t_player *player, t_audio *audio, t_game *game, long dt)
{
	static void		*prev_move_state = NULL;
	static int32_t	prev_ammo = AMMO_COUNT;
	void			*new_move_state;
	const float		new_spd = fmaxf(fabsf(player->speed.x.f), fabsf(player->speed.y.f));

	stt_update_hud(player, &game->assets, &game->drawbuf, dt);
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
	prev_move_state = new_move_state;
	prev_ammo = player->ammo;
}
