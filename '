#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cmlx_base.h"

int32_t	vec2_idist(t_vec2 a, t_vec2 b)
{
	int32_t	dist;
	t_vec2	delta;

	delta.x.i = a.x.i - b.x.i;
	delta.y.i = a.y.i - b.y.i;
	dist = delta.x.i * delta.x.i + delta.y.i * delta.y.i;
	return (dist);
}

float	vec2_dot(t_vec2 a, t_vec2 b)
{
	return (a.x.f * b.x.f + a.y.f * b.y.f);
}
