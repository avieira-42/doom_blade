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

	ray.ray_dir.x.f = dir.x.f + camera_x * plane.x.f;
	ray.ray_dir.y.f = dir.y.f + camera_x * plane.y.f;
	ray.map_pos.x.i = (int32_t)floorf(pos.x.f);
	ray.map_pos.y.i = (int32_t)floorf(pos.y.f);
	ray.d_dist.x.f = 1.0f / (fabsf(ray.ray_dir.x.f) + EPS);
	ray.d_dist.y.f = 1.0f / (fabsf(ray.ray_dir.y.f) + EPS);
	if (ray.ray_dir.x.f < 0)
		ray.step.x.i = -1;
	else
		ray.step.x.i = 1;
	if (ray.ray_dir.y.f < 0)
		ray.step.y.i = -1;
	else
		ray.step.y.i = 1;
	if (ray.ray_dir.x.f < 0)
		ray.side_dist.x.f = (pos.x.f - ray.map_pos.x.i) * ray.d_dist.x.f;
	else
		ray.side_dist.x.f = (ray.map_pos.x.i + 1.0f - pos.x.f) * ray.d_dist.x.f;
	if (ray.ray_dir.y.f < 0)
		ray.side_dist.y.f = (pos.y.f - ray.map_pos.y.i) * ray.d_dist.y.f;
	else
		ray.side_dist.y.f = (ray.map_pos.y.i + 1.0f - pos.y.f) * ray.d_dist.y.f;
	return (ray);
}

static inline
t_rayhit	stt_rayhit_info(t_ray *ray, uint8_t side,
		uint8_t block_index, t_vec2 pos)
{
	t_rayhit	hit;
	float		x_pos_texture;

	if (side == 0)
	{
		hit.perp_dist = (ray->map_pos.x.i - pos.x.f + 0.5f * (1 - ray->step.x.i)) / ray->ray_dir.x.f;
		hit.tex_dir = 1 + ((ray->step.x.i > 0) << 1);
		x_pos_texture = pos.y.f + hit.perp_dist * ray->ray_dir.y.f;
	}
	else
	{
		hit.perp_dist = (ray->map_pos.y.i - pos.y.f + 0.5f * (1 - ray->step.y.i)) / ray->ray_dir.y.f;
		hit.tex_dir = (ray->step.y.i < 0) << 1;
		x_pos_texture = pos.x.f + hit.perp_dist * ray->ray_dir.x.f;
	}
	hit.x_pos = (x_pos_texture - floorf(x_pos_texture));	// REVIEW
	hit.tex_index = block_index & 127;
	return (hit);
}

static inline
t_rayhit	stt_dda(t_ray *ray, t_vec2 pos, t_map *map)
{
	uint8_t	block_index;
	uint8_t	side;

	block_index = 0;
	while ((block_index & 128) == 0)	// Only cares about the collision bit
	{
		if (ray->side_dist.x.f < ray->side_dist.y.f)
		{
			ray->side_dist.x.f += ray->d_dist.x.f;
			ray->map_pos.x.i += ray->step.x.i;
			side = 0;
		}
		else
		{
			ray->side_dist.y.f += ray->d_dist.y.f;
			ray->map_pos.y.i += ray->step.y.i;
			side = 1;
		}
		if ((uint32_t)(ray->map_pos.x.i) >= map->width
				|| (uint32_t)(ray->map_pos.y.i) >= map->height)
			break ;
		block_index = map->tex_index[ray->map_pos.y.i
			* map->width + ray->map_pos.x.i];
	}
	return (stt_rayhit_info(ray, side, block_index, pos));
}

// Compares the x_pos of a door hit against the current state of the door
// State = 0, door is fully open; State = 255, door is fully closed;
// Assumes doors goes from right to left to open
static
t_rayhit	stt_raycast(float camera_x, t_view *cam, t_map *map)
{
	t_ray		ray;
	t_rayhit	hit;
	uint8_t		door_state;
	uint16_t	hit_index;	// The hit index relative to frame count of door openings

	ray = stt_raycast_init(camera_x, cam->pos, cam->dir, cam->plane);
	while (true)
	{
		hit = stt_dda(&ray, cam->pos, map);
		if (!(hit.tex_index & 130))	// It is not a door
			break ;
		hit_index = hit.x_pos * 255;
		door_state = map->state[ray.map_pos.y.i * map->width + ray.map_pos.x.i];
		if (hit_index <= door_state)
			break ;
	}
	return (hit);
}

void	raycast(t_view *cam, t_map *map, t_rayhit *rays)
{
	size_t		x;
	float		camera_x;
	const float	dx = 2.0 / R_WIDTH;

	x = 0;
	camera_x = -1.0f;
	while (x < R_WIDTH)
	{
		rays[x] = stt_raycast(camera_x, cam, map);
		camera_x += dx;
		x++;
	}
}
