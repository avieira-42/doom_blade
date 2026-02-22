#include <unistd.h>
#include <fcntl.h>
#include "types.h"
#include "../libs/libft/include/libft.h"

void	read_map(t_game *game, char *argv1)
{
	size_t	i;
	size_t	j;
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
	game->map.width = ft_strlen(game->map.grid[0]) - 1;
	//printf("w: %f\nh: %f\n", game->map.width, game->map.height);
	game->map.tile_x = 400 / game->map.width;
	game->map.tile_y = 400 / game->map.height;
	//printf("map.tile_x: %i\n", game->map.tile_x);
	//printf("map.tile_y: %i\n", game->map.tile_y);
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
			j++;
		}
		i++;
	}
}

int32_t	parse(t_game *game, char *argv1)
{
	read_map(game, argv1);
	return (0);
}
