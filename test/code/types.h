#ifndef TYPES_H
#define TYPES_H

typedef struct s_point
{
	float	x;
	float	y;
}	t_point;

typedef struct s_player
{
	t_point pos;
	float	acc;
	float	vel;
	t_point	dir;
	t_point	ori;
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
	t_player	player;
	long long	t0;
	float		dt;
}	t_game;

#endif
