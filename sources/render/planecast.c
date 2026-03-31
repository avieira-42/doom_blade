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

	// calc direction of left edfe of cam view
	plane->raydir_left.x.f = cam.dir.x.f - cam.plane.x.f;
	plane->raydir_left.y.f = cam.dir.y.f - cam.plane.y.f;
	// calc direction of right edfe of cam view
	plane->raydir_right.x.f = cam.dir.x.f + cam.plane.x.f;
	plane->raydir_right.y.f = cam.dir.y.f + cam.plane.y.f;
	// vert dist from center of screen to current row
	plane->vert_dist = y - (float)render_frame.height / 2 + frame.offset;
	// cam height in screen units (half)
	plane->pos_z = 0.5f * render_frame.height;
	// dist from cam to horizontal plane at current row
	plane->row_dist = plane->pos_z / plane->vert_dist;
	// horiz floor step in world space per screen pixel (x)
	plane->step.x.f = plane->row_dist * (plane->raydir_right.x.f
			- plane->raydir_left.x.f) / render_frame.width;
	// horiz floor step in world space per screen pixel (y)
	plane->step.y.f = plane->row_dist * (plane->raydir_right.y.f
			- plane->raydir_left.y.f) / render_frame.width;
	// world x coord of plane under leftmost pixel of current row
	plane->pos.x.f = cam.pos.x.f + plane->row_dist
		* plane->raydir_left.x.f;
	// world y coord of plane under leftmost pixel of current row
	plane->pos.y.f = cam.pos.y.f + plane->row_dist
		* plane->raydir_left.y.f;
}

//  compute per pixel value
static
void	stt_calculate_pixel_ceiling(t_frame frame, t_plane *plane,
			t_view cam, int32_t y)
{
	const t_mat32	render_frame = frame.render;

	// calc direction of left edfe of cam view
	plane->raydir_left.x.f = cam.dir.x.f - cam.plane.x.f;
	plane->raydir_left.y.f = cam.dir.y.f - cam.plane.y.f;
	// calc direction of right edfe of cam view
	plane->raydir_right.x.f = cam.dir.x.f + cam.plane.x.f;
	plane->raydir_right.y.f = cam.dir.y.f + cam.plane.y.f;
	// vert dist from center of screen to current row
	plane->vert_dist = y - ((float)render_frame.height / 2 + frame.offset);
	// cam height in screen units (half)
	plane->pos_z = 0.5f * render_frame.height;
	// dist from cam to horizontal plane at current row
	plane->row_dist = plane->pos_z / plane->vert_dist;
	// horiz floor step in world space per screen pixel (x)
	plane->step.x.f = plane->row_dist * (plane->raydir_right.x.f
			- plane->raydir_left.x.f) / render_frame.width;
	// horiz floor step in world space per screen pixel (y)
	plane->step.y.f = plane->row_dist * (plane->raydir_right.y.f
			- plane->raydir_left.y.f) / render_frame.width;
	// world x coord of plane under leftmost pixel of current row
	plane->pos.x.f = cam.pos.x.f + plane->row_dist
		* plane->raydir_left.x.f;
	// world y coord of plane under leftmost pixel of current row
	plane->pos.y.f = cam.pos.y.f + plane->row_dist
		* plane->raydir_left.y.f;
}

static
void	stt_render_row_floor(t_mat32 frame, t_plane *plane, int32_t x, int32_t y)
{
	uint32_t	floor_color;
	uint32_t	ceil_color;

	// calc text x coord from fract part of world x
	plane->texture.x.i = (int)(plane->sprite.width
			* (plane->pos.x.f - (int)plane->pos.x.f));
	// calc text y coord from fract part of world y
	plane->texture.y.i = (int)(plane->sprite.height
			* (plane->pos.y.f - (int)plane->pos.y.f));
	// wrap text coords
	plane->texture.x.i = plane->texture.x.i & (plane->sprite.width - 1);
	plane->texture.y.i = plane->texture.y.i & (plane->sprite.height - 1);
	// sample floor text pixel
	floor_color = plane->sprite.ptr[plane->texture.y.i
		* plane->sprite.width + plane->texture.x.i];
	// sample ceil text pixel
	// pointer to start of col x in framebuff
	plane->col_ptr = frame.ptr + (x * frame.stride);
	// draw floor pixel at row y
	plane->col_ptr[y] = floor_color;
	// move world pos forward for next pixel in curr row
	plane->pos.x.f += plane->step.x.f;
	plane->pos.y.f += plane->step.y.f;
}

// compute per row value
static
void	stt_render_row_ceil(t_mat32 frame, t_plane *plane, int32_t x, int32_t y)
{
	uint32_t	floor_color;
	uint32_t	ceil_color;

	// calc text x coord from fract part of world x
	plane->texture.x.i = (int)(plane->sprite.width
			* (plane->pos.x.f - (int)plane->pos.x.f));
	// calc text y coord from fract part of world y
	plane->texture.y.i = (int)(plane->sprite.height
			* (plane->pos.y.f - (int)plane->pos.y.f));
	// wrap text coords
	plane->texture.x.i = plane->texture.x.i & (plane->sprite.width - 1);
	plane->texture.y.i = plane->texture.y.i & (plane->sprite.height - 1);
	// sample floor text pixel
	floor_color = plane->sprite.ptr[plane->texture.y.i
		* plane->sprite.width + plane->texture.x.i];
	// sample ceil text pixel
	ceil_color = plane->sprite.ptr[plane->texture.y.i
		* plane->sprite.width + plane->texture.x.i];
	// pointer to start of col x in framebuff
	plane->col_ptr = frame.ptr + (x * frame.stride);
	// draw floor pixel at row y
	//plane->col_ptr[y] = floor_color;
	// calc mirrored row for ceiling
	plane->ceil_y = frame.height - y - 1;
	// draw ceiling pixel at mirrored row
	plane->col_ptr[plane->ceil_y] = ceil_color;
	// move world pos forward for next pixel in curr row
	plane->pos.x.f += plane->step.x.f;
	plane->pos.y.f += plane->step.y.f;
}

static
void	stt_floorcast(t_frame frame, t_mat32 floor_mat, t_view cam)
{
	int		y;
	int		x;
	t_plane	floor;
	const t_mat32	render_frame = frame.render;

	y = (float)render_frame.height / 2 - frame.offset;
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

static
void	stt_ceilingcast(t_frame frame, t_mat32 ceil_mat, t_view cam)
{
	int		y;
	int		x;
	t_plane	ceil;
	const t_mat32	render_frame = frame.render;

	y = (float)render_frame.height / 2 + frame.offset;
	ceil.sprite = ceil_mat;
	while (y < render_frame.height)
	{
		stt_calculate_pixel_ceiling(frame, &ceil, cam, y);
		x = 0;
		while (x < render_frame.width)
		{
			stt_render_row_ceil(render_frame, &ceil, x, y);
			x++;
		}
		y++;
	}
}

void	planecast(t_frame frame, t_mat32 floor_mat, t_mat32 ceil_mat, t_view cam)
{
	stt_floorcast(frame, floor_mat, cam);
	stt_ceilingcast(frame, ceil_mat, cam);
}
