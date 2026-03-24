#include <X11/keysym.h>
#include "cub_structs.h"
#include "cub_utils.h"

int	mlx_mouse_get_pos(void *mlx, void *win_ptr, int *x, int *y);

// 4 ButtonPress
// 12345 = LMB MMB RMB WHEELUP WHEELDOWN
int	cmlx_mousedown(int button, int32_t x, int32_t y, t_game *game)
{
	return (0);
}

// 5 ButtonRelease
int	cmlx_mouseup(int button, int32_t x, int32_t y, t_game *game)
{
	if (game->pause == false)
	{
		if (button == 1)
			game->mouse_l = true;
	}
	return (0);
}

int	cmlx_mousemove(t_game *game)
{
	int x1, y1;
	float	dx;

	if (game->pause == false)
	{
		mlx_mouse_get_pos(game->mlx, game->mlx->win_list, &x1, &y1);
		dx = (float)(x1 - ((float)SCREEN_WIDTH / 2)) * game->cfg.sens;
		game->player.cam.dir = vec2_rotate(game->player.cam.dir, dx);
		game->player.cam.plane = vec2_rotate(game->player.cam.plane, dx);
		mlx_mouse_move(game->mlx, game->mlx->win_list, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	}
	return (0);
}
