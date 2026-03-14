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

t_vec2	vec2_unit(t_vec2 vec)
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
