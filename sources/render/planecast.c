#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_structs.h"
#include "cub_utils.h"

//  compute per pixel value
static
void	stt_calculate_pixel(t_mat32 frame, t_plane *plane, t_view cam, int32_t y)
{
	// calc direction of left edfe of cam view
	plane->raydir_left.x.f = cam.dir.x.f - cam.plane.x.f;
	plane->raydir_left.y.f = cam.dir.y.f - cam.plane.y.f;

	// calc direction of right edfe of cam view
	plane->raydir_right.x.f = cam.dir.x.f + cam.plane.x.f;
	plane->raydir_right.y.f = cam.dir.y.f + cam.plane.y.f;

	// vert dist from center of screen to current row
	plane->vert_dist = y - frame.height / 2;

	// cam height in screen units (half)
	plane->pos_z = 0.5f * frame.height;

	// dist from cam to horizontal plane at current row
	plane->row_dist = plane->pos_z / plane->vert_dist;

	// horiz step in world space per screen pixel (x)
	plane->floor_step.x.f = plane->row_dist * (plane->raydir_right.x.f
			- plane->raydir_left.x.f) / frame.width;

	// horizontal floor_step in world space per screen pixel (y)
	plane->floor_step.y.f = plane->row_dist * (plane->raydir_right.y.f
			- plane->raydir_left.y.f) / frame.width;

	// world x coord of plane under leftmost pixel of current row
	plane->floor_pos.x.f = cam.pos.x.f + plane->row_dist
		* plane->raydir_left.x.f;

	// world y coord of plane under leftmost pixel of current row
	plane->floor_pos.y.f = cam.pos.y.f + plane->row_dist
		* plane->raydir_left.y.f;
}

// compute per row value
static
void	stt_render_row(t_mat32 frame, t_plane *plane, int32_t x, int32_t y)
{
	uint32_t	floor_color;
	uint32_t	ceil_color;

	// calc text x coord from fract part of world x
	plane->texture.x.i = (int)(plane->floor_tex.width
			* (plane->floor_pos.x.f - (int)plane->floor_pos.x.f));

	// calc text y coord from fract part of world y
	plane->texture.y.i = (int)(plane->floor_tex.height
			* (plane->floor_pos.y.f - (int)plane->floor_pos.y.f));

	// wrap text coords
	plane->texture.x.i = plane->texture.x.i & (plane->floor_tex.width - 1);
	plane->texture.y.i = plane->texture.y.i & (plane->floor_tex.height - 1);

	// sample floor text pixel
	floor_color = plane->floor_tex.ptr[plane->texture.y.i
		* plane->floor_tex.width + plane->texture.x.i];

	// sample ceil text pixel
	ceil_color = plane->ceil_tex.ptr[plane->texture.y.i
		* plane->ceil_tex.width + plane->texture.x.i];

	// pointer to start of col x in framebuff
	plane->col_ptr = frame.ptr + (x * frame.stride);

	// draw floor pixel at row y
	plane->col_ptr[y] = floor_color;

	// calc mirrored row for ceiling
	plane->ceil_y = frame.height - y - 1;

	// draw ceiling pixel at mirrored row
	plane->col_ptr[plane->ceil_y] = ceil_color;

	// move world pos forward for next pixel in curr row
	plane->floor_pos.x.f += plane->floor_step.x.f;
	plane->floor_pos.y.f += plane->floor_step.y.f;
}

void	planes_cast(t_mat32 frame, t_mat32 floor, t_mat32 ceiling, t_view cam)
{
	int		y;
	int		x;
	t_plane	plane;

	y = frame.height / 2;
	plane.floor_tex = floor;
	plane.ceil_tex = ceiling;
	while (y < frame.height)
	{
		stt_calculate_pixel(frame, &plane, cam, y);
		x = 0;
		while (x < frame.width)
		{
			stt_render_row(frame, &plane, x, y);
			x++;
		}
		y++;
	}
}
