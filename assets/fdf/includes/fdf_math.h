/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_math.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:51:22 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/05 12:36:18 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_MATH_H
# define FDF_MATH_H

# include <stdint.h>
# include <stddef.h>

# ifndef EPS
#  define EPS 0.00000011920929f
# endif

# define PI 3.141592653589793238462643383279502884L

typedef struct u_delta
{
	float		x;
	float		y;
	uint32_t	color;
}	t_delta;

typedef union u_vertex
{
	struct
	{
		int32_t		x;
		int32_t		y;
		int32_t		z;
		uint32_t	color;
	};
	struct
	{
		float		xf;
		float		yf;
		float		zf;
		float		wf;
	};
}	t_vtx;

typedef union u_vec2
{
	struct
	{
		float		x;
		float		y;
	};
	struct
	{
		int32_t		ix;
		int32_t		iy;
	};
	struct
	{
		uint32_t	ux;
		uint32_t	uy;
	};
	double			f64;
	uint64_t		u64;
}	t_vec2;

typedef struct s_vec3
{
	float	x;
	float	y;
	float	z;
}	t_vec3;

typedef struct t_vec4
{
	float	x;	// Abscissa
	float	y;	// Ordinate
	float	z;	// Altitude
	float	w;	// Normalization Vector
}	t_vec4;

typedef union u_mat4
{
	t_vec4		row[4];
	float		m[4][4];
	struct
	{
		float	a1;
		float	a2;
		float	a3;
		float	a4;
		float	b1;
		float	b2;
		float	b3;
		float	b4;
		float	c1;
		float	c2;
		float	c3;
		float	c4;
		float	d1;
		float	d2;
		float	d3;
		float	d4;
	};
}	t_mat4;

#endif
