/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_structs.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 15:20:28 by avieira-          #+#    #+#             */
/*   Updated: 2026/03/23 15:27:56 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/* struct that holds the variables
 * need to scroll each spritesheet */


/*typedef struct s_mat32
{
	uint32_t	*ptr;
	uint16_t	rows;
	uint16_t	cols;
	uint16_t	depth;
	uint16_t	flags;
}	t_mat32;*/

typedef struct s_sheet
{
	t_mat32		texture;
	size_t		iterator;
	float		counter;
	uint32_t	*first;
	uint16_t	loops_per_sprite;
	float		delta;
	bool		end;
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


typedef struct   s_anim_old
{
	int32_t     loops_per_sprite; // aprox time for each sprite to be rendered        // ADDED OLD
	int32_t     iterator; // iterator for the spritesheet scrolling                 // ADDED OLD
	int32_t     size;   // number of sprites in sheet                                // ADDED OLD
	int32_t     endian; // endian                                                     // ADDED OLD
	t_img       *sheet; // spritesheet                                                // ADDED OLD
	float       counter; // counter to compare with loops per sprite                  // ADDED OLD
	float        *dt; // delta time (TO REMOVE)                                       // ADDED OLD
	char        *base_path; // broader path name of sprite "sprite" -> "sprite1..."   // ADDED OLD
	char        *file_type; // file type of the sprite "sprite1" -> "sprite1.xpm"     // ADDED OLD
	bool        end; // animation reached its end check                              // ADDED OLD
}    t_anim_old;

typedef struct s_block
{
	t_mat32 north;
	t_mat32 east;
	t_mat32 south;
	t_mat32 west;
}   t_block;

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
	t_view  cam;
	t_speed move;
}   t_entity;

typedef struct s_ray
{
	t_vec2  ray_dir;
	t_vec2  map_pos;    // Integer
	t_vec2  delta_dist;
	t_vec2  side_dist;
	t_vec2  step;       // Integer
}   t_ray;

typedef struct s_rayhit
{
	t_mat32     texture;
	float       perp_dist;
	int32_t     line_height;
}   t_rayhit;

typedef struct s_memory
{
	uint32_t    render_frame[RENDER_WIDTH][RENDER_HEIGHT];
	uint32_t    empty_line[TEX_HEIGHT];
}   t_memory;

typedef struct s_gstate
{
	bool    paused;
	long    time;
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

// hud animations
typedef struct s_hud                // ADDED OLD
{                                   // ADDED OLD
	t_anim_old  *reload;                // ADDED OLD
	t_anim_old  *shoot;                 // ADDED OLD
	t_anim_old  *walk;                  // ADDED OLD
	t_anim_old  *ammo;                  // ADDED OLD
	t_anim_old  *health;                // ADDED OLD
	t_anim_old  *pill;                  // ADDED OLD
	t_anim_old  *city;                  // ADDED OLD
	t_gun   gun;                    // ADDED OLD
	bool    hands_reload;           // ADDED OLD
	bool    hands_shoot;            // ADDED OLD
	bool    shoot_sound;            // ADDED OLD
}   t_hud;                           // ADDED OLD

typedef struct  s_audio             // ADDED OLD
{                                   // ADDED OLD
	Mix_Chunk   *no_ammo;           // ADDED OLD
	Mix_Chunk   *gun_shot;          // ADDED OLD
	Mix_Chunk   *gun_reload;        // ADDED OLD
	Mix_Chunk   *step;              // ADDED OLD
	Mix_Chunk   *step_fast;         // ADDED OLD
	Mix_Chunk   *current_step;      // ADDED OLD
}   t_audio;                          // ADDED OLD

// <<<<< ADDED OLD

typedef struct s_game
{
	t_xvar      *mlx;
	t_img       *img;
	t_mat32     display_frame;
	t_mat32     render_frame;
	t_mat8      map;
	t_vec2      mouse_pos;
	t_entity    player;
	t_block     blocks[NUM_BLOCKS]; // World, Ceil/Floor, Doors, etc...
	size_t      key;
	t_cfg       cfg;
	t_assets	assets;


	// ADDED OLD >>>>>
	t_hud       hud;                // ADDED OLD
	t_time      time;               // ADDED OLD
	t_audio     audio;              // ADDED OLD
	// TMP KEY INPUTS >>>
	bool    w;                    // ADDED OLD
	bool    a;                    // ADDED OLD
	bool    s;                    // ADDED OLD
	bool    d;                    // ADDED OLD
	bool    r;                    // ADDED OLD
	bool    p;                    // ADDED OLD
	bool    shift;                // ADDED OLD
	bool    mouse_l;              // ADDED OLD
	bool    pause;                // ADDED OLD
	bool    w_up;                    // ADDED OLD
	bool    a_up;                    // ADDED OLD
	bool    s_up;                    // ADDED OLD
	bool    d_up;                    // ADDED OLD
	bool    r_up;                    // ADDED OLD
	bool    p_up;                    // ADDED OLD
	bool    shift_up;                // ADDED OLD
	t_img       frame;              // ADDED OLD
	int32_t     frame_endian;       // ADDED OLD
	// <<< TMP KEY INPUTS
	// <<<<< ADDED OLD
}   t_game;

#endif
