#ifndef LIBFT_MATH_H
#define LIBFT_MATH_H

#include <stdint.h>

typedef struct	s_vec
{
	float	x;
	float	y;
}	t_vec;

typedef union	u_f32
{
	float	float32;
	int32_t	int32;
}	t_f32;

int		vec_max_coord(t_vec vec);
float	ft_abs(float n);
t_vec	vec_add(t_vec a, t_vec b);
t_vec	vec_sub(t_vec a, t_vec b);
float	vec_mag_sqd(t_vec vec);
float	vec_inv_mag(t_vec vec);
float	q_rsqrt(float n);
float	u_rsqrt(float n);

#endif
