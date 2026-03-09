#include <X11/keysym.h>
#include "cub_structs.h"
#include "cub_utils.h"

// TODO: Make the functions take another smaller struct that only relates to keybinds 
// Keymap
// 2 KeyRelease
int	cmlx_keydown(int keycode, t_game *game)
{
	if (keycode == XK_Escape)
		return (cub_cleanup(game));
	// if (keycode == XK_w)
	// 	game->player.ori.y.f += 1.0f;
	// if (keycode == XK_a)
	// 	game->player.ori.x.f += -1.0f;
	// if (keycode == XK_s)
	// 	game->player.ori.y.f += -1.0f;
	// if (keycode == XK_d)
	// 	game->player.ori.x.f += 1.0f;
	if (keycode == XK_l)
		game->player.dir_mod += -1;
	if (keycode == XK_j)
		game->player.dir_mod += 1;
	// if (keycode == XK_k)
	// 	game->cam.dist_mod += -1;
	// if (keycode == XK_i)
	// 	game->cam.dist_mod += 1;
	if (keycode == XK_Shift_L)
		game->player.speed_mod += 1;
	// if (keycode == XK_Escape)
	// 	free_displays();
	return (0);
}

// 3 KeyPress
int	cmlx_keyup(int keycode, t_game *game)
{
	if (keycode == XK_w)
		game->player.cam.pos.y.f += -1.0f;
	if (keycode == XK_a)
		game->player.cam.pos.x.f += 1.0f;
	if (keycode == XK_s)
		game->player.cam.pos.y.f += 1.0f;
	if (keycode == XK_d)
		game->player.cam.pos.x.f += -1.0f;
	if (keycode == XK_l)
		game->player.dir_mod += 1;
	if (keycode == XK_j)
		game->player.dir_mod += -1;
	// if (keycode == XK_k)
	// 	game->cam.dist_mod += 1;
	// if (keycode == XK_i)
	// 	game->cam.dist_mod += -1;
	if (keycode == XK_Shift_L)
		game->player.speed_mod += -1.0f;
	return (0);
}

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
