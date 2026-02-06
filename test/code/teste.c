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

void	img_pixel_put(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= 1000 || y < 0 || y >= 1000)
		return ;
	dst = game->img.addr + (y * game->img.l_len + x * (game->img.bpp / 8));
	*(unsigned int *)dst = color;
}

int	ray_cast_size(t_game *game)
{
	return (2 * tanf(game->fov * 0.5) * 2 * game->vd);
}

void	ray_cast(t_game *game, t_vecf a, t_vecf b, int color)
{
}

void	window_clear(t_game *game, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < 1000)
	{
		j = 0;
		while (j < 1000)
		{
			img_pixel_put(game, j, i, color);
			j++;
		}
		i++;
	}
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

int     key_pressed(int keysym, void *arg)
{
        t_game  *game;

        game = (t_game *)arg;
        if (keysym == XK_w)
                game->player.dir.y += -1;
        if (keysym == XK_a)
                game->player.dir.x += -1;
        if (keysym == XK_s)
                game->player.dir.y += 1;
        if (keysym == XK_d)
                game->player.dir.x += 1;
        if (keysym == XK_h)
                game->player.cam.dir += -1;
        if (keysym == XK_l)
                game->player.cam.dir += 1;
        if (keysym == XK_Escape)
                free_displays();
        return (1);
}

int     key_released(int keysym, void *arg)
{
        t_game  *game;

        game = (t_game *)arg;
        if (keysym == XK_w)
                game->player.dir.y += 1;
        if (keysym == XK_a)
                game->player.dir.x += 1;
        if (keysym == XK_s)
                game->player.dir.y += -1;
        if (keysym == XK_d)
                game->player.dir.x += -1;
        if (keysym == XK_h)
                game->player.cam.dir += 1;
        if (keysym == XK_l)
                game->player.cam.dir += -1;
        return (1);
}

void	line_draw(t_vecf a, t_vecf b, t_game *game, int color)
{
	int			i;
	t_vecf const	line = vec_sub(a, b);
	int		x;
	int		y;
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
			img_pixel_put(game, a.x , a.y, color);
			i++;
		}
	}
}

void	line_draw_bresenham_v(t_vecf a, t_vecf b, t_game *game, int color)
{
	t_vecf	line;
	t_vecf	tmp;
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
			img_pixel_put(game, x, a.y, color);
			if (p >= 0)
			{
				x += dir;
				p = p - 2 * line.y;
			}
			p = p + 2 * line.x;
		}
	}
}

void	line_draw_bresenham_h(t_vecf a, t_vecf b, t_game *game, int color)
{
	t_vecf	line;
	t_vecf	tmp;
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
			img_pixel_put(game, a.x, y, color);
			if (p >= 0)
			{
				y += dir;
				p = p - 2 * line.x;
			}
			p = p + 2 * line.y;
		}
	}
}

void	line_draw_bresenham(t_vecf a, t_vecf b, t_game *game, int color)
{
	t_vecf const	line = vec_sub(a, b);
	if (ft_abs(line.x) >= ft_abs(line.y))
		line_draw_bresenham_h(a, b, game, color);
	else
		line_draw_bresenham_v(a, b, game, color);
}

void	quad_draw(t_vecf a, t_game *game, int color, float scale)
{
	int	x;
	int	y;

	y = 0;
	while (y < game->map.tile_y * scale)
	{
		x = 0;
		while (x < game->map.tile_x * scale)
		{
			img_pixel_put(game, (int)a.x + x, (int)a.y + y, color);
			x++;
		}
		y++;
	}
}

void	character_move(t_player *player, float dt)
{
	float	len;

	player->pos.x += player->vel * player->dir.x * dt;
	player->pos.y += player->vel * player->dir.y * dt;
}

void	camera_move(t_vecf player_pos, t_cam *cam)
{
	cam->angle += 0.01 * cam->dir;
	cam->pos.x = player_pos.x + sinf(cam->angle) * cam->dist;
	cam->pos.y = player_pos.y + cosf(cam->angle) * cam->dist;
}

void	update(t_game *game)
{
	time_delta_get(game);
	character_move(&game->camera, game->dt);
	character_move(&game->player, game->dt);
	camera_move(game->player.pos, &game->player.cam);
}

void	map_draw(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while(j < game->map.width)
		{
			if (game->map.grid[i][j] == '1')
				quad_draw((t_vecf){j * game->map.tile_x,
						i * game->map.tile_y},
						game, 0xFFFFFF, 1);
			if (game->map.grid[i][j] == 'W')
				quad_draw((t_vecf){j * game->map.tile_x,
						i * game->map.tile_y},
						game, 0x0000FF, 1);
			j++;
		}
		i++;
	}
}

void	fov_draw(t_game *game)
{
	int			i;
	t_vecf const	start = (t_vecf){game->player.pos.x - game->rc_size * 0.5
													+ game->map.tile_x * 0.5,
								game->player.pos.y + game->vd};

	i = 0;
	while (i < game->rc_size)
	{
		line_draw_bresenham((t_vecf){game->player.pos.x + game->map.tile_x * 0.5,
									game->player.pos.y + game->map.tile_y},
							(t_vecf){start.x + i, start.y}, game, 0xFF0000);
		 i++;
	}
}

void	render(t_game *game)
{
	window_clear(game, 0x000000);
	quad_draw(game->player.pos, game, 0xFF00FF, 1);
	line_draw_bresenham(game->player.pos, game->player.cam.pos, game, 0XFFFFFF);
	//fov_draw(game);
	quad_draw(game->player.cam.pos, game, 0x00FF00, 0.5);
	map_draw(game);
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
			game->img.img, 0, 0);
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
	game->map.tile_x = 1000 / game->map.width;
	game->map.tile_y = 1000 / game->map.height;
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

int	parse(t_game *game, char *argv1)
{
	read_map(game, argv1);
	return (0);
}

void	game_init(t_game *game)
{
	game->mlx_ptr = mlx_init();
	game->win_ptr = mlx_new_window(game->mlx_ptr, 1000, 1000, "doom_blade");
	game->t0 = time_get();
	game->img.img = mlx_new_image(game->mlx_ptr, 1000, 1000);
	game->img.addr = mlx_get_data_addr(
			game->img.img, &game->img.bpp, &game->img.l_len, &game->img.endian);
	time_delta_get(game);
	game->player.acc = 10;
	game->player.vel = 300;
	game->player.dir.x = 0;
	game->player.dir.y = 0;
	game->player.ori.x = 0;
	game->player.ori.y = 0;
	game->fov = 120;
	game->vd = 300;
	game->rc_size = ray_cast_size(game);
	game->player.cam.angle = 0;
	game->player.cam.dist = game->vd;
	game->player.cam.dir = 0;
	camera_move(game->player.pos, &game->player.cam);
	game->camera.acc = 10;
	game->camera.vel = 300;
	game->camera.dir.x = 0;
	game->camera.dir.y = 0;
	game->camera.ori.x = 0;
	game->camera.ori.y = 0;
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
