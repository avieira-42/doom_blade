#ifndef CLEAN_H
#define CLEAN_H

# include "types.h"

void	screen_init(t_game *game);
void	map_init(t_game *game);
void	player_init(t_game *game);
void	cam_init(t_cam *cam, t_player player);

int key_pressed(int keysym, void *arg);
int key_released(int keysym, void *arg);
int mouse_pressed(int keysym, void *arg);
int mouse_released(int keysym, void *arg);
int	mouse_move(int x, int y, void *arg);

# define RADIANS 0.0174533
# define LEFT -1
# define RIGHT 1

int			vec_max_coord(t_vecf32 vec);
float		ft_abs(float n);
float		vec_dot_product(t_vecf32 a, t_vecf32 b);
float		vec_mag_sqd(t_vecf32 vec);
float		vec_inv_mag(t_vecf32 vec);
float		u_rsqrt(float n);
float		vec_scalar_projec(t_vecf32 a, t_vecf32 b);
float		vec_cosin();
float		vec_sin();
float		vec_tan();
t_vecf32	vec_rotate(t_vecf32 vec, float speed, int32_t side);
t_vecf32	vec_prepend_dir(t_vecf32 a);
t_vecf32	vec_proj(t_vecf32 a, t_vecf32 b);
t_vecf32	vec_sum(t_vecf32 a, t_vecf32 b);
t_vecf32	vec_sub(t_vecf32 a, t_vecf32 b);
t_vecf32	vec_unit(t_vecf32 vec);
t_vecf32	vec_dir(t_vecf32 a, t_vecf32 b);
t_vecf32	vec_scalar_mult(t_vecf32 vec, float scalar);


void	read_map(t_game *game, char *argv1);
int32_t	parse(t_game *game, char *argv1);

int32_t	ray_cast_size(t_game *game);
void	ray_cast(t_game *game, t_ray *ray);

void	line_draw_bresenham(t_vecf32 a, t_vecf32 b, t_game *game, int color);
void	line_draw_bresenham_v(t_vecf32 a, t_vecf32 b, t_game *game, int color);
void	line_draw_bresenham_h(t_vecf32 a, t_vecf32 b, t_game *game, int color);
void	quad_draw(t_vecf32 a, t_game *game, int color, t_vecf32 len);
void	space_render(t_game *game);
void	column_render(t_game *game, t_ray ray, t_player player, int32_t x);

#endif
