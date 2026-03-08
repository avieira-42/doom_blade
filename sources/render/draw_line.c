#include <stdint.h>
#include "cmlx.h"
#include "cub_structs.h"
#include "cub_utils.h"

static inline
void	stt_putrgb(t_img *img, uint32_t x, uint32_t y, uint32_t argb)
{
	((uint32_t (*)[img->width])img->data)[y][x] = argb;
}

static inline
uint32_t stt_clerp(uint32_t dst, uint32_t src, uint8_t alpha)
{
	uint32_t rb;
	uint32_t ga;

	rb = dst & 0x00FF00FF;
	ga = (dst >> 8) & 0x00FF00FF;
	rb += (((src & 0x00FF00FF) - rb) * alpha) >> 8;
	ga += ((((src >> 8) & 0x00FF00FF) - ga) * alpha) >> 8;
	return (rb & 0x00FF00FF) | ((ga & 0x00FF00FF) << 8);
}

void	draw_line(t_img *img, t_vec3 p0, t_vec3 p1)
{
	const uint32_t	len = ft_iabsmax(p1.x.i - p0.x.i, p1.y.i - p0.y.i);
	const t_vec3	delta = {.x.f = (double)(p1.x.i - p0.x.i) / (len + 1),
		.y.f = (double)(p1.y.i - p0.y.i) / (len + 1), .z.u = (255u << 16) / (len + 1)};
	t_vec3			vec;
	uint32_t		i;

	vec.x.f = p0.x.u;
	vec.y.f = p0.y.u;
	vec.z.u = 0;
	i = 0;
	while (i <= len)
	{
		if (vec.x.f >= 0.0f && vec.y.f >= 0.0f && vec.x.f < SCREEN_WIDTH && vec.y.f < SCREEN_HEIGHT)
			stt_putrgb(img, vec.x.f, vec.y.f, stt_clerp(p0.z.u, p1.z.u, vec.z.u >> 16));
		vec.x.f += delta.x.f;
		vec.y.f += delta.y.f;
		vec.z.u += delta.z.u;	// Color
		i++;
	}
}
