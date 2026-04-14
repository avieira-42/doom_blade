#include <X11/Xutil.h>
#include <X11/keysym.h>
#include "cub_structs.h"
#include "cub_utils.h"

int	mlx_mouse_get_pos(void *mlx, void *win_ptr, int *x, int *y);

// 4 ButtonPress
// 12345 = LMB MMB RMB WHEELUP WHEELDOWN
int	cmlx_mousedown(uint8_t button, t_game *game)
{
	if (button == SDL_BUTTON_LEFT)
	{
		game->state.key |= (size_t) key_lmb;
	}
	return (0);
}

// 5 ButtonRelease
int	cmlx_mouseup(uint8_t button, t_game *game)
{
	if (button == SDL_BUTTON_LEFT)
		game->state.key &= ~(size_t)key_lmb;
	return (0);
}

int	cmlx_mousemove(t_game *game)
{
	int32_t	x1;
	int32_t	y1;
	float	dx;
	int32_t	dy;

	mlx_mouse_get_pos(game->mlx, game->mlx->win_list, &x1, &y1);
	dx = (float)(x1 - ((float)s_width / 2)) *(1.0f / 512.0f);
	dy = y1 - (s_height / 2);
	game->player.cam.dir = vec2_rotate(game->player.cam.dir, dx);
	game->player.cam.plane = vec2_rotate(game->player.cam.plane, dx);
	game->frame.offset = ft_iclamp
		(game->frame.offset + dy, -OFFSET_MAX, OFFSET_MAX);
	mlx_mouse_move(game->mlx, game->mlx->win_list, s_width / 2, s_height / 2);
	return (0);
}
