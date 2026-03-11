#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

static
t_ray	stt_raycast_init(float camera_x, t_view *cam)
{
	t_ray	ray;

	ray.ray_dir.x.f = cam->dir.x.f + cam->plane.x.f * camera_x;
	ray.ray_dir.y.f = cam->dir.y.f + cam->plane.y.f * camera_x;
	ray.map_pos.x.i = (int32_t)cam->pos.x.f;
	ray.map_pos.y.i = (int32_t)cam->pos.y.f;
	ray.delta_dist.x.f = fabs(1.0f / (ray.ray_dir.x.f + EPS));
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

static
uint8_t	stt_dda(t_ray *ray, t_mat8 *map, uint8_t *block_index)
{
	uint8_t	side;
	uint8_t	block;

	block = 0;
	while (block == 0)
	{
		if (ray->side_dist.x.f < ray->side_dist.y.f)
		{
			ray->side_dist.x.f += ray->delta_dist.x.f;
			ray->map_pos.x.i += ray->step.x.i;
			side = 0;
		}
		else
		{
			ray->side_dist.y.f += ray->delta_dist.y.f;
			ray->map_pos.y.i += ray->step.y.i;
			side = 1;
		}
		block = map->ptr[ray->map_pos.y.i * map->cols + ray->map_pos.x.i];
		if (block > 0)
			*block_index = block;
	}
	return (side);
}

static
void	*stt_raycast(t_ray *ray, t_view *cam, t_mat8 *map, t_block *blocks)
{
	float	perp_dist;
	t_mat32	block;
	uint8_t	block_index;
	float	block_x;
	uint8_t	side;

	side = stt_dda(ray, map, &block_index);
	if (side == 0)
	{
		perp_dist = (ray->map_pos.x.i - cam->pos.x.f + (1 - ray->step.x.i) / 2) / ray->ray_dir.x.f;
		if (ray->step.x.i > 0)
			block = blocks[block_index].west;
		else
			block = blocks[block_index].east;
		block_x = cam->pos.y.f + perp_dist * ray->ray_dir.y.f;
	}
	else
	{
		perp_dist = (ray->map_pos.y.i - cam->pos.y.f + (1 - ray->step.y.i) / 2) / ray->ray_dir.y.f;
		if (ray->step.y.i > 0)
			block = blocks[block_index].north;
		else
			block = blocks[block_index].south;
		block_x = cam->pos.x.f + perp_dist * ray->ray_dir.x.f;
	}
	block_x -= floorf(block_x);
	return (block.ptr + (size_t)(block_x * (block.rows - 1)) * block.cols);
}

// Blocks contains transposed rows for sequential memory access
// Everything is done in cols by rows, and then tranposed for the rendering
void	raycast(t_view *cam, t_mat8 *map, t_block *blocks, uint32_t *render_frame)
{
	size_t		x;
	t_ray		ray;
	uint32_t	*src;
	float		camera_x;

	x = 0;
	while (x < RENDER_WIDTH)
	{
		camera_x = 2.0f * x / (float)RENDER_WIDTH - 1.0f;
		ray = stt_raycast_init(camera_x, cam);
		src = stt_raycast(&ray, cam, map, blocks);
		ft_memcpy(render_frame + x * RENDER_HEIGHT, src, RENDER_HEIGHT * sizeof(uint32_t));
		x++;
	}
}
