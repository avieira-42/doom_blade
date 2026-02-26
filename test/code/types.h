#ifndef TYPES_H
#define TYPES_H

#include "libft_math.h"

#define SPRITE_X 128
#define SPRITE_Y 128
#define SCREEN_X 1920
#define SCREEN_Y 1024

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
	t_vecf32	plane;
	float		angle;
	float		dist;
	int32_t		dir;
	int32_t		dist_mod;
}	t_cam;

typedef struct s_player
{
	t_vecf32	pos;
	float		acc;
	float		vel;
	t_vecf32	dir;
	t_vecf32	ori;
	t_vecf32	coll;
	t_vecf32	center;
}	t_player;

typedef	struct s_map
{
	float		height;
	float		width;
	int32_t		tile_y;
	int32_t		tile_x;
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

	//	maybe tmp
	int32_t		fov;
	int32_t		vd;
	int32_t		rc_size;
	t_img		map_2d;
	t_img		map_3d;
	long long	t0;
	float		dt;
	int32_t		start;
	int32_t		here;
}	t_game;

#endif
