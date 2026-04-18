#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "game_defines.h"
#include "game_types.h"

static
void	stt_play_once(SDL_AudioStream *stream, const t_sound *sound)
{
	if (stream == NULL || sound == NULL || sound->buf == NULL || sound->len == 0)
		return ;
	SDL_ClearAudioStream(stream);
	SDL_PutAudioStreamData(stream, sound->buf, (int)sound->len);
	SDL_FlushAudioStream(stream);
}

static
void	stt_stop(SDL_AudioStream *stream)
{
	if (stream == NULL)
		return ;
	SDL_ClearAudioStream(stream);
}

static
void	stt_keep_looping(SDL_AudioStream *stream, const t_sound *sound)
{
	int	queued;

	if (stream == NULL || sound == NULL || sound->buf == NULL || sound->len == 0)
		return ;
	queued = SDL_GetAudioStreamQueued(stream);
	if (queued < 0)
		return ;
	while (queued < (int)(sound->len * 2))
	{
		if (!SDL_PutAudioStreamData(stream, sound->buf, (int)sound->len))
			return ;
		queued += (int)sound->len;
	}
}

void	cub_play_audio(t_player *player, t_audio *audio, t_game *game, long dt)
{
	static t_sound		*prev_move_state = NULL;
	static int32_t		prev_ammo = AMMO_COUNT;
	t_sound				*new_move_state;
	const float			new_spd = ABSMAX(player->speed.x.f, player->speed.y.f);

	(void)dt;
	if (new_spd < HALT_THR)
		new_move_state = NULL;
	else if (new_spd > SPEED_THR)
		new_move_state = &game->assets.audio.step_fast;
	else
		new_move_state = &game->assets.audio.step;

	if (player->ammo < prev_ammo)
		stt_play_once(audio->stream_shot, &audio->shot);
	else if (player->ammo > prev_ammo)
		stt_play_once(audio->stream_reload, &audio->reload);

	if (new_move_state != prev_move_state)
	{
		stt_stop(audio->stream_steps);
		if (new_move_state != NULL)
			stt_keep_looping(audio->stream_steps, new_move_state);
	}
	else if (new_move_state != NULL)
		stt_keep_looping(audio->stream_steps, new_move_state);

	if (game->player.ammo == 0 && (game->state.key & key_rmb))
	{
		stt_play_once(audio->stream_no_ammo, &audio->no_ammo);
		game->state.key &= ~(size_t)key_rmb;
	}
	prev_move_state = new_move_state;
	prev_ammo = player->ammo;
}
