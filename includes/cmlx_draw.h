#ifndef CMLX_DRAW_H
# define CMLX_DRAW_H

# include <cmlx_base.h>

typedef struct s_ray
{
	t_vec2  ray_dir;
	t_vec2  map_pos;    // Integer
	t_vec2  delta_dist;
	t_vec2  side_dist;
	t_vec2  step;       // Integer
}	t_ray;

// typedef struct s_rayhit
// {
// 	uint8_t		tex_index;
// 	uint8_t		tex_dir;
// 	uint16_t	tex_offset;
// 	float		perp_dist;
// }	t_rayhit;

typedef struct s_rayhit
{
	uint8_t	tex_index;
	uint8_t	tex_dir;
	float	x_pos;		// THIS IS UGLY
	float	perp_dist;
}	t_rayhit;

typedef struct s_plane
{
	t_mat32		sprite;
	t_vec2		raydir_left;
	t_vec2		raydir_right;
	t_vec2		step;
	t_vec2		pos;
	t_vec2		texture;
	uint32_t	*col_ptr;
	int32_t		ceil_y;
	int32_t		vert_dist;
	float		pos_z;
	float		row_dist;
}	t_plane;

typedef	struct s_grid
{
    t_vec2  size;
	t_vec2	i;
    t_vec2  draw_pos;
    t_vec2  map_pos;
	t_vec2	map_center;
	t_vec2	pos;
	int32_t	bound;
}	t_grid;

typedef struct s_quad
{
	t_vec2	pos;
	t_vec2	size;
	int32_t	color;
	int32_t	bound;
	t_vec2	map_center;
}	t_quad;

typedef struct s_sides
{
	int32_t	top;
	int32_t	bottom;
	int32_t	left;
	int32_t	right;
}	t_sides;

typedef struct s_transform
{
	t_vec2		draw_pos;
	t_vec2		delta;
	t_vec2		norm_offset;	// Clip start to normalized range
	uint32_t	top;
	uint32_t	bottom;
	uint32_t	left;
	uint32_t	right;
}	t_form;

typedef struct s_sheet
{
	t_mat32		texture;
	long		frame_time;		// When frame_dt exceeds frame_time, go to next frame
	long		frame_dt;		// Counts how much time has passed in the current frame
	uint32_t	frame_size;
	uint16_t	index;
	uint16_t	count;
}	t_sheet;

typedef union u_block
{
	struct
	{
		t_mat32	north;
		t_mat32	east;
		t_mat32	south;
		t_mat32	west;
	};
	t_mat32	index[4];
}	t_block;

typedef struct s_drawbuf
{
	t_sheet	hands;
	t_sheet	radar;
	// TMP >>>
	t_sheet	radar_l0;
	t_sheet	radar_l1;
	// <<< TMP
}	t_drawbuf;

typedef struct s_view
{
	t_vec2  pos;
	t_vec2  dir;
	t_vec2  plane;
}   t_view;

typedef struct s_frame
{
	t_mat32		display;
	t_mat32		render;
	t_img		*img;
	t_rayhit	*rays;
	int32_t		offset;
}	t_frame;

#endif
