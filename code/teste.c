#include "../libs/minilibx-linux/mlx.h"
#include "types.h"
#include "../libs/libft/include/libft.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int	key_pressed(int keysym, t_game *game)
{
	return (1);
}

int	key_released()
{
	return (1);
}

int	game_loop(void *arg)
{
	t_game *game;

	game = (t_game *)arg;
	printf("x: %i\ny: %i", game->player.pos.x, game->player.pos.y);
	mlx_pixel_put(game->mlx_ptr, game->win_ptr,
			game->player.pos.x, game->player.pos.y, 0xFFFFFF);
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
				printf("x: %i\ny: %i\n", game->player.pos.x, game->player.pos.y);
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
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (parse(&game, argv[1]) == -1)
		return (1);
	game_init(&game);
	mlx_hook(game.win_ptr, 02, (1L << 0), key_pressed, &game);
	mlx_hook(game.win_ptr, 02, (1L << 1), key_released, &game);
	mlx_loop_hook(game.mlx_ptr, game_loop, &game);
	mlx_loop(game.mlx_ptr);
}
