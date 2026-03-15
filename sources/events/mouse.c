#include <X11/keysym.h>
#include "cub_structs.h"
#include "cub_utils.h"

// 4 ButtonPress
// 12345 = LMB MMB RMB WHEELUP WHEELDOWN
int	cmlx_mousedown(int button, int32_t x, int32_t y, t_game *game)
{
	return (0);
}

// 5 ButtonRelease
int	cmlx_mouseup(int button, int32_t x, int32_t y, t_game *game)
{
	return (0);
}

// 6 MotionNotify
int	cmlx_mousemove(int32_t x1, int32_t y1, t_game *game)
{
	static int32_t	x0 = 0;
	static int32_t	y0 = 0;
	float			dx;

	dx = (float)(x1 - x0) * game->cfg.sens;
	game->player.cam.dir = vec2_rotate(game->player.cam.dir, dx);
	game->player.cam.plane = vec2_rotate(game->player.cam.plane, dx);
	x0 = x1;
	y0 = y1;
	return (0);
}
