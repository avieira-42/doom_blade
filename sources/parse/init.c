#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "game_types.h"
#include "game_defines.h"
#include "game_prototypes.h"
#include "game_assets.h"

static
bool	stt_load_sound(t_sound *sound, const char *path)
{
	SDL_zero(*sound);
	return SDL_LoadWAV(path, &sound->spec, &sound->buf, &sound->len);
}

static
SDL_AudioStream	*stt_create_bound_stream(t_audio *audio,
	const t_sound *sound)
{
	SDL_AudioStream	*stream;

	if (audio->device == 0 || sound->buf == NULL || sound->len == 0)
		return NULL;
	stream = SDL_CreateAudioStream(&sound->spec, &audio->device_spec);
	if (stream == NULL)
		return NULL;
	if (!SDL_BindAudioStream(audio->device, stream))
	{
		SDL_DestroyAudioStream(stream);
		return NULL;
	}
	return stream;
}

static
void	stt_audio_init(t_game *game)
{
	t_audio			*audio;
	SDL_AudioSpec	hint;

	audio = &game->assets.audio;
	SDL_zero(*audio);
	if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
		return ;
	hint = (SDL_AudioSpec){
		.format = SDL_AUDIO_S16,
		.channels = 2,
		.freq = 44100
	};
	audio->device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &hint);
	if (audio->device == 0)
		return ;
	if (!SDL_GetAudioDeviceFormat(audio->device, &audio->device_spec, NULL))
		return ;
	stt_load_sound(&audio->shot, "assets/audio/gun_shot.wav");
	stt_load_sound(&audio->reload, "assets/audio/gun_reload.wav");
	stt_load_sound(&audio->step, "assets/audio/step.wav");
	stt_load_sound(&audio->step_fast, "assets/audio/step_fast.wav");
	stt_load_sound(&audio->no_ammo, "assets/audio/no_ammo.wav");
	audio->stream_shot = stt_create_bound_stream(audio, &audio->shot);
	audio->stream_reload = stt_create_bound_stream(audio, &audio->reload);
	audio->stream_steps = stt_create_bound_stream(audio, &audio->step);
	audio->stream_no_ammo = stt_create_bound_stream(audio, &audio->no_ammo);
}

static
void	stt_sdl_init(t_game *game)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		write(2, "failed to init sdl context\n", 27);
		exit(1);
	}
	game->window = SDL_CreateWindow("SDL_WINDOW", s_width, s_height, 0);
	if (game->window == NULL)
	{
		write(2, "failed to create window\n", 24);
		exit(1);
	}
	SDL_SetWindowRelativeMouseMode(game->window, true);
	game->frame.img = SDL_CreateSurface(s_width, s_height, SDL_PIXELFORMAT_ARGB8888);
	if (game->frame.img == NULL)
	{
		write(2, "failed to create frame surface\n", 31);
		exit(1);
	}
}

static
void	stt_params_init(t_game *game, t_memory *memory)
{
	game->player.cam.plane = (t_vec2){.x.f = -game->player.cam.dir.y.f * 0.66f,
		.y.f = game->player.cam.dir.x.f * 0.66f};
	game->player.ammo = AMMO_COUNT;
	game->player.health = PLAYER_HEALTH;
	game->player.state = st_idle;
	game->frame.display.ptr = (uint32_t *)game->frame.img->pixels;
	game->frame.display.height = game->frame.img->h;
	game->frame.display.width = game->frame.img->w;
	game->frame.display.stride = game->frame.img->w;
	game->frame.render.ptr = (uint32_t *)memory->render_frame;
	game->frame.render.height = R_HEIGHT;
	game->frame.render.width = R_WIDTH;
	game->frame.render.stride = R_HEIGHT;
	game->frame.rays = memory->rays;
	game->state.paused = false;
}

void	stt_radar_init(t_game *game, t_sheet *rd_0)
{
	const t_vec2	pos = (t_vec2){.x.i = 110, .y.i = 240};

	game->map.radar_sprite_pos = pos;
	game->map.radar_size = (t_vec2){.x.i = rd_0->texture.width * 2,
		.y.i = rd_0->texture.height * 2};
	game->map.radar_quad.size = (t_vec2){.x.i = game->map.radar_size.x.i
		/ game->map.width, .y.i = game->map.radar_size.y.i / game->map.height};
	game->map.radar_quad.color = RADAR_CELL_COLOR;
	game->map.radar_quad.radius = rd_0->texture.width / 2 - RADAR_BORDER_SIZE;
	game->map.radar_quad.center = (t_vec2){
		.x.i = pos.x.i + rd_0->texture.width / 2,
		.y.i = pos.y.i + rd_0->texture.height / 2};
	game->map.icon_quad.size = (t_vec2){
		.x.i = RADAR_ICON_SIZE, .y.i = RADAR_ICON_SIZE};
	game->map.icon_quad.color = RADAR_PLAYER_COLOR;
	game->map.icon_quad.radius = game->map.icon_quad.size.x.i / 2;
}

static
void	stt_load_assets(t_game *game, t_enemy enemies[NUM_ENEMIES], t_hands *hands)
{
	size_t	i;

	enemies[0].running = cub_readsheet(game, EWALK, EWALK_COUNT, EWALK_SPEED);
	enemies[0].shooting = cub_readsheet(game, EATK, EATK_COUNT, EATK_SPEED);
	enemies[0].shot = cub_readsheet(game, EHIT, EHIT_COUNT, EHIT_SPEED);
	enemies[0].dying = cub_readsheet(game, EDEATH, EDEATH_COUNT, EDEATH_SPEED);
	enemies[0].state = e_dead;
	enemies[0].model = &enemies[0].running;
	enemies[0].health = 0;		// Enemies spawn dead
	enemies[0].cam.pos = random_valid_pos(&game->map);
	hands->shoot = cub_readsheet(game, PLAYER_ATK, 5, ANIM_TIME);
	hands->walk = cub_readsheet(game, PLAYER_WALK, 9, ANIM_TIME);
	hands->reload = cub_readsheet(game, PLAYER_RELOAD, 32, ANIM_TIME);
	hands->radar = cub_readsheet(game, PLAYER_RADAR, 4, ANIM_TIME);
	hands->radar_l0 = cub_readsheet(game, PLAYER_RADAR0, 13, ANIM_TIME / 3);
	hands->radar_l1 = cub_readsheet(game, PLAYER_RADAR1, 1, ANIM_TIME);
	hands->idle = cub_readsheet(game, PLAYER_IDLE, 3, ANIM_TIME * 5);
	game->assets.hud_blood = cub_readsheet(game, PLAYER_BLOOD, 3, ANIM_TIME);
	i = 1;
	while (i < NUM_ENEMIES)
		enemies[i++] = enemies[0];
}

static
void	stt_texture_init(t_block *blocks, t_memory *memory)
{
	size_t		i;
	uint32_t	*ptr;
	size_t		total_size;
	t_mat32		base;

	base = (t_mat32){NULL, TEX_SIZE, TEX_SIZE, 0, TEX_SIZE};
	i = 0;
	while (i < NUM_BLOCKS)
	{
		blocks[i].north = base;
		blocks[i].south = base;
		blocks[i].east = base;
		blocks[i].west = base;
		blocks[i].north.ptr = &memory->textures[i][0][0][0];
		blocks[i].south.ptr = &memory->textures[i][1][0][0];
		blocks[i].east.ptr = &memory->textures[i][2][0][0];
		blocks[i].west.ptr = &memory->textures[i][3][0][0];
		i++;
	}
	i = 0;
	ptr = (uint32_t *)memory->textures;
	total_size = sizeof(memory->textures) / sizeof(uint32_t);
	while (i < total_size)
		ptr[i++] = rgb_pink;
}

int	cub_init(const char *filename, t_game *game, t_memory *memory)
{
	size_t		file_size;
	const char	*str;

	memset(game, 0, sizeof(t_game));
	game->file = ft_read_all(filename, &file_size);
	str = game->file;
	if (game->file == NULL)
		return (cub_cleanup(game, "Failed to open file"));
	stt_texture_init(game->blocks, memory);
	cub_parse_textures(game, &str);
	cub_read_map(game, str, &game->map, &game->player);
	stt_sdl_init(game);
	stt_params_init(game, memory);
	stt_load_assets(game, game->enemies, &game->player.hands);
	stt_radar_init(game, &game->player.hands.radar_l0);
	stt_audio_init(game);
	ft_rng_init();
	get_time();
	return (0);
}
