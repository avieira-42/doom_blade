/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 20:36:40 by avieira-          #+#    #+#             */
/*   Updated: 2026/04/04 20:37:47 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

#define ENEMY_WALK "assets/sprites/xpm/characters/zombie/enemy_walking_front"
#define ENEMY_ATK "assets/sprites/xpm/characters/zombie/enemy_shooting"
#define ENEMY_HIT "assets/sprites/xpm/characters/zombie/enemy_shot"
#define ENEMY_DEATH "assets/sprites/xpm/characters/zombie/enemy_exploding"

static
void	stt_enemy_init(t_game *game)
{
	size_t	i;
	t_enemy	*enemy;

	i = 0;
	ft_memset(game->enemies, 0, sizeof(t_enemy) * NUM_ENEMIES);
	while (i < NUM_ENEMIES)
	{
		enemy = game->enemies + i;
		enemy->running = cub_readsheet(game, ENEMY_WALK, 4, ANIM_TIME * 3);
		enemy->shooting = cub_readsheet(game, ENEMY_ATK, 2, ANIM_TIME * 3);
		enemy->shot = cub_readsheet(game, ENEMY_HIT, 1, ANIM_TIME * 3);
		enemy->dying = cub_readsheet(game, ENEMY_DEATH, 12, ANIM_TIME * 1);
		enemy->state = e_idle;
		enemy->health = 100;
		enemy->cam.pos = random_valid_pos(&game->map);
		i++;
	}
}

#define PLAYER_ATK "assets/sprites/xpm/hud/hands/hands_shooting"
#define PLAYER_WALK "assets/sprites/xpm/hud/hands/hands_walking"
#define PLAYER_RELOAD "assets/sprites/xpm/hud/hands/hands_reloading"
#define PLAYER_RADAR "assets/sprites/xpm/hud/hands/hands_radar"
#define PLAYER_RADAR0 "assets/sprites/xpm/hud/hands/map/layer0_"
#define PLAYER_RADAR1 "assets/sprites/xpm/hud/hands/map/layer1_"
#define PLAYER_BLOOD "assets/sprites/xpm/hud/damage/damage"

static
int	stt_sprites_init(t_game *game, t_hands *hands)
{
	ft_memset(&game->assets, 0, sizeof(game->assets));
	hands->shoot = cub_readsheet(game, PLAYER_ATK, 5, ANIM_TIME);
	hands->walk = cub_readsheet(game, PLAYER_WALK, 9, ANIM_TIME);
	hands->reload = cub_readsheet(game, PLAYER_RELOAD, 32, ANIM_TIME);
	hands->radar = cub_readsheet(game, PLAYER_RADAR, 4, ANIM_TIME);
	hands->radar_l0 = cub_readsheet(game, PLAYER_RADAR0, 13, ANIM_TIME / 3);
	hands->radar_l1 = cub_readsheet(game, PLAYER_RADAR1, 1, ANIM_TIME);
	game->assets.hud_blood = cub_readsheet(game, PLAYER_BLOOD, 3, ANIM_TIME);
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

void stt_radar_init(t_game *game)
{
	game->map.radar_sprite_pos = (t_vec2){.x.i = 110, .y.i = 240};
	game->map.radar_size =
		(t_vec2){.x.i = game->player.hands.radar_l0.texture.width * 2,
			.y.i = game->player.hands.radar_l0.texture.height * 2};

	game->map.radar_quad.size = (t_vec2){.x.i = game->map.radar_size.x.i
		/ game->map.width,
		.y.i = game->map.radar_size.y.i / game->map.height};
	game->map.radar_quad.color = 0x000000; // DEFINES
	game->map.radar_quad.radius = game->player.hands.radar_l0.texture.width / 2;
	game->map.radar_quad.center = (t_vec2){
		.x.i = game->map.radar_sprite_pos.x.i
			+ game->player.hands.radar_l0.texture.width / 2,
			.y.i = game->map.radar_sprite_pos.y.i
				+ game->player.hands.radar_l0.texture.height / 2};
	game->map.icon_quad.size = (t_vec2){.x.i = 4, .y.i = 4};
	game->map.icon_quad.color = 0x005500; // DEFINES
	game->map.icon_quad.radius = game->map.icon_quad.size.x.i / 2;
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
	stt_radar_init(game);
	ft_rng_init();
	get_time();
	return (0);
}
