#include <unistd.h>
#include "libft_math.h"
#include "cub_structs.h"
#include "cub_utils.h"

void	line_draw(t_vecf32 a, t_vecf32 b, t_game *game, int color)
{
	size_t			i;
	t_vecf32 const	line = vec_sub(a, b);
	int		x;
	int		y;
	size_t const	max = vec_max_coord(a);

	if (max != 0)
	{
		x = line.x / max;
		y = line.y / max;
		i = 0;
		while (i < max)
		{
			a.x += x;
			a.y += y;
			frame_pixel_put(game->display_frame, a.x , a.y, color);
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
			frame_pixel_put(game->display_frame, x, a.y, color);
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
			frame_pixel_put(game->display_frame, a.x, y, color);
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
	if (ft_absolute(line.x) >= ft_absolute(line.y))
		line_draw_bresenham_h(a, b, game, color);
	else
		line_draw_bresenham_v(a, b, game, color);
}

void	quad_draw(t_vecf32 a, t_game *game, int color, t_vecf32 len)
{
	int			x;
	int			y;

	y = 0;
	while (y < len.y)
	{
		x = 0;
		while (x < len.x)
		{
			frame_pixel_put(game->display_frame, a.x + x, a.y + y, color);
			x++;
		}
		y++;
	}
}

