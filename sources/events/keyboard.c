#include <X11/Xutil.h>
#include <X11/keysym.h>
#include "cub_structs.h"
#include "cub_utils.h"

// TODO: Make the functions take another smaller struct that only relates to keybinds 
// Keymap
// 2 KeyRelease
int	cmlx_keydown(int keycode, t_game *game)
{
<<<<<<< HEAD
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
		game->state.key |= (size_t)key_shift;
	if (keycode == XK_Control_L)
		game->state.key |= (size_t)key_ctrl;
=======
	if (game->pause == false)
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
		if (keycode == XK_Shift_L)
		{
			game->key |= (size_t)key_shift;
			game->key_down |= (size_t)key_shift;
		}
		if (keycode == XK_Control_L)
			game->key |= (size_t)key_ctrl;
		if (keycode == XK_r)
			game->key |= (size_t)key_r;
	}
	if (keycode == XK_p)
		game->key |= (size_t)key_p;
>>>>>>> soeiro
	return (0);
}

// TMP >>>
void	input_handler(t_game *game)
{
	if (game->key & key_p)
	{
		if (game->pause == false)
			game->pause = true;
		else
		{
			game->pause = false;
			game->key = 0;
			game->key_up = 0;
			game->key_down = 0;
		}
		game->key &= ~key_p;
		return ;
	}
	if (game->key_down & key_shift)
	{
		game->audio.current_step = game->audio.step_fast;
		game->assets.walk.loops_per_sprite += -2;
		game->key_down &= ~key_shift;
	}
	if (game->key_up & key_shift)
	{
		game->audio.current_step = game->audio.step;
		game->assets.walk.loops_per_sprite += 2;
		game->key_up &= ~key_shift;
	}
	if (game->key & key_r && game->assets.shoot.start == false)
	{
		if (game->gun.ammo < game->gun.max_ammo)
		{
			game->assets.reload.start = true;
			game->gun.first_iterator = -1;
			game->key &= ~key_r;
		}
	}
	if (game->mouse_l == true && game->assets.shoot.start == false)
	{
		if (game->gun.ammo == 0)
			Mix_PlayChannel(3, game->audio.no_ammo, 0);
		else
		{
			game->gun.ammo--;
			game->assets.shoot.start = true;
			game->assets.shoot.sound = true;
			game->assets.reload.start = false;
		}
		game->mouse_l = false;
	}
}
// <<< TMP

// 3 KeyPress
int	cmlx_keyup(int keycode, t_game *game)
{
<<<<<<< HEAD
	if (keycode == XK_w)
		game->state.key &= ~(size_t)key_w;
	if (keycode == XK_a)
		game->state.key &= ~(size_t)key_a;
	if (keycode == XK_s)
		game->state.key &= ~(size_t)key_s;
	if (keycode == XK_d)
		game->state.key &= ~(size_t)key_d;
	if (keycode == XK_Shift_L)
		game->state.key &= ~(size_t)key_shift;
	if (keycode == XK_Control_L)
		game->state.key &= ~(size_t)key_ctrl;
=======
	if (game->pause == false)
	{
		if (keycode == XK_w)
			game->key &= ~(size_t)key_w;
		if (keycode == XK_a)
			game->key &= ~(size_t)key_a;
		if (keycode == XK_s)
			game->key &= ~(size_t)key_s;
		if (keycode == XK_d)
			game->key &= ~(size_t)key_d;
		if (keycode == XK_Shift_L)
		{
			game->key &= ~(size_t)key_shift;
			game->key_up |= key_shift;
		}
		if (keycode == XK_Control_L)
			game->key &= ~(size_t)key_ctrl;
		if (keycode == XK_r)
			game->key &= ~(size_t)key_r;
	}
>>>>>>> soeiro
	return (0);
}
