#ifndef CUB_STRUCTS_H
# define CUB_STRUCTS_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include "cmlx_base.h"
# include "cmlx_draw.h"
# include "cub_defines.h"

typedef struct s_hands
{
	t_sheet	reload;
	t_sheet	shoot;
	t_sheet	idle; // TODO: to free
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
	t_mat32		texture;	// Current texture being drawn
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
	uint8_t		*state;	// 0: nothing is drawn, 255: everything is drawn (left to right)
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
	t_mat32		texture;
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
	uint32_t	render_frame[render_width][render_height];
	t_rayhit	rays[render_width];
}	t_memory;

typedef struct s_gstate
{
	bool	paused;
	long	time;
	size_t	key;

	// ...
}	t_gstate;

typedef struct s_audio
{
	Mix_Chunk	*no_ammo;
	Mix_Chunk	*shot;
	Mix_Chunk	*reload;
	Mix_Chunk	*step;
	Mix_Chunk	*step_fast;
}	t_audio;

// If adding to assets, remember to add to the free list
typedef struct s_assets
{
	t_audio	audio;
	t_sheet	hud_blood;
}	t_assets;

typedef struct s_game
{
	t_xvar		*mlx;
	t_frame		frame;
	t_gstate	state;
	t_map		map;
	t_player	player;
	t_enemy		enemies[NUM_ENEMIES];
	t_block		blocks[NUM_BLOCKS];	// World, Ceil/Floor, Doors, etc...
	t_assets	assets;
	const char	*file;
}	t_game;

#endif
