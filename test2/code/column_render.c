#include <stdbool.h>
#include "render.h"
#include "types.h"
#include "libft_math.h"

t_column	column_init(t_ray ray, t_player player, int32_t x)
{
	t_column	column;

	// calculate perp_wall_dist
	if (ray.side == 0)
		ray.perp_wall_dist = (ray.p_map_pos.x - ray.p_pos.x
				+ (1 - ray.step_ori.x) / 2.0f) / ray.dir.x;
	else
		ray.perp_wall_dist = (ray.p_map_pos.y - ray.p_pos.y
				+ (1 - ray.step_ori.y) / 2.0f) / ray.dir.y;

	//	calculate height of line to be drawn
	column.height = SCREEN_Y / ray.perp_wall_dist;

	// calculate lowest and highest pixel to fill in current stripe
	column.end.x = x;
	column.start.x = x;
	column.start.y = -column.height / 2 + player.mouse_mov.y / 2;
	column.end.y = column.height / 2 + player.mouse_mov.y / 2;
	if (column.start.x < 0)
		column.start.x = 0;
	if (column.end.y >= SCREEN_Y)
		column.end.y = SCREEN_Y - 1;
	return (column);
}

void	column_render(t_game *game, t_ray ray, t_player player, int32_t x)
{
	t_column const	column = column_init(ray, player, x);

	// Draw Column
	if (ray.color != 0x777777 && ray.color != 0xFF00FF)
	{
		if (ray.dir.x > 0)
		{
			if (ray.dir.y > 0)
				ray.color = RED;
			else
				ray.color = BLUE;
		}
		else
		{
			if (ray.dir.y > 0)
				ray.color = WHITE;
			else
				ray.color = GREEN;
		}
	}
	line_draw_bresenham(column.start, column.end, game, ray.color);
}
