/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mvpmatrix.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 12:55:20 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/05 12:56:54 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef struct s_frustrum
{
	float	near;	// Near z plane camera
	float	far;	// Far z plane camera
	float	fov;	// Field of view
	float	ar;		// Aspect Ratio
}	t_frustrum;

t_mat4	build_mvpmatrix(t_params p, t_frustrum f)
{
	const t_vec3	vc = {cosf(p.rx), cosf(p.ry), cosf(p.rz)};
	const t_vec3	vs = {sinf(p.rx), sinf(p.ry), sinf(p.rz)};
	const t_vec4	v1 = {1.0f / tanf(f.fov * 0.5f), -vc.z * vs.y * vc.x + vs.z
		* vs.x, vs.z * vs.y * vc.x + vc.z * vs.x, vc.y * vc.x};
	const t_vec4	v2 = {v1.x / f.ar, v1.x, (f.far + f.near)
		/ (f.near - f.far), 2.0f * f.far * f.near / (f.near - f.far)};

	return ((t_mat4){{
		{v2.x * vc.z * vc.y, v2.x * -vs.z * vc.y, v2.x * vs.y, v2.x * p.dx},
		{v2.y * (vc.z * vs.y * vs.x + vs.z * vc.x), v2.y * (-vs.z * vs.y * vs.x + vc.z * vc.x), v2.y * (-vc.y * vs.x), v2.y * p.dy},
		{v2.z * v1.y, v2.z * v1.z, v2.z * v1.w, v2.z * p.dz + v2.w},
		{-v1.y, -v1.z, -v1.w, -p.dz}}});
}