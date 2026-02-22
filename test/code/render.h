#ifndef RENDER_H
#define RENDER_H

# include "types.h"

void	map_img_pixel_put(t_game *game, int x, int y, int color);
void	screen_img_pixel_put(t_game *game, int x, int y, int color);
void	screen_img_column_put(t_game *game, int x, float len, int color);
void	line_draw_bresenham(t_vecf32 a, t_vecf32 b, t_game *game, int color);
void	line_draw_bresenham_v(t_vecf32 a, t_vecf32 b, t_game *game, int color);
void	line_draw_bresenham_h(t_vecf32 a, t_vecf32 b, t_game *game, int color);
void	quad_draw(t_vecf32 a, t_game *game, int color, float scale);
void	fov_draw(t_game *game, t_player player);
void    grid_draw(t_game *game);
void	map_draw(t_game *game);
void	render(t_game *game);
void	window_clear(t_game *game, int color);

#endif
