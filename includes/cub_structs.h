#ifndef CUB_STRUCTS_H
# define CUB_STRUCTS_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include <SDL2/SDL.h>
# include <SDL2/SDL_mixer.h>
# include "cmlx.h"
# include "cub_defines.h"
# include "cub_types.h"

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

// REVIEW: bad names, like wtf is first? why is delta stored? iterator and counter mean the same thing but one is size_t and other is float
// how is loops per sprite used? 
typedef struct s_sheet
{
	t_mat32		texture;
	size_t		iterator;
	float		counter;
	uint32_t	*first;
	uint16_t	loops_per_sprite;
	float		delta;
	bool		start;
	bool		end;
	bool		sound;
}	t_sheet;

typedef struct s_view
{
	t_vec2  pos;
	t_vec2  dir;
	t_vec2  plane;
}   t_view;

// Every physics loop, positions are updated taking into account the speed vector
// And every physics loop, speed vector is updated with values from accel vector
typedef struct s_speed
{
	t_vec3  speed;  // 3d vector representing current speed
	t_vec3  accel;  // 3d vector representing current accel
	// float    base_accel;
	// float    max_speed;
}   t_speed;

typedef struct s_entity
{
	t_view	cam;
	t_speed	move;
	t_mat32	texture;
}	t_entity;

typedef struct s_ray
{
	t_vec2  ray_dir;
	t_vec2  map_pos;    // Integer
	t_vec2  delta_dist;
	t_vec2  side_dist;
	t_vec2  step;       // Integer
}   t_ray;

typedef struct s_plane
{
	t_mat32		floor_tex;
	t_mat32		ceil_tex;
	t_vec2		raydir_left;
	t_vec2		raydir_right;
	t_vec2		floor_step;
	t_vec2		floor_pos;
	t_vec2		texture;
	uint32_t	*col_ptr;
	int			ceil_y;
	int32_t		vert_dist;
	float		pos_z;
	float		row_dist;
}	t_plane;

typedef struct s_rayhit
{
	uint8_t		tex_index;
	uint8_t		tex_dir;
	uint16_t	tex_offset;
	float		perp_dist;
}	t_rayhit;

typedef struct s_memory
{
	uint32_t	render_frame[RENDER_WIDTH][RENDER_HEIGHT];
	t_rayhit	rays[RENDER_WIDTH];
	uint32_t	empty_line[TEX_HEIGHT];
}	t_memory;

typedef struct s_gstate
{
	bool	paused;
	long	time;
	size_t	key;
	// ...
}   t_gstate;

typedef struct s_cfg
{
	float   sens;
	float   speed;
}   t_cfg;

// ADDED OLD >>>>>

typedef struct s_time               // ADDED OLD
{                                   // ADDED OLD
	float    delta;                  // ADDED OLD
	long    prev;                   // ADDED OLD
}   t_time;                         // ADDED OLD

typedef struct  s_gun               // ADDED OLD
{                                   // ADDED OLD
	int32_t ammo;                   // ADDED OLD
	int32_t max_ammo;               // ADDED OLD
	int32_t first_iterator;         // ADDED OLD
}   t_gun;                           // ADDED OLD

typedef struct  s_audio             // ADDED OLD
{                                   // ADDED OLD
	Mix_Chunk   *no_ammo;           // ADDED OLD
	Mix_Chunk   *gun_shot;          // ADDED OLD
	Mix_Chunk   *gun_reload;        // ADDED OLD
	Mix_Chunk   *step;              // ADDED OLD
	Mix_Chunk   *step_fast;         // ADDED OLD
	Mix_Chunk   *current_step;      // ADDED OLD
}   t_audio;                          // ADDED OLD

typedef struct s_frame
{
	t_mat32		display;
	t_mat32		render;
	t_img		*img;
	t_rayhit	*rays;
}	t_frame;

typedef struct s_assets
{
	t_sheet	reload;
	t_sheet	shoot;
	t_sheet	walk;
	t_sheet	ammo;
	t_sheet	health;
	t_sheet	pill;
	t_sheet	city;
	t_sheet	radar;
	t_audio	audio;	// CHANGE TO ASSETS
	t_sheet	radar_l0;
	t_sheet	radar_l1;
}	t_assets;

typedef struct s_game
{
	t_xvar		*mlx;
	t_frame		frame;
	t_gstate	state;
	t_mat8		map;
	t_entity	player;
	t_entity	enemies[NUM_ENEMIES];
	t_block		blocks[NUM_BLOCKS];	// World, Ceil/Floor, Doors, etc...
	t_cfg		cfg;
	t_assets	assets;
	// NEW
	t_gun		gun;	// CHANGE TO PLAYER
}	t_game;

// 	size_t      key;
// 	size_t		key_down;
// 	size_t      key_up;

// 	// ADDED OLD >>>>>
// 	t_time      time;               // ADDED OLD

// 	bool    mouse_l;              // ADDED OLD
// 	bool    pause;                // ADDED OLD
// 	// <<< TMP KEY INPUTS
// 	// <<<<< ADDED OLD
// }   t_game;

#endif
