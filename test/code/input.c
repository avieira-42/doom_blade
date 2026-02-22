#include <X11/keysym.h>
#include "types.h"
#include "clean.h"

int     key_pressed(int keysym, void *arg)
{
	t_game  *game;

	game = (t_game *)arg;
	if (keysym == XK_w)
		game->player.ori.y += -1;
	if (keysym == XK_a)
		game->player.ori.x += -1;
	if (keysym == XK_s)
		game->player.ori.y += 1;
	if (keysym == XK_d)
		game->player.ori.x += 1;
	if (keysym == XK_l)
		game->player.cam.dir += -1;
	if (keysym == XK_j)
		game->player.cam.dir += 1;
	if (keysym == XK_Escape)
		free_displays();
	return (1);
}

int     key_released(int keysym, void *arg)
{
	t_game  *game;

	game = (t_game *)arg;
	if (keysym == XK_w)
		game->player.ori.y += 1;
	if (keysym == XK_a)
		game->player.ori.x += 1;
	if (keysym == XK_s)
		game->player.ori.y += -1;
	if (keysym == XK_d)
		game->player.ori.x += -1;
	if (keysym == XK_l)
		game->player.cam.dir += 1;
	if (keysym == XK_j)
		game->player.cam.dir += -1;
	return (1);
}
