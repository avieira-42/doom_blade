/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 09:34:26 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/05 13:27:22 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include "fdf.h"
#include "fdf_utils.h"

// Keymap
// 2 KeyRelease
int	cmlx_keydown(int keycode, t_vars *vars)
{
	if (keycode == XK_Escape)
		return (mlx_loop_end(vars->mlx));
	if (keycode == XK_r)
		fdf_reset(vars);
	if (keycode == XK_1)
		fdf_preset1(vars);
	if (keycode == XK_2)
		fdf_preset2(vars);
	if (keycode == XK_3)
		fdf_preset3(vars);
	if (keycode == XK_4)
		fdf_preset4(vars);
	vars->keys.w = (keycode == XK_w);
	vars->keys.a = (keycode == XK_a);
	vars->keys.s = (keycode == XK_s);
	vars->keys.d = (keycode == XK_d);
	vars->keys.q = (keycode == XK_q);
	vars->keys.e = (keycode == XK_e);
	vars->keys.up = (keycode == XK_Up);
	vars->keys.down = (keycode == XK_Down);
	vars->keys.left = (keycode == XK_Left);
	vars->keys.right = (keycode == XK_Right);
	vars->keys.shift = (keycode == XK_Shift_L);
	return (0);
}

// 3 KeyPress
int	cmlx_keyup(int keycode, t_vars *vars)
{
	vars->keys.w &= (keycode != XK_w);
	vars->keys.a &= (keycode != XK_a);
	vars->keys.s &= (keycode != XK_s);
	vars->keys.d &= (keycode != XK_d);
	vars->keys.up &= (keycode != XK_Up);
	vars->keys.down &= (keycode != XK_Down);
	vars->keys.left &= (keycode != XK_Left);
	vars->keys.right &= (keycode != XK_Right);
	vars->keys.q &= (keycode != XK_q);
	vars->keys.e &= (keycode != XK_e);
	vars->keys.shift &= (keycode != XK_Shift_L);
	return (0);
}

// 4 ButtonPress
// 12345 = LMB MMB RMB WHEELUP WHEELDOWN
int	cmlx_mousedown(int button, int32_t x, int32_t y, t_vars *vars)
{
	vars->keys.lmb = (button == 1);
	vars->keys.rmb = (button == 3);
	if (vars->keys.shift == 0)
	{
		if (button == 4)
			vars->params.zoom += 0.1f;
		else if (button == 5)
			vars->params.zoom = ft_max(vars->params.zoom - 0.1f, 0.1f);
	}
	else
	{
		if (button == 4)
			vars->params.zscale += 0.1f;
		else if (button == 5)
			vars->params.zscale = ft_max(vars->params.zscale - 0.1f, 0.1f);
	}
	(void) x;
	(void) y;
	return (0);
}

// 5 ButtonRelease
int	cmlx_mouseup(int button, int32_t x, int32_t y, t_vars *vars)
{
	vars->keys.lmb &= (button != 1);
	vars->keys.rmb &= (button != 3);
	(void) x;
	(void) y;
	return (0);
}

// 6 MotionNotify
int	cmlx_mousemove(int32_t x1, int32_t y1, t_vars *vars)
{
	static int32_t	x0 = 0;
	static int32_t	y0 = 0;

	if (vars->keys.lmb != 0)
	{
		vars->params.dx += (float)(x1 - x0) / 256.0f;
		vars->params.dy += (float)(y1 - y0) / 256.0f;
	}
	else if (vars->keys.rmb != 0)
	{
		vars->params.rz += -(float)(x1 - x0) / 256.0f;
		vars->params.rx += -(float)(y1 - y0) / 256.0f;
	}
	x0 = x1;
	y0 = y1;
	return (0);
}
