#include "../libs/minilibx-linux/mlx.h"
#include "clean.h"
#include "types.h"
#include "ray_cast.h"
#include "render.h"

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
			game->map_3d.img, 0, 0);
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
			game->map_2d.img, 0, 0);
}
