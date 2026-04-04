#include <X11/Xutil.h>
#include <X11/keysym.h>
#include "cub_structs.h"
#include "cub_utils.h"

static inline
void	stt_keydown(int keycode, t_game *game)
{
	if (keycode == XK_Shift_L)
		game->state.key |= (size_t)key_shift;
	if (keycode == XK_Control_L)
		game->state.key |= (size_t)key_ctrl;
	if (keycode == XK_Left)
		game->state.key |= (size_t)key_left;
	if (keycode == XK_Right)
		game->state.key |= (size_t)key_right;
}

// TODO: Make the functions take another smaller struct that only relates to keybinds 
// Keymap
// 2 KeyRelease
int	cmlx_keydown(int keycode, t_game *game)
{
	if (keycode == XK_Escape)
		return (mlx_loop_end(game->mlx));
	game->state.paused ^= (keycode == XK_p);
	if (game->state.paused == true)
		return (0);
	if (keycode == XK_w || keycode == XK_Up)
		game->state.key |= (size_t)key_w;
	if (keycode == XK_a)
		game->state.key |= (size_t)key_a;
	if (keycode == XK_s || keycode == XK_Down)
		game->state.key |= (size_t)key_s;
	if (keycode == XK_d)
		game->state.key |= (size_t)key_d;
	if (keycode == XK_r)
		game->state.key |= (size_t)key_r;
	if (keycode == XK_e)
		game->state.key |= (size_t)key_e;
	if (keycode == XK_Tab)
		game->state.key |= (size_t)key_tab;
	stt_keydown(keycode, game);
	return (0);
}

// 3 KeyPress
int	cmlx_keyup(int keycode, t_game *game)
{
	if (keycode == XK_w || keycode == XK_Up)
		game->state.key &= ~(size_t)key_w;
	if (keycode == XK_a)
		game->state.key &= ~(size_t)key_a;
	if (keycode == XK_s || keycode == XK_Down)
		game->state.key &= ~(size_t)key_s;
	if (keycode == XK_d)
		game->state.key &= ~(size_t)key_d;
	if (keycode == XK_Shift_L)
		game->state.key &= ~(size_t)key_shift;
	if (keycode == XK_Control_L)
		game->state.key &= ~(size_t)key_ctrl;
	if (keycode == XK_r)
		game->state.key &= ~(size_t)key_r;
	if (keycode == XK_e)
		game->state.key &= ~(size_t)key_e;
	if (keycode == XK_Tab)
		game->state.key &= ~(size_t)key_tab;
	if (keycode == XK_Left)
		game->state.key &= ~(size_t)key_left;
	if (keycode == XK_Right)
		game->state.key &= ~(size_t)key_right;
	return (0);
}
