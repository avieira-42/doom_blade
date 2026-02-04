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
	t_player	*player;

	player = (t_player *)arg;
	if (keysym == XK_w)
		player->dir.y += -1;
	if (keysym == XK_a)
		player->dir.x += -1;
	if (keysym == XK_s)
		player->dir.y += 1;
	if (keysym == XK_d)
		player->dir.x += 1;
	if (keysym == XK_Escape)
		free_displays();
	return (1);
}

int	key_released(int keysym, void *arg)
{
	printf("key %i released\n", keysym);
	t_player	*player;

	player = (t_player *)arg;
	if (keysym == XK_w)
		player->dir.y += 1;
	if (keysym == XK_a)
		player->dir.x += 1;
	if (keysym == XK_s)
		player->dir.y += -1;
	if (keysym == XK_d)
		player->dir.x += -1;
	return (1);
}

t_vec	vec_add(t_vec a, t_vec b)
{
	return ((t_vec){a.x + a.y, b.x + b.y});
}

t_vec	vec_sub(t_vec a, t_vec b)
{
	return ((t_vec){b.x - a.x, b.y - a.y});
}

float	vec_mag_sqd(t_vec vec)
{
	return (vec.x * vec.x + vec.y * vec.y);
}

float	fast_inv_sqrt(float n)
{
	long		i;
	float		x2;
	float		y;
	const float	three_halfs = 1.5F;
}

float	vec_mag(t_vec vec)
{
	return (fast_inv_sqrt((double)vec_mag_sqd(vec)));
}

t_vec	vec_unit(t_vec vec, float len, bool sqd)
{
	if (sqd == true)
	{
		vec.x *= vec.x;
		vec.y *= vec.y;
	}
	return ((t_vec){vec.x / len});
}

void	line_draw(t_vec a, t_vec b)
{
	int		i;
	t_vec	line;
	float	mag_sqd;

	line = vec_sub(a, b);
	mag_sqd = vec_mag_sqd(line);

	i = 0;
	while (i < 0)
	{
	}
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
	printf("dt: %f \n", game->dt);
}

void	render(t_game *game)
{
	mlx_clear_window(game->mlx_ptr, game->win_ptr);
	mlx_pixel_put(game->mlx_ptr, game->win_ptr,
			game->player.pos.x, game->player.pos.y, 0xFFFFFF);
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
				printf("i: %i\nj: %i\n", i, j);
				printf("x: %f\ny: %f\n", game->player.pos.x, game->player.pos.y);
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
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (parse(&game, argv[1]) == -1)
		return (1);
	game_init(&game);
	mlx_hook(game.win_ptr, 02, (1L << 0), key_pressed, &game.player);
	mlx_hook(game.win_ptr, 03, (1L << 1), key_released, &game.player);
	mlx_hook(game.win_ptr, 17, 0, free_displays, &game);
	mlx_loop_hook(game.mlx_ptr, game_loop, &game);
	mlx_loop(game.mlx_ptr);
}
