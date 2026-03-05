#ifndef RENDER_H
#define RENDER_H

# include "types.h"

void	map_img_pixel_put(t_game *game, int x, int y, int color);
void	img_pixel_put(t_game *game, int x, int y, int color);
void	line_draw_bresenham(t_vecf32 a, t_vecf32 b, t_game *game, int color);
void	line_draw_bresenham_v(t_vecf32 a, t_vecf32 b, t_game *game, int color);
void	line_draw_bresenham_h(t_vecf32 a, t_vecf32 b, t_game *game, int color);
void	quad_draw(t_vecf32 a, t_game *game, int color, t_vecf32 len);
void	window_clear(t_game *game, int color);
void	space_render(t_game *game);
void	column_render(t_game *game, t_ray ray, t_player player, int32_t x);

#endif
