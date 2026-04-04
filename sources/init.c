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
	mlx_hook(window, ButtonRelease, ButtonReleaseMask, cmlx_mouseup, game);
	mlx_hook(window, DestroyNotify, 0, mlx_loop_end, game->mlx);
	mlx_mouse_hide(game->mlx, window);
	mlx_loop_hook(game->mlx, cmlx_loop, game);
	return (0);
}

#define ENM_WALK "assets/sprites/xpm/characters/zombie/enemy_walking_front"
#define ENM_SHOOT "assets/sprites/xpm/characters/zombie/enemy_shooting"
#define ENM_SHOT "assets/sprites/xpm/characters/zombie/enemy_shot"
#define ENM_EXPL "assets/sprites/xpm/characters/zombie/enemy_exploding"

static
void	stt_enemy_init(t_game *game)
{
	size_t	i;
	t_enemy	*enm;

	i = 0;
	ft_memset(game->enemies, 0, sizeof(t_enemy) * NUM_ENEMIES);
	while (i < NUM_ENEMIES)
	{
		enm = game->enemies + i;
		enm->running = cub_readsheet(game, ENM_WALK, 4, ANIM_TIME * 3);
		enm->shooting = cub_readsheet(game, ENM_SHOOT, 2, ANIM_TIME * 3);
		enm->shot = cub_readsheet(game, ENM_SHOT, 1, ANIM_TIME * 3);
		enm->dying = cub_readsheet(game, ENM_EXPL, 12, ANIM_TIME * 1);
		enm->state = e_idle;
		enm->health = 100;
		enm->cam.pos = random_valid_pos(&game->map);
		i++;
	}
}

#define TEX_SHOT "assets/sprites/xpm/hud/hands/hands_shooting"
#define TEX_WALK "assets/sprites/xpm/hud/hands/hands_walking"
#define TEX_RELOAD "assets/sprites/xpm/hud/hands/hands_reloading"
#define TEX_RADR "assets/sprites/xpm/hud/hands/hands_radar"
#define TEX_RADR0 "assets/sprites/xpm/hud/hands/map/layer0_"
#define TEX_RADR1 "assets/sprites/xpm/hud/hands/map/layer1_"
#define TEX_BLOOD "assets/sprites/xpm/hud/damage/damage"

static
int	stt_sprites_init(t_game *game, t_hands *hands)
{
	ft_memset(&game->assets, 0, sizeof(game->assets));
	hands->shoot = cub_readsheet(game, TEX_SHOT, 5, ANIM_TIME);
	hands->walk = cub_readsheet(game, TEX_WALK, 9, ANIM_TIME);
	hands->reload = cub_readsheet(game, TEX_RELOAD, 32, ANIM_TIME);
	hands->radar = cub_readsheet(game, TEX_RADR, 4, ANIM_TIME * 3);
	hands->radar_l0 = cub_readsheet(game, TEX_RADR0, 13, ANIM_TIME / 3);
	hands->radar_l1 = cub_readsheet(game, TEX_RADR1, 1, ANIM_TIME);
	game->assets.hud_blood = cub_readsheet(game, TEX_BLOOD, 3, ANIM_TIME);
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
	stt_sprites_init(game, &game->player.hands);
	stt_audio_init(game);
	stt_enemy_init(game);
	ft_rng_init();
	get_time();
	return (0);
}
