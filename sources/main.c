#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cub_structs.h"
#include "cub_utils.h"

static
int	stt_mlx_init(t_game *game)
{
	t_win_list *window;

	game->mlx->win_list = mlx_new_window(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "doom_blade");
	if (game->mlx->win_list == NULL)
		return (-1);
	game->frame = mlx_int_new_image(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, ZPixmap);
	if (game->frame == NULL)
		return (-1);
	window = game->mlx->win_list;
	mlx_hook(window, KeyPress, KeyPressMask, cmlx_keydown, game);
	mlx_hook(window, KeyRelease, KeyReleaseMask, cmlx_keyup, game);
	mlx_hook(window, ButtonPress, ButtonPressMask, cmlx_mousedown, game);
	mlx_hook(window, ButtonRelease, ButtonReleaseMask, cmlx_mouseup, game);
	mlx_hook(window, MotionNotify, PointerMotionMask, cmlx_mousemove, game);
	mlx_mouse_hide(game->mlx, window);
	return (0);
}

static
void	stt_params_init(t_game *game)
{
	// game->vd = ft_qsqrt(game->map.cols * game->map.cols + game->map.rows * game->map.rows);
	game->player.cam.dir = (t_vec2){.x.f = 0.71f, .y.f = 0.71f};
	game->player.cam.plane = (t_vec2){.x.f = 0.0f, .y.f = 0.0f};
	game->player.dir_mod = 0;
	game->player.speed = 3;
	game->player.speed_mod = 1;
}

static
int	stt_cub_init(const char *filename, t_game *game)
{
	size_t		file_size;
	const char	*file = ft_read_all(filename, &file_size);
	const char	*str = file;

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
	stt_params_init(game);
	return (0);
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		return (1);
	if (stt_cub_init(argv[1], &game) == -1)
		return (1);
	// mlx_hook(game.win_ptr, 17, 0, free_displays, &game);
	// mlx_loop_hook(game.mlx, game_loop, &game);
	// mlx_loop(game.mlx);
}
