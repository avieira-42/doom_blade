#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_types.h"

float	vec2_dot_product(t_vec2 a, t_vec2 b)
{
	return (a.x.f * b.x.f + a.y.f * b.y.f);
}

// Angle in radians
t_vec2	vec2_rotate(t_vec2 vec, float angle)
{
	const float	xdir = vec.x.f;
	const float	ydir = vec.y.f;

	vec.x.f = xdir * cosf(angle) - ydir * sinf(angle);
	vec.y.f = xdir * sinf(angle) + ydir * cosf(angle);
	return (vec);
}

t_vec2	vec2_norm(t_vec2 vec)
{
	float	mag;
	t_32	inv_sqrt;

	mag = vec.x.f * vec.x.f + vec.y.f * vec.y.f;
	inv_sqrt.f = mag;
	inv_sqrt.u = 0x5f3759df - (inv_sqrt.u >> 1);
	inv_sqrt.f = inv_sqrt.f * (1.5f - (mag * 0.5f * inv_sqrt.f * inv_sqrt.f));
	vec.x.f *= inv_sqrt.f;
	vec.y.f *= inv_sqrt.f;
	return (vec);
}

t_vec2	vec2_sub(t_vec2 a, t_vec2 b)
{
	return ((t_vec2){.x = {.i = b.x.i - a.x.i}, .y = {.i = b.y.i - a.y.i}});
}

int32_t	vec2_mag_sqd(t_vec2 vec)
{
	return (vec.x.i * vec.x.i + vec.y.i * vec.y.i );
}

int32_t	vec2_dist(t_vec2 a, t_vec2 b)
{
	return (vec2_mag_sqd(vec2_sub(a, b)));
}
