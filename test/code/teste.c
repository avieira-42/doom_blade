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
#include <stddef.h>
#include <stdint.h>

int	free_displays()
{
	exit(1);
}

void	img_pixel_put(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= 400 || y < 0 || y >= 400)
		return ;
	dst = game->img.addr + (y * game->img.l_len + x * (game->img.bpp / 8));
	*(unsigned int *)dst = color;
}

int32_t	ray_cast_size(t_game *game)
{
	return (2 * tanf(game->fov * 0.5) * 2 * game->vd);
}

void	window_clear(t_game *game, int color)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < 800)
	{
		j = 0;
		while (j < 800)
		{
			img_pixel_put(game, j, i, color);
			j++;
		}
		i++;
	}
}

uint64_t 	time_get()
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		free_displays();
		write(2, "Could not get time!\n", 20);
	}
	return ((uint64_t)(tv.tv_sec * 1000000) + (tv.tv_usec));
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
		game->player.ori.y += -1;
	if (keysym == XK_a)
		game->player.ori.x += -1;
	if (keysym == XK_s)
		game->player.ori.y += 1;
	if (keysym == XK_d)
		game->player.ori.x += 1;
	if (keysym == XK_l)
		game->player.cam.dir += -1;
	if (keysym == XK_j)
		game->player.cam.dir += 1;
	/*if (keysym == XK_k)
		game->player.cam.dist_mod += -1;
	if (keysym == XK_i)
		game->player.cam.dist_mod += 1;*/
	if (keysym == XK_Escape)
		free_displays();
	return (1);
}

int     key_released(int keysym, void *arg)
{
	t_game  *game;

	game = (t_game *)arg;
	if (keysym == XK_w)
		game->player.ori.y += 1;
	if (keysym == XK_a)
		game->player.ori.x += 1;
	if (keysym == XK_s)
		game->player.ori.y += -1;
	if (keysym == XK_d)
		game->player.ori.x += -1;
	if (keysym == XK_l)
		game->player.cam.dir += 1;
	if (keysym == XK_j)
		game->player.cam.dir += -1;
	/*if (keysym == XK_k)
		game->player.cam.dist_mod += 1;
	if (keysym == XK_i)
		game->player.cam.dist_mod += -1;*/
	return (1);
}

void	line_draw(t_vecf32 a, t_vecf32 b, t_game *game, int color)
{
	size_t			i;
	t_vecf32 const	line = vec_sub(a, b);
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

void	line_draw_bresenham_v(t_vecf32 a, t_vecf32 b, t_game *game, int color)
{
	t_vecf32	line;
	t_vecf32	tmp;
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

void	line_draw_bresenham_h(t_vecf32 a, t_vecf32 b, t_game *game, int color)
{
	t_vecf32	line;
	t_vecf32	tmp;
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

void	line_draw_bresenham(t_vecf32 a, t_vecf32 b, t_game *game, int color)
{
	t_vecf32 const	line = vec_sub(a, b);
	if (ft_abs(line.x) >= ft_abs(line.y))
		line_draw_bresenham_h(a, b, game, color);
	else
		line_draw_bresenham_v(a, b, game, color);
}

void	quad_draw(t_vecf32 a, t_game *game, int color, float scale)
{
	int			x;
	int			y;
	int const	border_color = 0xFFFFFF;

	y = 0;
	while (y < game->map.tile_y * scale)
	{
		x = 0;
		while (x < game->map.tile_x * scale)
		{
			img_pixel_put(game, a.x + x, a.y + y, color);
			x++;
		}
		y++;
	}
}

void	character_move(t_game * game, t_player *player, float dt)
{
	float	len;

	player->pos.x += player->vel * player->ori.x * dt;
	player->pos.y += player->vel * player->ori.y * dt;
	player->center.x = player->pos.x + game->map.tile_x * 0.5;
	player->center.y = player->pos.y + game->map.tile_y * 0.5;
}

void	ray_get(t_vecf32 player_pos, t_ray *ray)
{
}

void	camera_move(t_vecf32 player_pos, t_cam *cam)
{
	//cam->dist += 1 * cam->dist_mod; // might add this to be more eerie
	cam->angle += 0.5 * RADIANS * cam->dir;
	cam->pos.x = player_pos.x + sinf(cam->angle) * cam->dist;
	cam->pos.y = player_pos.y + cosf(cam->angle) * cam->dist;
}

t_vecf32	update_player_dir(t_vecf32 player, t_vecf32 cam)
{
	return (vec_unit(vec_sub(player, cam)));
}

		/*For the dda ray_cast algorithm to work, all the coordinates
			must be translated to tile map unit coordinates, with
			the small caveat that theres the necessity of storing
			two different data types of the player position, one
			as integer number, so the collision is found, the other
			as float number, so the length of the ray (to be translated
			at the end) is preserved, due to possibility of multiple
			points of collision with the same Wall.
		For such, the algorithm requires the creatio of several
			variables:
				1.	The direction of the ray that is being casted
						that is already normalized.
				2.	The player position in tile units (float) needed
						to calculate variable 4.
				3.	The player position in tile units (integer)
						that we will increase with each step taken
				4.	The length of the ray respective to each
						axis step taken in the grid
				5.	The axis step orientation, according to the
						respective axis of the ray direction
				6.	A boolean to check for the collision of the
						ray and get out of the casting loop
				7.	The translation of the hit position back to
						world units (float)
				8.	The ray final len to set with whichever axis
						len corresponds to the next collision
						point to break out of the loop in case
						of maximum view distance reached

		while ray < view_distance
			if x_ray <= y_ray
				ray = x_ray
	   			increase x
	   			calculate x_ray
	   		else
				ray = y_ray
	   			increase y
	   			calculate y_ray
	   		check ray collision */
void	ray_cast(t_game *game, t_player player, t_vecf32 r_dir)
{

	t_vecf32	p_pos;
	t_veci32	p_map_pos;
	t_veci32	step_ori;
	t_vecf32	step_size;
	t_vecf32	ray_len;
	t_vecf32	hit_pos;
	float		final_len;
	bool		hit;

	// translate the player pos vec to float tile units
	p_pos.x = player.center.x / game->map.tile_x;
	p_pos.y = player.center.y / game->map.tile_y;
	//printf("p_pos.x: %f\np_pos.y: %f\n", p_pos.x, p_pos.y);

	// translate player pos to integer tile units
	p_map_pos = (t_veci32){p_pos.x, p_pos.y};
	//printf("p_map_pos.x: %i\np_map_pos.y: %i\n", p_map_pos.x, p_map_pos.y);

	// set unit size for each step taken on each grid axis
	step_size.x = 1 / u_rsqrt(1 + (r_dir.y * r_dir.y) / (r_dir.x * r_dir.x));
	step_size.y = 1 / u_rsqrt(1 + (r_dir.x * r_dir.x) / (r_dir.y * r_dir.y));
	//printf("step_size.x: %f\nstep_size.y: %f\n", step_size.x, step_size.y);

	// set unit orientation for each step to take and calculate respective axis starting len
	if (r_dir.x < 0)
	{
		step_ori.x = -1;
		ray_len.x = step_size.x * ft_abs(p_pos.x - p_map_pos.x) - 0.05;
		//printf("step_ori.x: %i\n ray_len.x: %f\n", step_ori.x, ray_len.x);
	}
	else
	{
		step_ori.x = 1;
		ray_len.x = step_size.x * (1 - ft_abs(p_pos.x - p_map_pos.x));
		//printf("step_ori.x: %i\n ray_len.x: %f\n", step_ori.x, ray_len.x);
	}
	if (r_dir.y < 0)
	{
		step_ori.y = -1;
		ray_len.y = step_size.y * (ft_abs(p_pos.y - p_map_pos.y)) - 0.06;
		//printf("step_ori.y: %i\n ray_len.y: %f\n", step_ori.y, ray_len.y);
	}
	else
	{
		step_ori.y = 1;
		ray_len.y = step_size.y * (1 - ft_abs(p_pos.y - p_map_pos.y));
		//printf("step_ori.y: %i\n ray_len.y: %f\n", step_ori.y, ray_len.y);
	}

	hit = false;
	final_len = 0;
	while (!hit && final_len < game->player.cam.dist / game->map.tile_x)
	{
		if (ray_len.x < ray_len.y)
		{
			final_len = ray_len.x;
			p_map_pos.x += step_ori.x;
			ray_len.x += step_size.x;
			hit_pos.x = (p_pos.x + r_dir.x * final_len) * game->map.tile_x;
			hit_pos.y = (p_pos.y + r_dir.y * final_len) * game->map.tile_y;
			//quad_draw(hit_pos, game, 0x0000FF, 0.3);
		}
		else
		{
			final_len = ray_len.y;
			p_map_pos.y += step_ori.y;
			ray_len.y += step_size.y;
			hit_pos.x = (p_pos.x + r_dir.x * final_len) * game->map.tile_x;
			hit_pos.y = (p_pos.y + r_dir.y * final_len) * game->map.tile_y;
			//quad_draw(hit_pos, game, 0xFF00FF, 0.3);
		}

		if (p_map_pos.x < game->map.width && p_map_pos.y < game->map.height
				&& p_map_pos.x >= 0 && p_map_pos.y >= 0)
		{
			if (game->map.grid[p_map_pos.y][p_map_pos.x] == '1')
			{
				hit = true;
			}
		}
	}
	if (hit == true)
	{
		hit_pos.x = (p_pos.x + r_dir.x * final_len) * game->map.tile_x;
		hit_pos.y = (p_pos.y + r_dir.y * final_len) * game->map.tile_y;
		//quad_draw(hit_pos, game, 0xFF0000, 1);
	}
	line_draw_bresenham(game->player.center, hit_pos, game, 0x00FF00);
}

void	update(t_game *game)
{
	time_delta_get(game);
	character_move(game, &game->player, game->dt);
	camera_move(game->player.center, &game->player.cam);
	game->player.dir = update_player_dir(game->player.center, game->player.cam.pos);
}

void    grid_draw(t_game *game)
{
	int	pos;

	pos = 0;
	while (pos <= 400)
	{
		if (pos == 400)
			pos = 399;
		line_draw_bresenham((t_vecf32){pos, 0},
				(t_vecf32){pos, 799},
				game, 0x777777);
		line_draw_bresenham((t_vecf32){0, pos},
				(t_vecf32){799, pos},
				game, 0x777777);
		pos += game->map.tile_x;
	}
}

void	map_draw(t_game *game)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < game->map.width)
	{
		j = 0;
		while(j < game->map.width)
		{
			if (game->map.grid[i][j] == '1')
				quad_draw((t_vecf32){j * game->map.tile_x,
						i * game->map.tile_y},
						game, 0x555555, 1);
			j++;
		}
		i++;
	}
}

void	fov_draw(t_game *game, t_player player)
{
	// HERE
	int32_t		i;
	float		ray_angle;
	t_vecf32	ray_target;
	t_vecf32	ray_dir;

	ray_angle = player.cam.angle - RADIANS * 960 * 0.05;
	i = 0;
	while (i < 1920)
	{
		ray_angle += RADIANS * 0.05;
		ray_target.x = player.pos.x + sinf(ray_angle) * game->vd;
		ray_target.y = player.pos.y + cosf(ray_angle) * game->vd;
		ray_dir = vec_dir(player.pos, ray_target);
		ray_cast(game, player, ray_dir);
		i++;
	}
}

void	render(t_game *game)
{
	window_clear(game, 0x000000);
	/*line_draw_bresenham((t_vecf32){game->player.pos.x + game->map.tile_x * 0.5,
	  game->player.pos.y + game->map.tile_y * 0.5},
	  game->player.cam.pos, game, 0X00FF00);*/
	//quad_draw(game->player.cam.pos, game, 0x00FF00, 0.5);
	map_draw(game);
	grid_draw(game);
	quad_draw(game->player.pos, game, 0xFF00FF, 1);
	ray_cast(game, game->player, game->player.dir);
	fov_draw(game, game->player);
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
	game->map.width = strlen(game->map.grid[0]) - 1;
	//printf("w: %f\nh: %f\n", game->map.width, game->map.height);
	game->map.tile_x = 400 / game->map.width;
	game->map.tile_y = 400 / game->map.height;
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

	static
int32_t	stt_parse(t_game *game, char *argv1)
{
	read_map(game, argv1);
	return (0);
}

void	game_init(t_game *game)
{
	game->mlx_ptr = mlx_init();
	game->win_ptr = mlx_new_window(game->mlx_ptr, 1920, 1080, "doom_blade");
	game->t0 = time_get();
	game->img.img = mlx_new_image(game->mlx_ptr, 400, 400);
	game->img.addr = mlx_get_data_addr(
			game->img.img, &game->img.bpp, &game->img.l_len, &game->img.endian);
	time_delta_get(game);
	//game->player = (t_player){.acc = 10, .vel = 300, .ori = (t_vecf32){0, 0}};
	game->player.acc = 10;
	game->player.vel = 300;
	game->player.ori.x = 0;
	game->player.ori.y = 0;
	game->player.dir.x = 0;
	game->player.dir.y = 0;
	game->here = 0;
	game->fov = 90 * RADIANS;
	game->vd = 800;
	game->rc_size = ray_cast_size(game);
	game->player.cam.dist_mod = 0;
	game->player.cam.angle = 15;
	game->player.cam.dist = game->vd;
	game->player.cam.dir = 0;
	game->player.center.x = 0;
	game->player.center.x = 0;
	camera_move(game->player.pos, &game->player.cam);
	game->camera.acc = 10;
	game->camera.vel = 300;
	game->camera.dir.x = 0;
	game->camera.dir.y = 0;
	game->camera.ori.x = 0;
	game->camera.ori.y = 0;
	game->start = 0;
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (stt_parse(&game, argv[1]) == -1)
		return (1);
	game_init(&game);
	mlx_hook(game.win_ptr, 02, (1L << 0), key_pressed, &game);
	mlx_hook(game.win_ptr, 03, (1L << 1), key_released, &game);
	mlx_hook(game.win_ptr, 17, 0, free_displays, &game);
	mlx_loop_hook(game.mlx_ptr, game_loop, &game);
	mlx_loop(game.mlx_ptr);
}
