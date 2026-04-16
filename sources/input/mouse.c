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

int	cmlx_mousemove(Sint32 x, Sint32 y, t_game *game)
{
	float		dx;
	float		dy;

	dx = (float)(x - ((float)s_width / 2.f)) *(1.0f / 512.0f);
	dy = ((float)y - ((float)s_height / 2.f));
	game->player.cam.dir = vec2_rotate(game->player.cam.dir, dx);
	game->player.cam.plane = vec2_rotate(game->player.cam.plane, dx);
	game->frame.offset = ft_iclamp
		(game->frame.offset + dy, -OFFSET_MAX, OFFSET_MAX);
	SDL_WarpMouseInWindow(game->window, s_width / 2, s_height / 2);
	return (0);
}
