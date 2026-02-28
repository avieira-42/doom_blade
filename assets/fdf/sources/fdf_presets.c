/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_presets.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 13:48:56 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/05 13:02:20 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "fdf.h"

void	fdf_reset(t_vars *vars)
{
	vars->params.rx = 0.0f;
	vars->params.ry = 0.0f;
	vars->params.rz = 0.0f;
	vars->params.dx = 0.0f;
	vars->params.dy = 0.0f;
	vars->params.dz = 0.0f;
	vars->params.zoom = 0.5f;
	vars->params.zscale = 1.0f;
}

void	fdf_preset1(t_vars *vars)
{
	fdf_reset(vars);
	vars->params.rx = PI / 6;
	vars->params.ry = 0.0f;
	vars->params.rz = -PI / 4;
	fdf_render_frame(vars);
}

void	fdf_preset2(t_vars *vars)
{
	fdf_reset(vars);
	vars->params.rx = PI / 2;
	vars->params.ry = 0.0f;
	vars->params.rz = 0.0f;
	fdf_render_frame(vars);
}

void	fdf_preset3(t_vars *vars)
{
	fdf_reset(vars);
	vars->params.rx = 0.0f;
	vars->params.ry = PI / 2;
	vars->params.rz = 0.0f;
	fdf_render_frame(vars);
}

void	fdf_preset4(t_vars *vars)
{
	fdf_reset(vars);
	vars->params.rx = 0.0f;
	vars->params.ry = 0.0f;
	vars->params.rz = PI / 2;
	fdf_render_frame(vars);
}
