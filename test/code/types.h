#ifndef TYPES_H
#define TYPES_H

#include "libft_math.h"

typedef struct	s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		l_len;
	int		endian;
}	t_img;

typedef struct	s_cam
{
	t_vecf32	pos;
	float	angle;
	float	dist;
	int		dir;
	int		dist_mod;
}	t_cam;

typedef struct s_player
{
	t_vecf32	pos;
	float	acc;
	float	vel;
	t_vecf32	dir;
	t_vecf32	ori;
	t_cam	cam;
	t_vecf32	coll;
}	t_player;

typedef	struct s_map
{
	float		height;
	float		width;
	int		tile_y;
	int		tile_x;
	char	*grid[500];
}	t_map;

typedef struct s_game
{
	int			fov;
	int			vd;
	int			rc_size;
	t_img		img;
	t_map		map;
	void		*mlx_ptr;
	void		*win_ptr;
	t_player	camera;
	t_player	player;
	long long	t0;
	float		dt;
	int			start;
	int			here;
}	t_game;

#endif
