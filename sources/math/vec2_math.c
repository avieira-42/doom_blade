#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cmlx_base.h"

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

float	vec2_dot(t_vec2 a, t_vec2 b)
{
	return (a.x.f * b.x.f + a.y.f * b.y.f);
}

int32_t	vec2_idist(t_vec2 a, t_vec2 b)
{
	int32_t	dist;
	t_vec2	delta;

	delta.x.i = a.x.i - b.x.i;
	delta.y.i = a.y.i - b.y.i;
	dist = delta.x.i * delta.x.i + delta.y.i * delta.y.i;
	return (dist);
}

t_vec2	vec2_sum(t_vec2 a, t_vec2 b)
{
	return ((t_vec2){.x.f = a.x.f + b.x.f, .y.f = a.y.f + b.y.f});
}

t_vec2	vec2_mult(t_vec2 a, float scalar)
{
	return ((t_vec2){.x.f = a.x.f * scalar, .y.f = a.y.f * scalar});
}

t_vec2	vec2_sub(t_vec2 a, t_vec2 b)
{
	return ((t_vec2){.x.f = b.x.f - a.x.f, .y.f = b.y.f - a.y.f});
}

float	vec2_max(t_vec2 vec)
{
	vec.x.u &= 0x7FFFFFFF;
	vec.y.u &= 0x7FFFFFFF;
	if (vec.x.f > vec.y.f)
		return (vec.x.f);
	else
		return (vec.y.f);
}
