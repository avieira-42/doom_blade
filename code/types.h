#ifndef TYPES_H
#define TYPES_H

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

typedef struct s_player
{
	t_point pos;
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
}	t_game;

#endif
