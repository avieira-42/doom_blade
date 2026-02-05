#ifndef TYPES_H
#define TYPES_H

#include "libft_math.h"

typedef struct s_player
{
	t_vec pos;
	float	acc;
	float	vel;
	t_vec	dir;
	t_vec	ori;
}	t_player;

typedef	struct s_map
{
	int		height;
	int		width;
	int		tile_y;
	int		tile_x;
	char	*grid[500];
}	t_map;

typedef struct s_game
{
	t_map		map;
	void		*mlx_ptr;
	void		*win_ptr;
	t_player	origin;
	t_player	player;
	long long	t0;
	float		dt;
}	t_game;

#endif
