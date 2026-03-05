#include <unistd.h>
#include <fcntl.h>
#include "types.h"
#include "../libs/libft/include/libft.h"

void	read_map(t_game *game, char *argv1)
{
	int32_t	i;
	int32_t	j;
	int		fd;

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
	game->map.width = ft_strlen(game->map.grid[0]) - 1;
	game->map.height = i;
	game->map.max_y = SCREEN_X / 3;
	game->map.max_x = SCREEN_Y / 3;
	game->map.tile_x = game->map.max_x / game->map.width;
	game->map.tile_y = game->map.max_y / game->map.height;
	i = 0;
	while (game->map.grid[i])
	{
		j = 0;
		while(game->map.grid[i][j])
		{
			if (game->map.grid[i][j] == 'P')
			{
				game->player.pos.y = i;
				game->player.pos.x = j;
				printf("i: %i, j: %i\n", i, j);
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
