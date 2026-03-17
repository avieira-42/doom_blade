#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "cub_structs.h"

void	audio_init(t_game *game)
{
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(10);
	game->audio.gun_shot = Mix_LoadWAV("assets/audio/gun_shot.wav");
	game->audio.gun_reload = Mix_LoadWAV("assets/audio/gun_reload.wav");
	game->audio.step = Mix_LoadWAV("assets/audio/step.wav");
	game->audio.step_fast = Mix_LoadWAV("assets/audio/step_fast.wav");
	game->audio.no_ammo = Mix_LoadWAV("assets/audio/no_ammo.wav");
	game->audio.current_step = game->audio.step;
}
