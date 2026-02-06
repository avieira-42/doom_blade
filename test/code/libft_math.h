#ifndef LIBFT_MATH_H
#define LIBFT_MATH_H

#include <stdint.h>

typedef struct	s_vecf
{
	float	x;
	float	y;
}	t_vecf;

typedef struct	s_veci
{
	int	x;
	int	y;
}	t_veci;

typedef union	u_f32
{
	float	float32;
	int32_t	int32;
}	t_f32;

int		vec_max_coord(t_vecf vec);
float	ft_abs(float n);
t_vecf	vec_add(t_vecf a, t_vecf b);
t_vecf	vec_sub(t_vecf a, t_vecf b);
float	vec_mag_sqd(t_vecf vec);
float	vec_inv_mag(t_vecf vec);
float	q_rsqrt(float n);
float	u_rsqrt(float n);

#endif
