#ifndef TYPES_H
#define TYPES_H

#include "libft_math.h"
#include <stdio.h>
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define WHITE 0xFFFFFF
#define SPRITE_X 128
#define SPRITE_Y 128
#define SCREEN_X 1920.0
#define SCREEN_Y 1024.0

typedef enum	e_card
{
	N,
	S,
	E,
	W,
}	t_card;

typedef struct	s_img
{
	void	*img;
	char	*addr;
	int32_t	bpp;
	int32_t	l_len;
	int32_t	endian;
	float	width;
	float	height;;
}	t_img;
 
typedef struct	s_ray
{
}	t_ray;

typedef struct	s_cam
{
	t_vecf32	pos;
	t_vecf32	dir;
	int32_t		half_len;
	float		dist;
	int32_t		dist_mod;
}	t_cam;

typedef struct s_player
{
	t_vecf32	dir;
	t_vecf32	pos;
	t_vecf32	ori;
	int32_t		dir_mod;
	float		acc;
	float		vel;
	float		speed;
	float		speed_mod;
}	t_player;

typedef	struct s_map
{
	float		height;
	float		width;
	float		tile_y;
	float		tile_x;
	float		max_x;
	float		max_y;
	char		*grid[500];
}	t_map;

typedef struct s_game
{
	//	definitive
	t_cam		cam;
	t_player	player;
	t_map		map;
	void		*mlx_ptr;
	void		*win_ptr;
	t_img		frame;

	//	maybe tmp
	int32_t		vd;
	long long	t0;
	float		dt;
	int32_t		start;
	int32_t		here;
}	t_game;

#endif
