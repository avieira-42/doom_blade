#ifndef CUB_STRUCTS_H
# define CUB_STRUCTS_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include "cmlx.h"
# include "cub_defines.h"
# include "cub_types.h"

typedef struct s_block
{
	t_mat32	north;
	t_mat32	east;
	t_mat32	south;
	t_mat32	west;
}	t_block;

typedef struct	s_view
{
	t_vec2	pos;
	t_vec2	dir;
	t_vec2	plane;
}	t_view;

typedef struct s_entity
{
	t_view		cam;
	int32_t		dir_mod;
	float		speed;
	float		speed_mod;
}	t_entity;

typedef struct s_ray
{
	t_vec2	ray_dir;
	t_vec2	map_pos;	// Integer
	t_vec2	delta_dist;
	t_vec2	side_dist;
	t_vec2	step;		// Integer
}	t_ray;

typedef struct s_rayhit
{
	t_mat32	texture;
	float	x_pos_texture;
	float	perp_dist;
}	t_rayhit;

typedef struct s_memory
{
	uint32_t	render_frame[RENDER_WIDTH][RENDER_HEIGHT];
	uint32_t	empty_line[RENDER_HEIGHT];
}	t_memory;

typedef struct s_gstate
{
	bool	paused;
	long	time;
	// ...
}	t_gstate;

typedef struct s_game
{
	t_xvar		*mlx;
	t_img		*img;
	t_mat32		display_frame;
	t_mat32		render_frame;
	t_mat8		map;
	t_vec2		mouse_pos;
	t_entity	player;
	t_block		blocks[NUM_BLOCKS];	// World, Ceil/Floor, Doors, etc...
}	t_game;

#endif
