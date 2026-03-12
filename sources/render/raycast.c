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

// To do: Add safety clauses for rendering 
static
uint8_t	stt_dda(t_ray *ray, t_mat8 *map, uint8_t *block_index)
{
	uint8_t	side;
	uint8_t	block;

	block = 0;
	*block_index = 0;
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
		// if ((uint32_t)(ray->map_pos.y.i) > map->cols || (uint32_t)(ray->map_pos.x.i) > map->rows)
		// 	return (0);
		block = map->ptr[ray->map_pos.y.i * map->cols + ray->map_pos.x.i];
		if (block > 0)
			*block_index = block;
	}
	return (side);
}

static
t_ray_hit	stt_raycast(t_ray *ray, t_view *cam, t_mat8 *map, t_block *blocks)
{
	t_ray_hit		hit;
	uint8_t			block_index;
	const uint8_t	side = stt_dda(ray, map, &block_index);

	if (side == 0)
	{
		hit.perp_dist = (ray->map_pos.x.i - cam->pos.x.f + (1 - ray->step.x.i) / 2) / ray->ray_dir.x.f;
		if (ray->step.x.i > 0)
			hit.texture = blocks[block_index].west;
		else
			hit.texture = blocks[block_index].east;
		hit.x_pos_texture = cam->pos.y.f + hit.perp_dist * ray->ray_dir.y.f;
	}
	else
	{
		hit.perp_dist = (ray->map_pos.y.i - cam->pos.y.f + (1 - ray->step.y.i) / 2) / ray->ray_dir.y.f;
		if (ray->step.y.i > 0)
			hit.texture = blocks[block_index].north;
		else
			hit.texture = blocks[block_index].south;
		hit.x_pos_texture = cam->pos.x.f + hit.perp_dist * ray->ray_dir.x.f;
	}
	hit.x_pos_texture -= floorf(hit.x_pos_texture);
	return (hit);
}

// See if FLTO inlines
// Convert textures to power of two
static
void	stt_column_render(t_ray_hit hit, uint32_t *render_col)
{
	size_t			x;
	const int32_t	line_height = (float) RENDER_HEIGHT / (float) hit.perp_dist;
	const uint32_t	draw_start = ft_imax(0, (RENDER_HEIGHT / 2) - (line_height / 2));
	const uint32_t	draw_end = ft_imin(RENDER_HEIGHT, (RENDER_HEIGHT / 2) + (line_height / 2));
	const float		dx = (double)hit.texture.cols / (double)line_height;	// cols represents height here because the texture is transposed
	float			tex_pos;

	hit.texture.ptr += (size_t)(hit.x_pos_texture * hit.texture.rows) * hit.texture.cols;
	tex_pos = (draw_start - (RENDER_HEIGHT / 2) + (line_height / 2)) * dx;
	x = draw_start;
	while (x < draw_end)
	{
		render_col[x] = hit.texture.ptr[(size_t)tex_pos];
		tex_pos += dx;
		x++;
	}
}

// Blocks contains transposed rows for sequential memory access
// Everything is done in cols by rows, and then tranposed for the rendering
void	raycast(t_view *cam, t_mat8 *map, t_block *blocks, t_mat32 render_frame)
{
	size_t		x;
	t_ray		ray;
	t_ray_hit	hit;
	float		camera_x;
	const float	dx = 2.0 / (double) RENDER_WIDTH;

	x = 0;
	camera_x = -1.0f;
	while (x < RENDER_WIDTH)
	{
		ray = stt_raycast_init(camera_x, cam);
		hit = stt_raycast(&ray, cam, map, blocks);	// if block_index is a specific number, do not render
		stt_column_render(hit, render_frame.ptr);
		render_frame.ptr += render_frame.cols;
		camera_x += dx;
		x++;
	}
}
