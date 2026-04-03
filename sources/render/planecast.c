#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_utils.h"

static
void	stt_plane_row(t_plane *plane, t_frame frame, t_view cam, int32_t dist)
{
	const t_mat32	dst = frame.render;
	const float		row_dist = (0.5f * dst.height) / dist;
	const float		factor = row_dist / dst.width;

	plane->step.x.f = factor * (plane->ray_right.x.f - plane->ray_left.x.f);
	plane->step.y.f = factor * (plane->ray_right.y.f - plane->ray_left.y.f);
	plane->pos.x.f = cam.pos.x.f + row_dist * plane->ray_left.x.f;
	plane->pos.y.f = cam.pos.y.f + row_dist * plane->ray_left.y.f;
}

static
void	stt_plane_draw(t_mat32 dst, t_plane *plane, int32_t y)
{
	int32_t		x;
	uint16_t	tx;
	uint16_t	ty;
	uint32_t	*ptr;
	uint32_t	color;

	x = 0;
	ptr = dst.ptr;
	while (x < dst.width)
	{
		tx = (int)(plane->texture.width * (plane->pos.x.f - (int)plane->pos.x.f));
		ty = (int)(plane->texture.height * (plane->pos.y.f - (int)plane->pos.y.f));
		tx &= plane->texture.width - 1;
		ty &= plane->texture.height - 1;
		color = plane->texture.ptr[tx * plane->texture.stride + ty];
		ptr[y] = color;
		plane->pos.x.f += plane->step.x.f;
		plane->pos.y.f += plane->step.y.f;
		ptr += dst.stride;
		x++;
	}
}

static
void	stt_render_floor(t_frame frame, t_mat32 texture, t_view cam)
{
	int32_t			y;
	int32_t			horizon;
	t_plane			plane;
	const t_mat32	dst = frame.render;

	plane.texture = texture;
	plane.ray_left.x.f = cam.dir.x.f - cam.plane.x.f;
	plane.ray_left.y.f = cam.dir.y.f - cam.plane.y.f;
	plane.ray_right.x.f = cam.dir.x.f + cam.plane.x.f;
	plane.ray_right.y.f = cam.dir.y.f + cam.plane.y.f;
	horizon = dst.height / 2 - frame.offset;
	y = ft_imax(0, horizon + 1);
	while (y < dst.height)
	{
		stt_plane_row(&plane, frame, cam, y - horizon);
		stt_plane_draw(dst, &plane, y);
		y++;
	}
}

static
void	stt_render_ceil(t_frame frame, t_mat32 texture, t_view cam)
{
	int32_t			y;
	int32_t			horizon;
	t_plane			plane;
	const t_mat32	dst = frame.render;

	plane.texture = texture;
	plane.ray_left.x.f = cam.dir.x.f - cam.plane.x.f;
	plane.ray_left.y.f = cam.dir.y.f - cam.plane.y.f;
	plane.ray_right.x.f = cam.dir.x.f + cam.plane.x.f;
	plane.ray_right.y.f = cam.dir.y.f + cam.plane.y.f;
	horizon = dst.height / 2 + frame.offset;
	y = ft_imax(0, horizon + 1);
	while (y < dst.height)
	{
		stt_plane_row(&plane, frame, cam, y - horizon);
		stt_plane_draw(dst, &plane, dst.height - y - 1);
		y++;
	}
}

void	planecast(t_frame frame, t_mat32 floor, t_mat32 ceil, t_view cam)
{
	stt_render_floor(frame, floor, cam);
	stt_render_ceil(frame, ceil, cam);
}
