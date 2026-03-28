#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cmlx_base.h"
#include "cmlx_draw.h"
#include "cub_structs.h"
#include "cub_utils.h"
// On event animation
// Health, Ammo, Pill

// Hands reloading had duplicate texture

// 30000 us
// Always displays the correct frame regardless of lag spikes
// In low framerates, will skip frames
uint8_t	stt_animate_precise(t_sheet *sheet, long dt)
{
	uint8_t	rvalue;
	long	frame_increments;

	sheet->frame_dt += dt;
	if (sheet->frame_dt < sheet->frame_time)
		return (0);
	frame_increments = (sheet->frame_dt / sheet->frame_time);			// Integer division
	rvalue = 1 + ((sheet->index + frame_increments) > sheet->count);
	sheet->index = (sheet->index + frame_increments) % sheet->count;	// Integer division
	sheet->frame_dt -= frame_increments * sheet->frame_time;
	return (rvalue);
}

// Is faster, and always advanced animation by one frame only
// In low framerates, will slow down the animation
// Returns 1 if updated, 2 on animation end
uint8_t	stt_animate(t_sheet *sheet, long dt)
{
	uint8_t	rvalue;

	sheet->frame_dt += dt;
	if (sheet->frame_dt < sheet->frame_time)
		return (0);
	sheet->index++;
	rvalue = 1;
	if (sheet->index >= sheet->count)	// check if >= or >
	{
		sheet->index = 0;
		rvalue = 2;
	}
	sheet->frame_dt -= sheet->frame_time;
	if (sheet->frame_dt < 0)
		sheet->frame_dt = 0;
	return (rvalue);
}

// Just shot condition is: animation state is shooting, and drawbuf->hands index is 0
void	stt_update_hud(t_player *player, t_assets *assets, t_drawbuf *drawbuf, long dt)
{
	uint8_t	rvalue;

	drawbuf->health.index = ft_imax(0, drawbuf->health.count - drawbuf->health.count * ((float)player->health / PLAYER_HEALTH));
	drawbuf->ammo.index = ft_imax(0, drawbuf->ammo.count - drawbuf->ammo.count * ((float)player->ammo / AMMO_COUNT) - 1);			// Ammo frame count is 10, but max ammo count is 8
	drawbuf->pill.index = 0;
	rvalue = stt_animate(&drawbuf->hands, dt);
	if (player->state == st_reloading && rvalue > 0 && (drawbuf->hands.index % RELOAD_CYCLE == 0))
		player->ammo++;
	if (rvalue == 2)
	{
		drawbuf->hands = assets->walk;
		player->state = st_idle;
	}
}

void	cub_update_state(t_player *player, t_audio *audio, t_game *game, long dt)
{
	static void		*prev_state = NULL;
	static int32_t	prev_ammo = AMMO_COUNT;
	void			*new_state;
	const float		new_speed = fmaxf(fabsf(player->speed.x.f), fabsf(player->speed.y.f));

	stt_update_hud(player, &game->assets, &game->drawbuf, dt);
	if (new_speed < HALT_THR)
		new_state = NULL;
	else if (new_speed > SPEED_THR)
		new_state = game->assets.audio.step_fast;
	else
		new_state = game->assets.audio.step;
	if (player->ammo < prev_ammo)
		Mix_PlayChannel(ch_shot, audio->shot, 0);
	else if (player->ammo > prev_ammo)
		Mix_PlayChannel(ch_reload, audio->reload, 0);
	if (new_state != prev_state)
	{
		Mix_HaltChannel(ch_steps);
		Mix_PlayChannel(ch_steps, new_state, -1);
	}
	prev_state = new_state;
	prev_ammo = player->ammo;
}
