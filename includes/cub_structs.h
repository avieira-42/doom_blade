#ifndef CUB_STRUCTS_H
# define CUB_STRUCTS_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include "cmlx.h"
# include "cub_defines.h"
# include "cub_types.h"

/* original mlx s_img struct
 * to check more easily how
typedef struct	s_img
{
	XImage			*image;
	Pixmap			pix;
	GC				gc;
	int				size_line;
	int				bpp;
	int				width;
	int				height;
	int				type;
	int				format;
	char			*data;
	XShmSegmentInfo	shm;
}				t_img;*/

/* struct that holds the variables
 * need to scroll each spritesheet */
typedef struct	s_anim
{
	int32_t		loops_per_sprite; // aprox time for each sprite to be rendered
	int32_t		iterator; // iterator for the spritesheet scrolling
	int32_t		size;	// number of sprites in sheet
	int32_t		endian; // endian
	t_img		*sheet; // spritesheet
	float		counter; // counter to compare with loops per sprite
	long		*dt; // delta time (TO REMOVE)
	char		*base_path; // broader path name of sprite "sprite" -> "sprite1 sprite2 sprite3"
	char		*file_type; // file type of the sprite "sprite1" -> "sprite1.xpm"
	bool		end; // animation reached its end check
}	t_anim;

typedef struct s_block
{
	t_mat32	north;
	t_mat32	east;
	t_mat32	south;
	t_mat32	west;
}	t_block;

typedef struct s_view
{
	t_vec2	pos;
	t_vec2	dir;
	t_vec2	plane;
}	t_view;

// Every physics loop, positions are updated taking into account the speed vector
// And every physics loop, speed vector is updated with values from accel vector
typedef struct s_speed
{
	t_vec3	speed;	// 3d vector representing current speed
	t_vec3	accel;	// 3d vector representing current accel
	// float	base_accel;
	// float	max_speed;
}	t_speed;

typedef struct s_entity
{
	t_view	cam;
	t_speed	move;
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
	t_mat32		texture;
	float		perp_dist;
	int32_t		line_height;
}	t_rayhit;

typedef struct s_memory
{
	uint32_t	render_frame[RENDER_WIDTH][RENDER_HEIGHT];
	uint32_t	empty_line[TEX_HEIGHT];
}	t_memory;

typedef struct s_gstate
{
	bool	paused;
	long	time;
	// ...
}	t_gstate;

typedef struct s_cfg
{
	float	sens;
	float	speed;
}	t_cfg;
typedef struct s_time
{
	long	delta;
	long	prev;
}	t_time;

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
	size_t		key;
	t_cfg		cfg;
	t_time		time;
	// MAYBE CHANGE ORGANIZATION
	// hud
	t_anim	*reload;
	t_anim	*shoot;
	t_anim	*walk;
	t_anim	*ammo;
	t_anim	*health;
	t_anim	*pill;
	t_anim	*city;
}	t_game;

#endif
