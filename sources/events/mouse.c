#include <X11/Xutil.h>
#include <X11/keysym.h>
#include "cub_structs.h"
#include "cub_utils.h"

int	mlx_mouse_get_pos(void *mlx, void *win_ptr, int *x, int *y);

// 4 ButtonPress
// 12345 = LMB MMB RMB WHEELUP WHEELDOWN
// TODO: The X Y could be relevant for hitreg
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
	float	dy;
	float	offset;

	mlx_mouse_get_pos(game->mlx, game->mlx->win_list, &x1, &y1);
	dx = (float)(x1 - ((float)SCREEN_WIDTH / 2)) * (1.0f / 512.0f);
	dy = (float)(y1 - ((float)SCREEN_HEIGHT / 2));
	game->player.cam.dir = vec2_rotate(game->player.cam.dir, dx);
	game->player.cam.plane = vec2_rotate(game->player.cam.plane, dx);
	offset = game->frame.offset;
	offset += dy;
	offset = ft_absclamp(offset, OFFSET_MAX);
	game->frame.offset = offset;
	mlx_mouse_move(game->mlx, game->mlx->win_list, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	return (0);
}