#ifndef CUB_STRUCTS_H
# define CUB_STRUCTS_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include "cmlx.h"
# include "cub_defines.h"
# include "cub_types.h"

/* struct that holds the variables
 * need to scroll each spritesheet */
// typedef struct	s_anim
// {
// 	int32_t		loops_per_sprite; // aprox time for each sprite to be rendered
// 	int32_t		iterator; // iterator for the spritesheet scrolling
// 	int32_t		size;	// number of sprites in sheet
// 	int32_t		endian; // endian
// 	t_img		*sheet; // spritesheet
// 	float		counter; // counter to compare with loops per sprite
// 	long		*dt; // delta time (TO REMOVE)
// 	char		*base_path; // broader path name of sprite "sprite" -> "sprite1 sprite2 sprite3"
// 	char		*file_type; // file type of the sprite "sprite1" -> "sprite1.xpm"
// 	bool		end; // animation reached its end check
// }	t_anim;

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
	t_mat32	texture;
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
	float		zbuffer[RENDER_WIDTH];
	uint32_t	empty_line[TEX_HEIGHT];
}	t_memory;

typedef struct s_gstate
{
	bool	paused;
	long	time;
	size_t	key;
	// ...
}	t_gstate;

typedef struct s_cfg
{
	float	sens;
	float	speed;
}	t_cfg;

typedef struct s_sheet
{
	t_mat32		texture;
	float		counter;
	uint16_t	iterator;
	uint16_t	loops_per_sprite;
}	t_sheet;

typedef struct s_assets
{
	t_sheet	reload;
	t_sheet	shoot;
	t_sheet	walk;
	t_sheet	ammo;
	t_sheet	health;
	t_sheet	pill;
	t_sheet	city;
}	t_assets;

typedef struct s_frame
{
	t_mat32	display;
	t_mat32	render;
	float	*zbuffer;
	t_img	*img;
}	t_frame;

typedef struct s_game
{
	t_xvar		*mlx;
	t_frame		frame;
	t_gstate	state;
	t_mat8		map;
	t_entity	player;
	t_entity	enemies[4];
	t_block		blocks[NUM_BLOCKS];	// World, Ceil/Floor, Doors, etc...
	t_cfg		cfg;
	t_assets	assets;
}	t_game;

#endif
