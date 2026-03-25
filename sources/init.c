#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

static
void	stt_audio_init(t_game *game)
{
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(10);
	game->assets.audio.gun_shot = Mix_LoadWAV("assets/audio/gun_shot.wav");
	game->assets.audio.gun_reload = Mix_LoadWAV("assets/audio/gun_reload.wav");
	game->assets.audio.step = Mix_LoadWAV("assets/audio/step.wav");
	game->assets.audio.step_fast = Mix_LoadWAV("assets/audio/step_fast.wav");
	game->assets.audio.no_ammo = Mix_LoadWAV("assets/audio/no_ammo.wav");
	game->assets.audio.current_step = game->assets.audio.step;
}

static
int stt_mlx_init(t_game *game)
{
	t_win_list *window;

	mlx_new_window(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "doom_blade");
	if (game->mlx->win_list == NULL)
		return (-1);
	game->frame.img = mlx_int_new_image(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, ZPixmap);
	if (game->frame.img == NULL)
		return (-1);
	window = game->mlx->win_list;
	mlx_hook(window, KeyPress, KeyPressMask, cmlx_keydown, game);
	mlx_hook(window, KeyRelease, KeyReleaseMask, cmlx_keyup, game);
	mlx_hook(window, ButtonPress, ButtonPressMask, cmlx_mousedown, game);
	mlx_hook(window, ButtonRelease, ButtonReleaseMask, cmlx_mouseup, game);
	mlx_mouse_hide(game->mlx, window);
	mlx_loop_hook(game->mlx, cmlx_loop, game);
	return (0);
}

static
t_cfg   stt_config(void)
{
	t_cfg   cfg;

	cfg.sens = 1.0f / 512.0f;
	return (cfg);
}

static
void	stt_tmp(t_game *game, t_memory *memory)
{
	t_view	cam;
	t_speed	speed;
	t_mat32	texture;

	ft_memset(&cam, 0, sizeof(cam));
	ft_memset(&speed, 0, sizeof(speed));
	game->enemies[0].cam = cam;
	game->enemies[0].move = speed;
	cub_read_texture(game->mlx, &texture, "assets/ghost_tmp.xpm", NULL);
	game->enemies[0].texture = texture;
}

static
void	stt_sprites_init(t_game *game)
{
	game->assets.shoot = cub_read_spritesheet(game->mlx, "assets/sprites/xpm/hud/hands/hands_shooting", 6);
	game->assets.shoot.end = false;
	game->assets.shoot.loops_per_sprite = 5;
	game->assets.shoot.iterator = 0;
	game->assets.shoot.counter = 0;
	if (game->assets.shoot.texture.ptr == NULL)
		return ;
	game->assets.walk = cub_read_spritesheet(game->mlx, "assets/sprites/xpm/hud/hands/hands_walking", 8);
	game->assets.walk.end = false;
	game->assets.walk.loops_per_sprite = 5;
	game->assets.reload = cub_read_spritesheet(game->mlx, "assets/sprites/xpm/hud/hands/hands_reloading", 34);
	game->assets.reload.end = false;
	game->assets.reload.loops_per_sprite = 5;
	game->assets.ammo = cub_read_spritesheet(game->mlx, "assets/sprites/xpm/hud/hud_ammo/ammo", 10);
	game->assets.ammo.end = false;
	game->assets.ammo.loops_per_sprite = 0;
	game->assets.health = cub_read_spritesheet(game->mlx, "assets/sprites/xpm/hud/hud_health/health", 10);
	game->assets.health.end = true;
	game->assets.health.loops_per_sprite = 0;
	game->assets.pill = cub_read_spritesheet(game->mlx, "assets/sprites/xpm/hud/hud_pill/pill", 2);
	game->assets.pill.end = true;
	game->assets.pill.loops_per_sprite = 0;
	game->assets.city = cub_read_spritesheet(game->mlx, "assets/sprites/xpm/tiles/city", 4);
	game->assets.city.end = true;
	game->assets.city.loops_per_sprite = 0;
	game->assets.radar = cub_read_spritesheet(game->mlx, "assets/sprites/xpm/hud/hud_radar/radar", 1);
	game->assets.radar.end = true;
	game->assets.radar.start = false;

	//bools
	game->assets.reload.start = false;
	game->assets.shoot.start = false;
	game->assets.shoot.sound = false;
}


static
void	stt_params_init(t_game *game, t_memory *memory)
{
	const t_mat32   empty = {memory->empty_line, 1, RENDER_HEIGHT, 1, 0};

	//game->vd = ft_qsqrt(game->map.cols * game->map.cols + game->map.rows * game->map.rows);
	game->player.cam.dir = (t_vec2){.x.f = 0.0f, .y.f = 1.0f};
	game->player.cam.plane = (t_vec2){
		.x.f = -game->player.cam.dir.y.f * 0.66f,
		.y.f =  game->player.cam.dir.x.f * 0.66f
	};
	game->frame.display.ptr = (uint32_t*)game->frame.img->data;
	game->frame.display.height = game->frame.img->height;
	game->frame.display.width = game->frame.img->width;
	game->frame.display.stride = game->frame.img->width;
	game->frame.render.ptr = (uint32_t*)memory->render_frame;
	game->frame.render.height = RENDER_HEIGHT;
	game->frame.render.width = RENDER_WIDTH;
	game->frame.render.stride = RENDER_HEIGHT;
	game->frame.rays = memory->rays;
	ft_memset(memory->empty_line, 0, sizeof(memory->empty_line));
	game->blocks[0].east = empty;
	game->blocks[0].west = empty;
	game->player.move.speed.x.f = 0.0f;
	game->player.move.speed.y.f = 0.0f;
	game->cfg = stt_config();
	stt_tmp(game, memory);
}

int cub_init(const char *filename, t_game *game, t_memory *memory)
{
	size_t	  file_size;
	const char  *file = ft_read_all(filename, &file_size);
	const char  *str = file;

	ft_memset(game, 0, sizeof(*game));
	if (file == NULL)
		return (-1);	// TODO: PRINT ERROR
	game->mlx = mlx_init();
	if (game->mlx == NULL)
		return (-1);	// TODO: Print Error
	if (cub_read_textures(game->mlx, str, &str, game->blocks) == -1)	// Double check: sending by value wont affect the struct
		return (free(file), -1);
	if (cub_read_map(str, &game->map, &game->player) == -1)
		return (free(file), -1);
	free(file);
	if (stt_mlx_init(game) == -1)
		return (-1);
	// if (cub_is_map_enclosed(game->map, game->player.cam.pos) == -1)
	//  return (-1);
	stt_params_init(game, memory);
	// NEW
	stt_sprites_init(game);
	stt_audio_init(game);
	game->state.paused = false;
	game->gun.ammo = 8;
	game->gun.max_ammo = 8;
	game->gun.first_iterator = -1;
	get_time();
	return (0);
}
