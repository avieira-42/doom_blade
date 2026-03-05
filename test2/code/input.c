#include <X11/keysym.h>
#include "types.h"
#include "clean.h"

int     key_pressed(int keysym, void *arg)
{
	t_game  *game;

	game = (t_game *)arg;
	if (keysym == XK_w)
		game->player.ori.y += 1;
	if (keysym == XK_a)
		game->player.ori.x += -1;
	if (keysym == XK_s)
		game->player.ori.y += -1;
	if (keysym == XK_d)
		game->player.ori.x += 1;
	if (keysym == XK_l)
		game->player.dir_mod += -1;
	if (keysym == XK_j)
		game->player.dir_mod += 1;
	if (keysym == XK_k)
		game->cam.dist_mod += -1;
	if (keysym == XK_i)
		game->cam.dist_mod += 1;
	if (keysym == XK_Shift_L)
		game->player.speed_mod += 1;
	if (keysym == XK_Escape)
		free_displays();
	return (1);
}

int     key_released(int keysym, void *arg)
{
	t_game  *game;

	game = (t_game *)arg;
	if (keysym == XK_w)
		game->player.ori.y += -1;
	if (keysym == XK_a)
		game->player.ori.x += 1;
	if (keysym == XK_s)
		game->player.ori.y += 1;
	if (keysym == XK_d)
		game->player.ori.x += -1;
	if (keysym == XK_l)
		game->player.dir_mod += 1;
	if (keysym == XK_j)
		game->player.dir_mod += -1;
	if (keysym == XK_k)
		game->cam.dist_mod += 1;
	if (keysym == XK_i)
		game->cam.dist_mod += -1;
	if (keysym == XK_Shift_L)
		game->player.speed_mod += -1;
	return (1);
}

int	mouse_pressed(int keysym, void *arg)
{
	t_game  *game;

	game = (t_game *)arg;
	(void)game;
	(void)keysym;
	(void)arg;
	return (1);
}

int	mouse_released(int keysym, void *arg)
{
	t_game  *game;

	game = (t_game *)arg;
	(void)game;
	(void)keysym;
	(void)arg;
	return (1);
}

int	mouse_move(int x, int y, void *arg)
{
	t_game	*game;

	game = (t_game *) arg;
	game->player.mouse_mov.x = (x - SCREEN_X / 2) / 100;
	game->player.mouse_mov.y += (y - SCREEN_Y / 2) * -1;
	if (game->player.mouse_mov.y <= -1000.)
		game->player.mouse_mov.y = -1000.;
	if (game->player.mouse_mov.y <= -1200.)
		game->player.mouse_mov.y = -1000.;
	if (game->player.mouse_mov.y >= 3500.)
		game->player.mouse_mov.y = 3500.;
	return (1);
}
