#include <X11/Xutil.h>
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
	if (keycode == XK_Shift_L)
		game->key |= (size_t)key_shift;
	if (keycode == XK_Control_L)
		game->key |= (size_t)key_ctrl;
	if (keycode == XK_r)
		game->key |= (size_t)key_r;
	//	TMP >>>
	if (keycode == XK_w)
		game->w = true;
	if (keycode == XK_a)
		game->a = true;
	if (keycode == XK_s)
		game->s = true;
	if (keycode == XK_d)
		game->d = true;
	if (keycode == XK_r)
		game->r = true;
	if (keycode == XK_p)
		game->p = true;
	if (keycode == XK_Shift_L)
		game->shift = true;
	//	<<< TMP
	return (0);
}

// TMP >>>
void	input_handler(t_game *game)
{
	if (game->shift == true)
	{
		game->audio.current_step = game->audio.step_fast;
		game->hud.walk->loops_per_sprite--;
	}
	else
	{
		game->audio.current_step = game->audio.step;
		game->hud.walk->loops_per_sprite++;
	}
	if (game->p == true)
	{
		if (game->pause == false)
			game->pause = true;
		else
			game->pause = false;
	}
	if (game->r == true && game->hud.hands_shoot == false)
	{
		if (game->hud.gun.ammo < game->hud.gun.max_ammo)
		{
			game->hud.hands_reload = true;
			game->hud.gun.first_iterator = -1;
			game->r = false;
		}
	}
	if (game->mouse_l == true)
	{
		game->hud.gun.ammo--;
		game->hud.hands_shoot = true;
		game->hud.shoot_sound = true;
		game->hud.hands_reload = false;
		game->mouse_l = false;
	}
}
// <<< TMP

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
	if (keycode == XK_Shift_L)
		game->key &= ~(size_t)key_shift;
	if (keycode == XK_Control_L)
		game->key &= ~(size_t)key_ctrl;
	if (keycode == XK_r)
		game->key &= ~(size_t)key_r;
	//	TMP >>>
	if (keycode == XK_w)
		game->w_up = true;
	if (keycode == XK_a)
		game->a_up = true;
	if (keycode == XK_s)
		game->s_up = true;
	if (keycode == XK_d)
		game->d_up = true;
	if (keycode == XK_r)
		game->r_up = true;
	if (keycode == XK_Shift_L)
		game->shift_up = true;
	//	<<< TMP
	return (0);
}
