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
t_rayhit	stt_rayhit_info(t_ray *ray, uint8_t side, size_t block_index, t_view *cam)
{
	t_rayhit	hit;
	float		x_pos_texture;

	if (side == 0)
	{
		hit.perp_dist = (ray->map_pos.x.i - cam->pos.x.f + (1 - ray->step.x.i) / 2) / ray->ray_dir.x.f;
		hit.tex_dir = 1 + ((ray->step.x.i > 0) << 1);
		x_pos_texture = cam->pos.y.f + hit.perp_dist * ray->ray_dir.y.f;
	}
	else
	{
		hit.perp_dist = (ray->map_pos.y.i - cam->pos.y.f + (1 - ray->step.y.i) / 2) / ray->ray_dir.y.f;
		hit.tex_dir = (ray->step.y.i > 0) << 1;
		x_pos_texture = cam->pos.x.f + hit.perp_dist * ray->ray_dir.x.f;
	}
	hit.tex_offset = (x_pos_texture - floorf(x_pos_texture)) * TEX_HEIGHT;	// TODO: TEMPORARY, textures aren't necessarily square
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
	return (stt_rayhit_info(&ray, side, block_index, cam));
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

void    render_textured_floor(t_game *game)
{
    t_mat32     frame;
    t_mat32     floor_tex;
    t_mat32     ceil_tex;
    int         y;
    int         x;
    float       rayDirX0;
    float       rayDirY0;
    float       rayDirX1;
    float       rayDirY1;
    int         p;
    float       posZ;
    float       rowDist;
    float       stepX;
    float       stepY;
    float       floorX;
    float       floorY;
    int         tx;
    int         ty;
    uint32_t    floor_color;
    uint32_t    ceil_color;
    uint32_t    *col_ptr;
    int         ceil_y;

    frame = game->frame.render;
    floor_tex = game->blocks[0].south;
    ceil_tex = game->blocks[0].north;

    y = frame.height / 2;
    while (y < frame.height)
    {
        rayDirX0 = game->player.cam.dir.x.f - game->player.cam.plane.x.f;
        rayDirY0 = game->player.cam.dir.y.f - game->player.cam.plane.y.f;
        rayDirX1 = game->player.cam.dir.x.f + game->player.cam.plane.x.f;
        rayDirY1 = game->player.cam.dir.y.f + game->player.cam.plane.y.f;

        p = y - frame.height / 2;
        posZ = 0.5f * frame.height;
        rowDist = posZ / p;

        stepX = rowDist * (rayDirX1 - rayDirX0) / frame.width;
        stepY = rowDist * (rayDirY1 - rayDirY0) / frame.width;

        floorX = game->player.cam.pos.x.f + rowDist * rayDirX0;
        floorY = game->player.cam.pos.y.f + rowDist * rayDirY0;

        x = 0;
        while (x < frame.width)
        {
            tx = (int)(floor_tex.width * (floorX - (int)floorX));
            ty = (int)(floor_tex.height * (floorY - (int)floorY));

            tx = tx & (floor_tex.width - 1);
            ty = ty & (floor_tex.height - 1);

            floor_color = floor_tex.ptr[ty * floor_tex.width + tx];
            ceil_color = ceil_tex.ptr[ty * ceil_tex.width + tx];

            col_ptr = frame.ptr + (x * frame.stride);

            /* FLOOR */
            col_ptr[y] = floor_color;

            /* CEILING */
            ceil_y = frame.height - y - 1;
            col_ptr[ceil_y] = ceil_color;

            floorX += stepX;
            floorY += stepY;
            x++;
        }
        y++;
    }
}
