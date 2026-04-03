#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_utils.h"

//  compute per pixel value
static
void	stt_calculate_pixel_floor(t_frame frame, t_plane *plane,
			t_view cam, int32_t y)
{
	const t_mat32	render_frame = frame.render;

	plane->raydir_left.x.f = cam.dir.x.f - cam.plane.x.f;
	plane->raydir_left.y.f = cam.dir.y.f - cam.plane.y.f;
	plane->raydir_right.x.f = cam.dir.x.f + cam.plane.x.f;
	plane->raydir_right.y.f = cam.dir.y.f + cam.plane.y.f;
	plane->vert_dist = y - (float)render_frame.height / 2 + frame.offset;
	plane->pos_z = 0.5f * render_frame.height;
	plane->row_dist = plane->pos_z / plane->vert_dist;
	plane->step.x.f = plane->row_dist * (plane->raydir_right.x.f
			- plane->raydir_left.x.f) / render_frame.width;
	plane->step.y.f = plane->row_dist * (plane->raydir_right.y.f
			- plane->raydir_left.y.f) / render_frame.width;
	plane->pos.x.f = cam.pos.x.f + plane->row_dist
		* plane->raydir_left.x.f;
	plane->pos.y.f = cam.pos.y.f + plane->row_dist
		* plane->raydir_left.y.f;
}

static
void	stt_render_row_floor(t_mat32 frame, t_plane *plane,
		int32_t x, int32_t y)
{
	uint32_t	floor_color;

	plane->texture.x.i = (int)(plane->sprite.width
			* (plane->pos.x.f - (int)plane->pos.x.f));
	plane->texture.y.i = (int)(plane->sprite.height
			* (plane->pos.y.f - (int)plane->pos.y.f));
	plane->texture.x.i = plane->texture.x.i & (plane->sprite.width - 1);
	plane->texture.y.i = plane->texture.y.i & (plane->sprite.height - 1);
	floor_color = plane->sprite.ptr[plane->texture.y.i
		* plane->sprite.width + plane->texture.x.i];
	plane->col_ptr = frame.ptr + (x * frame.stride);
	plane->col_ptr[y] = floor_color;
	plane->pos.x.f += plane->step.x.f;
	plane->pos.y.f += plane->step.y.f;
}

void	floorcast(t_frame frame, t_mat32 floor_mat, t_view cam)
{
	int				y;
	int				x;
	t_plane			floor;
	const t_mat32	render_frame = frame.render;

	y = render_frame.height / 2 - frame.offset;
	if (y < 0)
		y = 0;
	floor.sprite = floor_mat;
	while (y < render_frame.height)
	{
		stt_calculate_pixel_floor(frame, &floor, cam, y);
		x = 0;
		while (x < render_frame.width)
		{
			stt_render_row_floor(render_frame, &floor, x, y);
			x++;
		}
		y++;
	}
}
