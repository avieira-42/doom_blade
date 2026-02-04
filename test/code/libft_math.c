#include "libft_math.h"

t_vec	vec_add(t_vec a, t_vec b)
{
	return ((t_vec){a.x + b.x, a.y + b.y});
}

t_vec	vec_sub(t_vec a, t_vec b)
{
	return ((t_vec){b.x - a.x, b.y - a.y});
}

float	vec_mag_sqd(t_vec vec)
{
	return (vec.x * vec.x + vec.y * vec.y);
}

float	q_rsqrt(float n)
{
	int32_t		i;
	float		x2;
	float		y;
	const float	three_halfs = 1.5F;

	x2 = n * 0.5f;
	y = n;
	i = * (int32_t *) &y;
	i = 0x5f3759df - (i >> 1);
	y = * (float *) &i;
	y = y * (three_halfs - (x2 * y * y));
	return (y);
}

float	u_rsqrt(float n)
{
	t_f32		f;
	float		x2;
	const float	three_halfs = 1.5F;

	x2 = n * 0.5f;
	f.float32 = n;
	f.int32 = 0x5f3759df - (f.int32 >> 1);
	return (f.float32 * (three_halfs - (x2 * f.float32 * f.float32)));
}

float	vec_inv_mag(t_vec vec)
{
	return (q_rsqrt(vec_mag_sqd(vec)));
}

t_vec	vec_unit(t_vec vec)
{
	float	mag_sqd;
	float	inv_mag;

	mag_sqd = vec_mag_sqd(vec);
	if (mag_sqd < 1e-10f)
		return ((t_vec){0, 0});
	inv_mag = q_rsqrt(mag_sqd);
	return ((t_vec){vec.x * inv_mag, vec.y * inv_mag});
}

void	vec_dot_product()
{
}

void	vec_cosin()
{
}

void	vec_sin()
{
}

void	vec_tan()
{
}
