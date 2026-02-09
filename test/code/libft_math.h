#ifndef LIBFT_MATH_H
#define LIBFT_MATH_H

#include <stdint.h>

typedef struct	s_vecf32
{
	float	x;
	float	y;
}	t_vecf32;

typedef struct	s_veci32
{
	int32_t	x;
	int32_t	y;
}	t_veci32;

typedef union	u_f32
{
	float	float32;
	int32_t	int32;
}	t_f32;

int			vec_max_coord(t_vecf32 vec);
float		ft_abs(float n);
float		vec_dot_product(t_vecf32 a, t_vecf32 b);
float		vec_mag_sqd(t_vecf32 vec);
float		vec_inv_mag(t_vecf32 vec);
float		q_rsqrt(float n);
float		u_rsqrt(float n);
float		vec_scalar_projec(t_vecf32 a, t_vecf32 b);
float		vec_cosin();
float		vec_sin();
float		vec_tan();
t_vecf32	vec_prepend_dir(t_vecf32 a);
t_vecf32	vec_proj(t_vecf32 a, t_vecf32 b);
t_vecf32	vec_sum(t_vecf32 a, t_vecf32 b);
t_vecf32	vec_sub(t_vecf32 a, t_vecf32 b);
t_vecf32	vec_unit(t_vecf32 vec);
t_vecf32	vec_dir(t_vecf32 a, t_vecf32 b);
t_vecf32	vec_scalar_mult(t_vecf32 unit, float scalar);

#endif
