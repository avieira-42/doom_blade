#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

static inline
t_ray	stt_raycast_init(float camera_x, t_view *cam)
{
	t_ray	ray;

	ray.ray_dir.x.f = cam->dir.x.f + cam->plane.x.f * camera_x;
	ray.ray_dir.y.f = cam->dir.y.f + cam->plane.y.f * camera_x;
	ray.map_pos.x.i = (int32_t)cam->pos.x.f;
	ray.map_pos.y.i = (int32_t)cam->pos.y.f;
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
		ray.side_dist.x.f = (cam->pos.x.f - ray.map_pos.x.i) * ray.delta_dist.x.f;
	else
		ray.side_dist.x.f = (ray.map_pos.x.i + 1.0f - cam->pos.x.f) * ray.delta_dist.x.f;
	if (ray.ray_dir.y.f < 0)
		ray.side_dist.y.f = (cam->pos.y.f - ray.map_pos.y.i) * ray.delta_dist.y.f;
	else
		ray.side_dist.y.f = (ray.map_pos.y.i + 1.0f - cam->pos.y.f) * ray.delta_dist.y.f;
	return (ray);
}

static inline
t_rayhit	stt_rayhit_info(t_ray *ray, uint8_t side, t_block *block, t_view *cam)
{
	t_rayhit	hit;
	float		x_pos_texture;

	if (side == 0)
	{
		hit.perp_dist = (ray->map_pos.x.i - cam->pos.x.f + (1 - ray->step.x.i) / 2) / ray->ray_dir.x.f;
		if (ray->step.x.i > 0)
			hit.texture = block->west;
		else
			hit.texture = block->east;
		x_pos_texture = cam->pos.y.f + hit.perp_dist * ray->ray_dir.y.f;
	}
	else
	{
		hit.perp_dist = (ray->map_pos.y.i - cam->pos.y.f + (1 - ray->step.y.i) / 2) / ray->ray_dir.y.f;
		if (ray->step.y.i > 0)
			hit.texture = block->north;
		else
			hit.texture = block->south;
		x_pos_texture = cam->pos.x.f + hit.perp_dist * ray->ray_dir.x.f;
	}
	x_pos_texture -= floorf(x_pos_texture);
	hit.texture.ptr += (size_t)(x_pos_texture * hit.texture.width) * hit.texture.stride;
	hit.line_height = (float) RENDER_HEIGHT / hit.perp_dist;
	return (hit);
}

t_rayhit	raycast(float camera_x, t_view *cam, t_mat8 *map, t_block *blocks)
{
	t_ray		ray;
	uint8_t		block_index;
	uint8_t		side;

	block_index = 0;
	ray = stt_raycast_init(camera_x, cam);
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
	return (stt_rayhit_info(&ray, side, blocks + block_index, cam));
}
