#include <X11/Xutil.h>
#include <X11/keysym.h>
#include "cub_structs.h"
#include "cub_utils.h"

int	mlx_mouse_get_pos(void *mlx, void *win_ptr, int *x, int *y);

// 4 ButtonPress
// 12345 = LMB MMB RMB WHEELUP WHEELDOWN
int	cmlx_mousedown(int button, int32_t x, int32_t y, t_game *game)
{
	(void) x;
	(void) y;
	if (button == 1)
		game->state.key |= (size_t) key_rmb;
	return (0);
}

// 5 ButtonRelease
int	cmlx_mouseup(int button, int32_t x, int32_t y, t_game *game)
{
	(void) x;
	(void) y;
	if (button == 1)
		game->state.key &= ~(size_t)key_rmb;
	return (0);
}

int	cmlx_mousemove(t_game *game)
{
	int32_t	x1;
	int32_t	y1;
	float	dx;
	int32_t	dy;

	mlx_mouse_get_pos(game->mlx, game->mlx->win_list, &x1, &y1);
	dx = (float)(x1 - ((float)screen_width / 2)) *(1.0f / 512.0f);
	dy = y1 - (screen_height / 2);
	game->player.cam.dir = vec2_rotate(game->player.cam.dir, dx);
	game->player.cam.plane = vec2_rotate(game->player.cam.plane, dx);
	game->frame.offset = ft_iclamp(game->frame.offset + dy,
			-OFFSET_MAX, OFFSET_MAX);
	mlx_mouse_move(game->mlx, game->mlx->win_list,
			screen_width / 2, screen_height / 2);
	return (0);
}
