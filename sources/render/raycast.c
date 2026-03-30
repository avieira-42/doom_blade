#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

static inline
t_ray	stt_raycast_init(float camera_x, t_vec2 pos, t_vec2 dir, t_vec2 plane)
{
	t_ray	ray;

	ray.ray_dir.x.f = dir.x.f + plane.x.f * camera_x;
	ray.ray_dir.y.f = dir.y.f + plane.y.f * camera_x;
	ray.map_pos.x.i = (int32_t)pos.x.f;
	ray.map_pos.y.i = (int32_t)pos.y.f;
	ray.delta_dist.x.f = fabs(1.0f / (ray.ray_dir.x.f + EPS));	// TODO: Check if EPS works
	ray.delta_dist.y.f = fabs(1.0f / (ray.ray_dir.y.f + EPS));
	if (ray.ray_dir.x.f < 0)
		ray.step.x.i = -1;
	else
		ray.step.x.i = 1;
	if (ray.ray_dir.y.f < 0)
		ray.step.y.i = -1;
	else
		ray.step.y.i = 1;
	if (ray.ray_dir.x.f < 0)
		ray.side_dist.x.f = (pos.x.f - ray.map_pos.x.i) * ray.delta_dist.x.f;
	else
		ray.side_dist.x.f = (ray.map_pos.x.i + 1.0f - pos.x.f) * ray.delta_dist.x.f;
	if (ray.ray_dir.y.f < 0)
		ray.side_dist.y.f = (pos.y.f - ray.map_pos.y.i) * ray.delta_dist.y.f;
	else
		ray.side_dist.y.f = (ray.map_pos.y.i + 1.0f - pos.y.f) * ray.delta_dist.y.f;
	return (ray);
}

static inline
t_rayhit	stt_rayhit_info(t_ray *ray, uint8_t side, size_t block_index, t_vec2 pos)
{
	t_rayhit	hit;
	float		x_pos_texture;

	if (side == 0)
	{
		hit.perp_dist = (ray->map_pos.x.i - pos.x.f + (1 - ray->step.x.i) / 2) / ray->ray_dir.x.f;
		hit.tex_dir = 1 + ((ray->step.x.i > 0) << 1);
		x_pos_texture = pos.y.f + hit.perp_dist * ray->ray_dir.y.f;
	}
	else
	{
		hit.perp_dist = (ray->map_pos.y.i - pos.y.f + (1 - ray->step.y.i) / 2) / ray->ray_dir.y.f;
		hit.tex_dir = (ray->step.y.i > 0) << 1;
		x_pos_texture = pos.x.f + hit.perp_dist * ray->ray_dir.x.f;
	}
	hit.x_pos = (x_pos_texture - floorf(x_pos_texture));	// REVIEW
	hit.tex_index = block_index;
	return (hit);
}

static
t_rayhit	stt_raycast(float camera_x, t_view *cam, t_mat8 *map)
{
	t_ray		ray;
	uint8_t		block_index;
	uint8_t		side;

	block_index = 0;
	ray = stt_raycast_init(camera_x, cam->pos, cam->dir, cam->plane);
	while (block_index == 0)
	{
		if (ray.side_dist.x.f < ray.side_dist.y.f)
		{
			ray.side_dist.x.f += ray.delta_dist.x.f;
			ray.map_pos.x.i += ray.step.x.i;
			side = 0;
		}
		else
		{
			ray.side_dist.y.f += ray.delta_dist.y.f;
			ray.map_pos.y.i += ray.step.y.i;
			side = 1;
		}
		if ((uint32_t)(ray.map_pos.x.i) > map->width || (uint32_t)(ray.map_pos.y.i) > map->height)	// REVIEW: Cast trick may be problematic
			break ;
		block_index = map->ptr[ray.map_pos.y.i * map->stride + ray.map_pos.x.i];
	}
	return (stt_rayhit_info(&ray, side, block_index, cam->pos));
}

void	raycast(t_view *cam, t_mat8 *map, t_frame *frame)
{
	size_t		x;
	float		camera_x;
	const float	dx = 2.0 / (double) RENDER_WIDTH;

	x = 0;
	camera_x = -1.0f;
	while (x < RENDER_WIDTH)
	{
		frame->rays[x] = stt_raycast(camera_x, cam, map);	// if block_index is a specific number, do not render
		camera_x += dx;
		x++;
	}
}
