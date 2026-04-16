#include <X11/X.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cmlx_base.h"
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"
#include "cub_assets.h"

static
void	stt_audio_init(t_game *game)
{
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(10);
	game->assets.audio.shot = Mix_LoadWAV("assets/audio/gun_shot.wav");
	game->assets.audio.reload = Mix_LoadWAV("assets/audio/gun_reload.wav");
	game->assets.audio.step = Mix_LoadWAV("assets/audio/step.wav");
	game->assets.audio.step_fast = Mix_LoadWAV("assets/audio/step_fast.wav");
	game->assets.audio.no_ammo = Mix_LoadWAV("assets/audio/no_ammo.wav");
}

static
void	stt_sdl_init(t_game *game)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
	{
		write(2, "failed to init sdl context\n", 26);
		exit(1);
	}
	game->window = SDL_CreateWindow
		(
		 "SDL_WINDOW",
		 SDL_WINDOWPOS_CENTERED,
		 SDL_WINDOWPOS_CENTERED,
		 s_width, s_height,
		 0
		);
	SDL_ShowCursor(SDL_DISABLE);
}


static
int	stt_mlx_init(t_game *game)
{
	/*t_win_list		*window; // to remove

	mlx_new_window(game->mlx, s_width, s_height, "doom_blade");
	if (game->mlx->win_list == NULL)
		return (cub_cleanup(game, "MLX (WIN) failed to initialize"));

	game->frame.img2 = mlx_int_new_image(game->mlx, s_width, s_height, ZPixmap); // to remove
	if (game->frame.img2 == NULL) // to remove
		return (cub_cleanup(game, "MLX (IMG) failed to initialize")); // to remove*/
	game->frame.img = SDL_CreateRGBSurfaceWithFormat
		(
		 0,
		 s_width,
		 s_height,
		 32,
		 SDL_PIXELFORMAT_ARGB8888
		 );

	/*window = game->mlx->win_list;
	mlx_hook(window, DestroyNotify, 0, mlx_loop_end, game->mlx);
	mlx_loop_hook(game->mlx, cmlx_loop, game);*/
	return (0);
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

static void	stt_load_assets(t_game *game, t_enemy enemies[NUM_ENEMIES], t_hands *hands)
{
	size_t	i;

	enemies[0].running = cub_readsheet(game, EWALK, EWALK_COUNT, EWALK_SPEED);
	enemies[0].shooting = cub_readsheet(game, EATK, EATK_COUNT, EATK_SPEED);
	enemies[0].shot = cub_readsheet(game, EHIT, EHIT_COUNT, EHIT_SPEED);
	enemies[0].dying = cub_readsheet(game, EDEATH, EDEATH_COUNT, EDEATH_SPEED);
	enemies[0].state = e_dead;
	enemies[0].model = &enemies[0].running;
	enemies[0].health = 100;	// Enemies spawn dead
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

int	cub_init(const char *filename, t_game *game, t_memory *memory)
{
	size_t		file_size;
	const char	*str;

	ft_memset(game, 0, sizeof(t_game));
	game->file = ft_read_all(filename, &file_size);
	str = game->file;
	if (game->file == NULL)
		return (cub_cleanup(game, "Failed to open file"));
	game->mlx = mlx_init();
	if (game->mlx == NULL)
		return (cub_cleanup(game, "MLX initialization failed"));
	// here we get each sprite from the paths written in the map input file
	cub_parse_textures(game, &str, memory);
	cub_read_map(game, str, &game->map, &game->player);
	stt_sdl_init(game);
	stt_mlx_init(game);
	stt_params_init(game, memory);
	stt_load_assets(game, game->enemies, &game->player.hands);
	stt_radar_init(game, &game->player.hands.radar_l0);
	stt_audio_init(game);
	ft_rng_init();
	get_time();
	return (0);
}
