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
int stt_mlx_init(t_game *game)
{
	t_win_list	*window;

	mlx_new_window(game->mlx, screen_width, screen_height, "doom_blade");
	if (game->mlx->win_list == NULL)
		return (cub_cleanup(game, "MLX (WIN) failed to initialize"));
	game->frame.img = mlx_int_new_image(game->mlx, screen_width, screen_height, ZPixmap);
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
	game->player.texture[0].ptr = NULL;
	game->player.texture[0].ptr = NULL;
	game->frame.display.ptr = (uint32_t*)game->frame.img->data;
	game->frame.display.height = game->frame.img->height;
	game->frame.display.width = game->frame.img->width;
	game->frame.display.stride = game->frame.img->width;
	game->frame.render.ptr = (uint32_t*)memory->render_frame;
	game->frame.render.height = render_height;
	game->frame.render.width = render_width;
	game->frame.render.stride = render_height;
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
		/ game->map.width, .y.i = game->map.radar_size.y.i / game->map.height};
	game->map.radar_quad.color = RADAR_CELL_COLOR;
	game->map.radar_quad.radius = game->player.hands.radar_l0.texture.width / 2
		- RADAR_BORDER_SIZE;
	game->map.radar_quad.center = (t_vec2){
		.x.i = game->map.radar_sprite_pos.x.i
			+ game->player.hands.radar_l0.texture.width / 2,
			.y.i = game->map.radar_sprite_pos.y.i
				+ game->player.hands.radar_l0.texture.height / 2};
	game->map.icon_quad.size = (t_vec2){.x.i = RADAR_ICON_SIZE,
		.y.i = RADAR_ICON_SIZE};
	game->map.icon_quad.color = RADAR_PLAYER_COLOR;
	game->map.icon_quad.radius = game->map.icon_quad.size.x.i / 2;
}

static
void	stt_load_assets(t_game *game, t_enemy enemies[NUM_ENEMIES], t_hands *hands)
{
	size_t	i;

	ft_memset(enemies, 0, sizeof(t_enemy) * NUM_ENEMIES);
	ft_memset(&game->assets, 0, sizeof(game->assets));
	enemies[0].running = cub_readsheet(game, ENEMY_WALK, ENEMY_WALK_COUNT, ENEMY_WALK_SPEED);
	enemies[0].shooting = cub_readsheet(game, ENEMY_ATK, ENEMY_ATK_COUNT, ENEMY_ATK_SPEED);
	enemies[0].shot = cub_readsheet(game, ENEMY_HIT, ENEMY_HIT_COUNT, ENEMY_HIT_SPEED);
	enemies[0].dying = cub_readsheet(game, ENEMY_DEATH, ENEMY_DEATH_COUNT, ENEMY_DEATH_SPEED);
	enemies[0].state = e_idle;
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
	{
		enemies[i] = enemies[0];	// Now enemies all draw from the same texture
		enemies[i].cam.pos = random_valid_pos(&game->map);
		i++;
	}
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
	cub_parse_textures(game, &str, memory);
	cub_read_map(game, str, &game->map, &game->player);
	stt_mlx_init(game);
	stt_params_init(game, memory);
	stt_load_assets(game, game->enemies, &game->player.hands);
	stt_radar_init(game);
	ft_rng_init();
	get_time();
	return (0);
}
