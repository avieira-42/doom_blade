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
	// game->cam.mouse_mov.x.f = (x1 - SCREEN_WIDTH / 2) / 100;	// TODO: adjust values for power of two divisions, e.g. 128
	// game->cam.mouse_mov.y.f += (y1 - SCREEN_HEIGHT / 2) * -1;
	// if (game->cam.mouse_mov.y.f <= -1000.)
	// 	game->cam.mouse_mov.y.f = -1000.;
	// if (game->cam.mouse_mov.y.f <= -1200.)
	// 	game->cam.mouse_mov.y.f = -1000.;
	// if (game->cam.mouse_mov.y.f >= 3500.)
	// 	game->cam.mouse_mov.y.f = 3500.;
	return (0);
}
