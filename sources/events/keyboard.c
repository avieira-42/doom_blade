#include <X11/Xutil.h>
#include <X11/keysym.h>
#include "cub_structs.h"
#include "cub_utils.h"

// TODO: Make the functions take another smaller struct that only relates to keybinds 
// Keymap
// 2 KeyRelease
int	cmlx_keydown(int keycode, t_game *game)
{
	if (game->state.paused == false)
	{
		if (keycode == XK_Escape)
			return (cub_cleanup(game));
		if (keycode == XK_w)
			game->state.key |= (size_t)key_w;
		if (keycode == XK_a)
			game->state.key |= (size_t)key_a;
		if (keycode == XK_s)
			game->state.key |= (size_t)key_s;
		if (keycode == XK_d)
			game->state.key |= (size_t)key_d;
		if (keycode == XK_Shift_L)
		{
			game->assets.audio.current_step = game->assets.audio.step_fast;
			game->assets.walk.loops_per_sprite = 7;
			game->state.key |= (size_t)key_shift;
		}
		if (keycode == XK_Control_L)
			game->state.key |= (size_t)key_ctrl;
		if (keycode == XK_r)
		{
			game->state.key |= (size_t)key_r;
			if (game->assets.shoot.start == false && game->gun.ammo < game->gun.max_ammo)
			{
				game->assets.reload.start = true;
				game->gun.first_iterator = -1;
			}
			// TODO: This can't run on keydown, only fast changes
		}
	}
	if (keycode == XK_p)
	{
		game->state.key = (size_t)key_p;	// Clears other keys
		game->state.paused = true;
	}
	return (0);
}

// 3 KeyPress
int	cmlx_keyup(int keycode, t_game *game)
{
	if (keycode == XK_p)
	{
		game->state.key = 0;
		game->state.paused = false;
	}
	if (keycode == XK_w)
		game->state.key &= ~(size_t)key_w;
	if (keycode == XK_a)
		game->state.key &= ~(size_t)key_a;
	if (keycode == XK_s)
		game->state.key &= ~(size_t)key_s;
	if (keycode == XK_d)
		game->state.key &= ~(size_t)key_d;
	if (keycode == XK_Shift_L)
	{
		game->state.key &= ~(size_t)key_shift;
		game->assets.audio.current_step = game->assets.audio.step_fast;
		game->assets.walk.loops_per_sprite = 5;	
	}
	if (keycode == XK_Control_L)
		game->state.key &= ~(size_t)key_ctrl;
	if (keycode == XK_r)
		game->state.key &= ~(size_t)key_r;
	return (0);
}
