#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

typedef struct s_audio
{
	Mix_Chunk	*no_ammo;
	Mix_Chunk	*shot;
	Mix_Chunk	*reload;
	Mix_Chunk	*step;
	Mix_Chunk	*step_fast;
}	t_audio;
