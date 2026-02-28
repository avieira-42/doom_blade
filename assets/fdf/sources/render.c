/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:56:46 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/05 13:16:32 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <math.h>
#include "fdf_utils.h"
#include "fdf.h"

static
void	apply_transform(t_vars *vars, t_vec4 *v, t_vtx *vt, t_params params)
{
	size_t			i;
	float			tmp_z;
	const t_vec3	vcos = {cosf(params.rx), cosf(params.ry), cosf(params.rz)};
	const t_vec3	vsin = {sinf(params.rx), sinf(params.ry), sinf(params.rz)};
	const t_mat4	m = {{
	{vcos.z * vcos.y, -vsin.z * vcos.y, vsin.y, params.dx},
	{vcos.z * vsin.y * vsin.x + vsin.z * vcos.x, -vsin.z * vsin.y * vsin.x
		+ vcos.z * vcos.x, -vcos.y * vsin.x, params.dy},
	{-vcos.z * vsin.y * vcos.x + vsin.z * vsin.x, vsin.z * vsin.y * vcos.x
		+ vcos.z * vsin.x, vcos.y * vcos.x, params.dz},
	{0, 0, 0, 1}}};

	i = 0;
	while (i < vars->length)
	{
		tmp_z = vt[i].zf * vars->params.zscale;
		v[i].x = m.a1 * vt[i].xf + m.a2 * vt[i].yf + m.a3 * tmp_z + m.a4;
		v[i].y = m.b1 * vt[i].xf + m.b2 * vt[i].yf + m.b3 * tmp_z + m.b4;
		v[i].z = m.c1 * vt[i].xf + m.c2 * vt[i].yf + m.c3 * tmp_z + m.c4;
		i++;
	}
}

void	fdf_render_frame(t_vars *vars)
{
	size_t	row;
	size_t	col;

	ft_bzero(vars->img->data, HEIGHT * WIDTH * sizeof(int32_t));
	apply_transform(vars, vars->vec, vars->vtx, vars->params);
	row = 0;
	while (row < vars->rows)
	{
		col = 0;
		while (col < vars->cols)
		{
			draw_neighbours(vars, row, col);
			col++;
		}
		row++;
	}
	mlx_put_image_to_window(vars->mlx, vars->mlx->win_list, vars->img, 0, 0);
}
