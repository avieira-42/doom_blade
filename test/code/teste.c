#include "../libs/minilibx-linux/mlx.h"
#include "types.h"
#include "../libs/libft/include/libft.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <X11/keysym.h>
#include <math.h>
#include <stdbool.h>

int	free_displays()
{
	exit(1);
}

long long	time_get()
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		free_displays();
		write(2, "Could not get time!\n", 20);
	}
	return ((long long)(tv.tv_sec * 1000000) + (tv.tv_usec));
}

void	time_delta_get(t_game *game)
{
	game->dt = (time_get() - game->t0) / 1000000.0f;
	game->t0 = time_get();
}

int	key_pressed(int keysym, void *arg)
{
	printf("key %i pressed\n", keysym);
	t_game	*game;

	game = (t_game *)arg;
	if (keysym == XK_w)
		game->player.dir.y += -1;
	if (keysym == XK_a)
		game->player.dir.x += -1;
	if (keysym == XK_s)
		game->player.dir.y += 1;
	if (keysym == XK_d)
		game->player.dir.x += 1;
	if (keysym == XK_i)
		game->origin.dir.y += -1;
	if (keysym == XK_j)
		game->origin.dir.x += -1;
	if (keysym == XK_k)
		game->origin.dir.y += 1;
	if (keysym == XK_l)
		game->origin.dir.x += 1;
	if (keysym == XK_Escape)
		free_displays();
	return (1);
}

int	key_released(int keysym, void *arg)
{
	printf("key %i released\n", keysym);
	t_game	*game;

	game = (t_game *)arg;
	if (keysym == XK_w)
		game->player.dir.y += 1;
	if (keysym == XK_a)
		game->player.dir.x += 1;
	if (keysym == XK_s)
		game->player.dir.y += -1;
	if (keysym == XK_d)
		game->player.dir.x += -1;
	if (keysym == XK_i)
		game->origin.dir.y += 1;
	if (keysym == XK_j)
		game->origin.dir.x += 1;
	if (keysym == XK_k)
		game->origin.dir.y += -1;
	if (keysym == XK_l)
		game->origin.dir.x += -1;
	return (1);
}

void	line_draw(t_vec a, t_vec b, t_game *game, int color)
{
	int			i;
	t_vec const	line = vec_sub(a, b);
	float		x;
	float		y;
	int const	max = vec_max_coord(a);

	if (max != 0)
	{
		x = line.x / max;
		y = line.y / max;
		i = 0;
		while (i < max)
		{
			a.x += x;
			a.y += y;
			mlx_pixel_put(game->mlx_ptr, game->win_ptr, a.x, a.y, color);
			i++;
		}
	}
}

void	line_draw_bresenham_v(t_vec a, t_vec b, t_game *game, int color)
{
	t_vec	line;
	t_vec	tmp;
	int		max;
	int		x;
	int		p;
	int		i;
	int		dir;

	i = -1;
	if (a.y > b.y)
	{
		tmp = a;
		a = b;
		b = tmp;
	}
	line = vec_sub(a, b);
	max = vec_max_coord(line);
	if (line.x < 0)
		dir = -1;
	else
		dir = 1;
	line.x *= dir;
	if (line.y != 0)
	{
		x = a.x;
		p = 2 * line.x - line.y;
		while (++i < max)
		{
			a.y++;
			mlx_pixel_put(game->mlx_ptr, game->win_ptr, x, a.y, color);
			if (p >= 0)
			{
				x += dir;
				p = p - 2 * line.y;
			}
			p = p + 2 * line.x;
		}
	}
}

void	line_draw_bresenham_h(t_vec a, t_vec b, t_game *game, int color)
{
	t_vec	line;
	t_vec	tmp;
	int		max;
	int		y;
	int		p;
	int		i;
	int		dir;

	i = -1;
	if (a.x > b.x)
	{
		tmp = a;
		a = b;
		b = tmp;
	}
	line = vec_sub(a, b);
	max = vec_max_coord(line);
	if (line.y < 0)
		dir = -1;
	else
		dir = 1;
	line.y *= dir;
	if (line.x != 0)
	{
		y = a.y;
		p = 2 * line.y - line.x;
		while (++i < max)
		{
			a.x++;
			mlx_pixel_put(game->mlx_ptr, game->win_ptr, a.x, y, color);
			if (p >= 0)
			{
				y += dir;
				p = p - 2 * line.x;
			}
			p = p + 2 * line.y;
		}
	}
}

void	line_draw_bresenham(t_vec a, t_vec b, t_game *game, int color)
{
	t_vec const	line = vec_sub(a, b);
	if (ft_abs(line.x) >= ft_abs(line.y))
		line_draw_bresenham_h(a, b, game, color);
	else
		line_draw_bresenham_v(a, b, game, color);
}

void	player_move(t_player *player, float dt)
{
	player->pos.x += player->vel * player->dir.x * dt;
	player->pos.y += player->vel * player->dir.y * dt;
}

void	update(t_game *game)
{
	time_delta_get(game);
	player_move(&game->player, game->dt);
	player_move(&game->origin, game->dt);
	printf("dt: %f \n", game->dt);
}

void	render(t_game *game)
{
	mlx_clear_window(game->mlx_ptr, game->win_ptr);
	//line_draw(game->origin.pos, game->player.pos, game, 0xFF00FF);
	line_draw_bresenham(game->origin.pos, game->player.pos, game, 0xFFFFFF);
	printf("x: %f\ny: %f\n", game->player.pos.x, game->player.pos.y);
	mlx_pixel_put(game->mlx_ptr, game->win_ptr,
			game->player.pos.x, game->player.pos.y, 0xFF0000);
	mlx_pixel_put(game->mlx_ptr, game->win_ptr,
			game->origin.pos.x, game->origin.pos.y, 0x00FF00);
}

int	game_loop(void *arg)
{
	t_game *game;

	game = (t_game *)arg;
	update(game);
	render(game);
	return (1);
}

void	read_map(t_game *game, char *argv1)
{
	int	i;
	int	j;
	int fd;

	i = 0;
	fd = open(argv1, O_RDONLY);
	while (1)
	{
		game->map.grid[i] = get_next_line(fd);
		if (game->map.grid[i] == NULL)
			break;
		i++;
	}
	game->map.grid[i] = NULL;
	game->map.height = i;
	game->map.width = strlen(game->map.grid[i-2]);
	game->map.tile_x = 800 / game->map.width;
	game->map.tile_y = 600 / game->map.height;
	printf("width: %i\nheight: %i\n", game->map.width, game->map.height);
	printf("tile_x: %i\ntile_y: %i\n", game->map.tile_x, game->map.tile_y);
	i = 0;
	while (game->map.grid[i])
	{
		j = 0;
		while(game->map.grid[i][j])
		{
			if (game->map.grid[i][j] == 'P')
			{
				game->player.pos.y = i * game->map.tile_y;
				game->player.pos.x = j * game->map.tile_x;
			}
			if (game->map.grid[i][j] == 'O')
			{
				game->origin.pos.y = i * game->map.tile_y;
				game->origin.pos.x = j * game->map.tile_x;
			}
			j++;
		}
		printf("%s", game->map.grid[i]);
		i++;
	}
}

int	parse(t_game *game, char *argv1)
{
	read_map(game, argv1);
	return (0);
}

void	game_init(t_game *game)
{
	game->mlx_ptr = mlx_init();
	game->win_ptr = mlx_new_window(game->mlx_ptr, 800, 800, "doom_blade");
	game->t0 = time_get();
	time_delta_get(game);
	game->player.acc = 10;
	game->player.vel = 300;
	game->player.dir.x = 0;
	game->player.dir.y = 0;
	game->player.ori.x = 0;
	game->player.ori.y = 0;
	game->origin.acc = 10;
	game->origin.vel = 300;
	game->origin.dir.x = 0;
	game->origin.dir.y = 0;
	game->origin.ori.x = 0;
	game->origin.ori.y = 0;
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (parse(&game, argv[1]) == -1)
		return (1);
	game_init(&game);
	mlx_hook(game.win_ptr, 02, (1L << 0), key_pressed, &game);
	mlx_hook(game.win_ptr, 03, (1L << 1), key_released, &game);
	mlx_hook(game.win_ptr, 17, 0, free_displays, &game);
	mlx_loop_hook(game.mlx_ptr, game_loop, &game);
	mlx_loop(game.mlx_ptr);
}
