#ifndef GAME_TYPES_H
# define GAME_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include "base.h"
#include "game_defines.h"

typedef struct s_str
{
	char	*ptr;
	size_t	length;
}	t_str;

typedef struct s_mat
{
	void		*ptr;
	uint16_t	width;
	uint16_t	height;
	uint16_t	depth;
	uint8_t		type_size;
	uint8_t		flags;	// MSB -> LSB: float, transposed, 
}	t_mat;

typedef struct s_mat32
{
	uint32_t	*ptr;
	uint16_t	width;
	uint16_t	height;
	uint16_t	depth;
	uint16_t	stride;
}	t_mat32;

typedef union t_32
{
	float		f;
	int32_t		i;
	uint32_t	u;
	struct
	{
		uint8_t	b;	// LSB
		uint8_t	g;
		uint8_t	r;
		uint8_t	a;	// MSB
	};
}	t_32;

typedef struct s_vec2
{
	t_32	x;
	t_32	y;
}	t_vec2;

typedef struct s_vec3
{
	t_32	x;
	t_32	y;
	t_32	z;
}	t_vec3;

typedef struct s_vec4
{
	t_32	x;
	t_32	y;
	t_32	z;
	t_32	w;
}	t_vec4;

// 0xAARRGGBB
typedef union u_argb
{
	uint32_t	i;
	struct
	{
		uint8_t	b;
		uint8_t	g;
		uint8_t	r;
		uint8_t	a;
	};
}	t_argb;

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
	t_rayhit	*rays;
	int32_t		offset;
}	t_frame;

typedef struct s_sound
{
	SDL_AudioSpec	spec;
	uint8_t			*buf;
	uint32_t		len;
}	t_sound;

typedef struct s_audio
{
	SDL_AudioDeviceID	device;
	SDL_AudioSpec		device_spec;
	SDL_AudioStream		*stream_shot;
	SDL_AudioStream		*stream_reload;
	SDL_AudioStream		*stream_steps;
	SDL_AudioStream		*stream_no_ammo;
	t_sound				shot;
	t_sound				reload;
	t_sound				step;
	t_sound				step_fast;
	t_sound				no_ammo;
}	t_audio;

typedef struct s_hands
{
	t_sheet	reload;
	t_sheet	shoot;
	t_sheet	idle;
	t_sheet	walk;
	t_sheet	radar;
	t_sheet	radar_l0;
	t_sheet	radar_l1;
}	t_hands;

// Every physics loop, positions are updated taking
// into account the speed vector
typedef struct s_player
{
	t_view		cam;
	t_vec2		speed;
	uint32_t	state;	// Defined by player_state enum
	uint32_t	map;
	t_sheet		*viewmodel[2];	// Current texture being drawn
	t_hands		hands;
	float		spd_forward;
	float		spd_sideway;
	float		spd_max;
	int32_t		health;
	int32_t		ammo;
}	t_player;

typedef struct s_map
{
	uint8_t		*tex_index;
	uint8_t		*state;	// 0: no draw, 255: everything drawn (left to right)
	uint32_t	width;
	uint32_t	height;
	t_vec2		radar_sprite_pos;
	t_vec2		radar_size;
	t_quad		radar_quad;
	t_quad		icon_quad;
}	t_map;

typedef struct s_enemy
{
	t_view		cam;
	t_vec2		speed;
	t_sheet		*model;
	t_sheet		running;
	t_sheet		shooting;
	t_sheet		dying;
	t_sheet		shot;
	uint32_t	state;
	int32_t		health;
	long		respawn_timer;
	float		dist;
}	t_enemy;

typedef struct s_memory
{
	uint32_t	render_frame[R_WIDTH][R_HEIGHT];
	t_rayhit	rays[R_WIDTH];
	uint32_t	textures[NUM_BLOCKS][4][TEX_SIZE][TEX_SIZE];
}	t_memory;

typedef struct s_gstate
{
	bool	paused;
	bool	quit;
	long	time;
	size_t	key;

	// ...
}	t_gstate;

// If adding to assets, remember to add to the free list
typedef struct s_assets
{
	t_sheet	hud_blood; // to relocate
	t_audio	audio;
}	t_assets;

typedef struct s_game
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;
	t_frame			frame;
	t_gstate		state;
	t_map			map;
	t_player		player;
	t_enemy			enemies[NUM_ENEMIES];
	t_block			blocks[NUM_BLOCKS];	// World, Ceil/Floor, Doors, etc...
	t_assets		assets;
}	t_game;

#endif
