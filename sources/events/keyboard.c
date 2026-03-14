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
	if (keycode == XK_w)
		game->key |= (size_t)key_w;
	if (keycode == XK_a)
		game->key |= (size_t)key_a;
	if (keycode == XK_s)
		game->key |= (size_t)key_s;
	if (keycode == XK_d)
		game->key |= (size_t)key_d;
	return (0);
}

// 3 KeyPress
int	cmlx_keyup(int keycode, t_game *game)
{
	if (keycode == XK_w)
		game->key &= ~(size_t)key_w;
	if (keycode == XK_a)
		game->key &= ~(size_t)key_a;
	if (keycode == XK_s)
		game->key &= ~(size_t)key_s;
	if (keycode == XK_d)
		game->key &= ~(size_t)key_d;
	return (0);
}
