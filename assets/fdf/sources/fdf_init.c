/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 13:13:44 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/04 10:45:30 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include "fdf.h"

static
void	fdf_hooks(t_vars *vars)
{
	t_win_list	*window;

	window = vars->mlx->win_list;
	mlx_hook(window, KeyPress, KeyPressMask, cmlx_keydown, vars);
	mlx_hook(window, KeyRelease, KeyReleaseMask, cmlx_keyup, vars);
	mlx_hook(window, ButtonPress, ButtonPressMask, cmlx_mousedown, vars);
	mlx_hook(window, ButtonRelease, ButtonReleaseMask, cmlx_mouseup, vars);
	mlx_hook(window, MotionNotify, PointerMotionMask, cmlx_mousemove, vars);
	mlx_loop_hook(vars->mlx, cmlx_loop, vars);
}

static
uint8_t	cmlx_error(t_vars *vars, uint8_t error_code)
{
	write(2, "Error: MLX Failure\n", 19);
	free(vars->vtx);
	vars->vtx = NULL;
	if (error_code == 1)
	{
		mlx_destroy_display(vars->mlx);
		free(vars->mlx);
	}
	else if (error_code == 2)
	{
		mlx_destroy_image(vars->mlx, vars->img);
		mlx_destroy_display(vars->mlx);
		free(vars->mlx);
	}
	vars->mlx = NULL;
	vars->img = NULL;
	return (1);
}

static
void	fdf_init_params(t_vars *vars)
{
	fdf_reset(vars);
	vars->keys.w = 0;
	vars->keys.a = 0;
	vars->keys.s = 0;
	vars->keys.d = 0;
	vars->keys.q = 0;
	vars->keys.e = 0;
	vars->keys.up = 0;
	vars->keys.down = 0;
	vars->keys.left = 0;
	vars->keys.right = 0;
	vars->keys.rmb = 0;
	vars->keys.lmb = 0;
}

static
void	fdf_normalize(t_vars *vars)
{
	size_t		i;
	const float	invx = 2.0f / (vars->cols - 1);
	const float	invy = 2.0f / (vars->rows - 1);
	const float	invz = 2.0f / (vars->max - vars->min + 1);

	i = 0;
	while (i < vars->length)
	{
		vars->vtx[i].xf = vars->vtx[i].x * invx - 1.0f;
		vars->vtx[i].yf = vars->vtx[i].y * invy - 1.0f;
		vars->vtx[i].zf = (vars->vtx[i].z - vars->min) * invz - 1.0f;
		i++;
	}
}

uint8_t	fdf_init(t_vars *vars, const char *filename, const char *charset)
{
	if (fdf_read(vars, filename, charset))
		return (1);
	vars->mlx = mlx_init();
	if (vars->mlx == NULL)
		return (cmlx_error(vars, 0));
	vars->img = mlx_int_new_image(vars->mlx, WIDTH, HEIGHT, ZPixmap);
	if (vars->img == NULL)
		return (cmlx_error(vars, 1));
	mlx_new_window(vars->mlx, WIDTH, HEIGHT, "Fildefer");
	if (vars->mlx->win_list == NULL)
		return (cmlx_error(vars, 2));
	fdf_hooks(vars);
	fdf_init_params(vars);
	fdf_normalize(vars);
	mlx_loop(vars->mlx);
	return (0);
}
