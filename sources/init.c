#include <X11/X.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cmlx_base.h"
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

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
int stt_mlx_init(t_game *game)
{
	t_win_list	*window;

	mlx_new_window(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "doom_blade");
	if (game->mlx->win_list == NULL)
		return (cub_cleanup(game, "MLX (WIN) failed to initialize"));
	game->frame.img = mlx_int_new_image(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, ZPixmap);
	if (game->frame.img == NULL)
		return (cub_cleanup(game, "MLX (IMG) failed to initialize"));
	window = game->mlx->win_list;
	mlx_hook(window, KeyPress, KeyPressMask, cmlx_keydown, game);
	mlx_hook(window, KeyRelease, KeyReleaseMask, cmlx_keyup, game);
	mlx_hook(window, ButtonPress, ButtonPressMask, cmlx_mousedown, game);
	mlx_hook(window, DestroyNotify, 0, mlx_loop_end, game->mlx);
	mlx_mouse_hide(game->mlx, window);
	mlx_loop_hook(game->mlx, cmlx_loop, game);
	return (0);
}

static
void	stt_enemy_init(t_game *game, t_memory *memory)
{
	size_t	i;

	i = 0;
	ft_memset(game->enemies, 0, sizeof(t_enemy) * NUM_ENEMIES);
	while (i < NUM_ENEMIES)
	{
		cub_read_texture(game->mlx, &game->enemies[i].texture, "assets/ghost_tmp.xpm", NULL);
		game->enemies[i].health = 100;
		game->enemies[i].cam.pos = random_valid_pos(&game->map);
		i++;
	}
}

// All of these need error printing
static
int	stt_sprites_init(t_game *game)
{
	ft_memset(&game->assets, 0, sizeof(game->assets));
	game->assets.shoot = cub_read_spritesheet(game, "assets/sprites/xpm/hud/hands/hands_shooting", 5, ANIM_TIME);	// REVIEW: all of these values were wrong. this is supposed to be count, not index where it ends
	game->assets.walk = cub_read_spritesheet(game, "assets/sprites/xpm/hud/hands/hands_walking", 9, ANIM_TIME);
	game->assets.reload = cub_read_spritesheet(game, "assets/sprites/xpm/hud/hands/hands_reloading", 32, ANIM_TIME);	// Changed from 33 to 32
	game->assets.city = cub_read_spritesheet(game, "assets/sprites/xpm/tiles/city", 4, ANIM_TIME);
	game->assets.radar = cub_read_spritesheet(game, "assets/sprites/xpm/hud/hands/hands_radar", 4, ANIM_TIME * 3);
	game->assets.radar_l0 = cub_read_spritesheet(game, "assets/sprites/xpm/hud/hands/map/layer0_", 13, ANIM_TIME / 3);
	game->assets.radar_l1 = cub_read_spritesheet(game, "assets/sprites/xpm/hud/hands/map/layer1_", 1, ANIM_TIME);
	game->drawbuf.hands = game->assets.walk;
	game->drawbuf.radar = game->assets.radar;
	game->drawbuf.radar_l0 = game->assets.radar_l0;
	game->drawbuf.radar_l1 = game->assets.radar_l1;
	// segredo >>>
	enemy_init(game);
	// <<< segredo
	return (0);
}

// Cannot fail
static
void	stt_params_init(t_game *game, t_memory *memory)
{
	game->player.cam.plane = (t_vec2){.x.f = -game->player.cam.dir.y.f * 0.66f,
		.y.f =  game->player.cam.dir.x.f * 0.66f};
	game->player.ammo = AMMO_COUNT;
	game->player.health = PLAYER_HEALTH;
	game->player.state = st_idle;
	game->player.map = st_off;
	game->frame.display.ptr = (uint32_t*)game->frame.img->data;
	game->frame.display.height = game->frame.img->height;
	game->frame.display.width = game->frame.img->width;
	game->frame.display.stride = game->frame.img->width;
	game->frame.render.ptr = (uint32_t*)memory->render_frame;
	game->frame.render.height = RENDER_HEIGHT;
	game->frame.render.width = RENDER_WIDTH;
	game->frame.render.stride = RENDER_HEIGHT;
	game->frame.rays = memory->rays;
	game->state.paused = false;
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
	cub_parse_textures(game, str, &str, game->blocks);
	cub_read_map(game, str, &game->map, &game->player);
	stt_mlx_init(game);
	stt_params_init(game, memory);
	stt_sprites_init(game);
	stt_audio_init(game);
	stt_enemy_init(game, memory);
	ft_rng_init();
	get_time();
	return (0);
}
