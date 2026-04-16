#ifndef CMLX_DRAW_H
# define CMLX_DRAW_H

# include <cmlx_base.h>

typedef struct s_ray
{
	t_vec2	ray_dir;
	t_vec2	map_pos;	// Integer
	t_vec2	d_dist;
	t_vec2	side_dist;
	t_vec2	step;		// Integer
}	t_ray;

typedef struct s_rayhit
{
	uint8_t	tex_index;
	uint8_t	tex_dir;
	float	x_pos;
	float	perp_dist;
}	t_rayhit;

typedef struct s_plane
{
	t_mat32	texture;
	t_vec2	ray_left;
	t_vec2	ray_right;
	t_vec2	pos;
	t_vec2	step;
}	t_plane;

typedef struct s_quad
{
	t_vec2		pos;
	t_vec2		size;
	uint32_t	color;
	int32_t		radius;
	t_vec2		center;
}	t_quad;

typedef struct s_sides
{
	int32_t		x;
	int32_t		y;
	uint32_t	top;
	uint32_t	bottom;
	uint32_t	left;
	uint32_t	right;
}	t_sides;

typedef struct s_transform
{
	t_vec2	draw_pos;
	t_vec2	delta;
	t_vec2	norm_offset;	// Clip start to normalized range
	t_sides	bounds;
}	t_form;

typedef struct s_sheet
{
	t_mat32		texture;
	int32_t		frame_time;	// When frame_dt > frame_time, go to next frame
	int32_t		frame_dt;	// Counts how much time has passed in current frame
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

typedef struct s_view
{
	t_vec2	pos;
	t_vec2	dir;
	t_vec2	plane;
}	t_view;

typedef struct s_frame
{
	t_mat32		display;
	t_mat32		render;
	SDL_Surface	*img;
	t_img		*img2;
	t_rayhit	*rays;
	int32_t		offset;
}	t_frame;

#endif
