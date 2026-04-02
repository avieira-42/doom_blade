#include <X11/Xutil.h>
#include <X11/keysym.h>
#include "cub_structs.h"
#include "cub_utils.h"

int	mlx_mouse_get_pos(void *mlx, void *win_ptr, int *x, int *y);

// TODO: Make the functions take another smaller struct that only relates to keybinds 
// Keymap
// 2 KeyRelease
int	cmlx_keydown(int keycode, t_game *game)
{
	if (keycode == XK_Escape)
		return (mlx_loop_end(game->mlx));
	if (keycode == XK_p)
		game->state.paused ^= 1;
	if (game->state.paused == true)
		return (0);
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
	if (keycode == XK_e && !(game->player.state & (st_reloading | st_shooting)))
		game->player.state = st_interacting;
	if (keycode == XK_r && !(game->player.state & (st_reloading | st_shooting)) && game->player.ammo < AMMO_COUNT)
	{
		game->player.state = (st_reloading);
		game->drawbuf.hands = game->assets.reload;
		game->drawbuf.hands.index = game->player.ammo * RELOAD_CYCLE;
		game->state.key |= (size_t)key_r;
		game->player.map &= ~(size_t)st_raising;
		game->player.map &= ~(size_t)st_checking;
	}
	if (keycode == XK_Tab)
		game->state.key |= (size_t)key_tab;
	if (keycode == XK_Left)
	{
		game->player.cam.dir = vec2_rotate(game->player.cam.dir, -PI_DEG);
		game->player.cam.plane = vec2_rotate(game->player.cam.plane, -PI_DEG);
	}
	if (keycode == XK_Right)
	{
		game->player.cam.dir = vec2_rotate(game->player.cam.dir, PI_DEG);
		game->player.cam.plane = vec2_rotate(game->player.cam.plane, PI_DEG);
	}
	return (0);
}

// 3 KeyPress
int	cmlx_keyup(int keycode, t_game *game)
{
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
	if (keycode == XK_r)
		game->state.key &= ~(size_t)key_r;
	if (keycode == XK_Tab)
		game->state.key &= ~(size_t)key_tab;
	return (0);
}

// 4 ButtonPress
// 12345 = LMB MMB RMB WHEELUP WHEELDOWN
// TODO: The X Y could be relevant for hitreg
int	cmlx_mousedown(int button, int32_t x, int32_t y, t_game *game)
{
	(void) x;
	(void) y;
	if (button == 1 && game->player.state != st_shooting)
	{
		if (game->player.ammo == 0)
			Mix_PlayChannel(ch_no_ammo, game->assets.audio.no_ammo, 0);	// This should be done outside here
		else
		{
			game->player.ammo--;
			game->drawbuf.hands = game->assets.shoot;
			game->player.state = (st_shooting | st_shot);
		}
	}
	return (0);
}

int	cmlx_mousemove(t_game *game)
{
	int32_t	x1;
	int32_t	y1;
	float	dx;
	float	dy;
	float	offset;

	mlx_mouse_get_pos(game->mlx, game->mlx->win_list, &x1, &y1);
	dx = (float)(x1 - ((float)SCREEN_WIDTH / 2)) * (1.0f / 512.0f);
	dy = (float)(y1 - ((float)SCREEN_HEIGHT / 2));
	game->player.cam.dir = vec2_rotate(game->player.cam.dir, dx);
	game->player.cam.plane = vec2_rotate(game->player.cam.plane, dx);
	offset = game->frame.offset;
	offset += dy;
	offset = ft_absclamp(offset, OFFSET_MAX);
	game->frame.offset = offset;
	mlx_mouse_move(game->mlx, game->mlx->win_list, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	return (0);
}
