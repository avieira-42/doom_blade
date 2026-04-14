#ifndef CUB_UTILS_H
# define CUB_UTILS_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include <unistd.h>
# include <math.h>
# include "cmlx_base.h"
# include "cmlx_draw.h"
# include "cub_structs.h"
# include <SDL2/SDL.h>

// Gameplay
void		cub_update_game(t_game *game, long dt);
void		cub_update_pos(t_game *game, float dt);

// Draw
void		raycast(t_view *cam, t_map *map, t_rayhit *rays);
void		planecast(t_frame *frame, t_mat32 floor, t_mat32 ceil, t_view *cam);
void		cub_draw_viewmodel(t_mat32 frame, t_player *player, t_game *game, long dt);
void		cub_draw_enemies(t_game *game, long dt);
void		cub_draw_world(t_game *game);
void		cub_draw_blood(t_mat32 frame, t_game *game);
void		cub_draw_crosshair(uint32_t *ptr);

// Draw Utils
void		draw_line(t_mat32 frame, t_vec2 p0, t_vec2 p1, uint32_t color);
void		draw_circle(t_mat32 frame, t_quad quad);
void		draw_number(t_mat32 frame, size_t x, size_t y, uint32_t value);
uint8_t		cub_advance_animation(t_sheet *sheet, long dt);
void		cub_draw_texture(t_mat32 frame, t_mat32 image, uint32_t xc, uint32_t yc);
void		cub_draw_sheet(t_mat32 frame, const t_sheet *sheet, uint32_t xc, uint32_t yc);
t_sides		cub_center_clip(t_mat32 frame, t_vec2 draw_pos, t_vec2 size);

// Audio
void		cub_play_audio(t_player *player, t_audio *audio, t_game *game, long dt);

// Input
int			cmlx_keydown(SDL_Scancode keycode, t_game *game);
int			cmlx_keyup(SDL_Scancode keycode, t_game *game);
int			cmlx_mousedown(int button, int32_t x, int32_t y, t_game *game);
int			cmlx_mouseup(int button, int32_t x, int32_t y, t_game *game);
int			cmlx_mousemove(t_game *game);
int			cmlx_loop(t_game *game);
void		input_handler(t_game *game, t_player *player);

// Parsing
int			cub_init(const char *filename, t_game *game, t_memory *memory);
int			cub_cleanup(t_game *game, const char *msg);
int			cub_parse_textures(t_game *game, const char **str_ptr, t_memory *memory);
int			cub_read_map(t_game *game, const char *str, t_map *map, t_player *player);
t_sheet		cub_readsheet(t_game *game, const char *base_path, size_t count, long frame_time);
int			cub_read_xpm(t_game *game, t_mat32 *dst, const char *filename, const char **filename_ptr);

// Prototypes: Cub Utils -----------------------------------------------------
size_t		cub_first_neighbour(t_vec2 pos, t_vec2 dir, uint32_t stride);
void		ft_integer_scaling(t_mat32 src, t_mat32 dst, size_t factor);
void		ft_integer_scaling_t(t_mat32 src, t_mat32 dst, size_t factor);
t_vec2		random_valid_pos(t_map *map);
void		ft_rng_init(void);
uint64_t	ft_rand(void);
float		ft_randf(void);
long		get_time(void);
uint32_t	ft_strtoargb(const char *str, const char **str_ptr);
void		*ft_read_all(const char *filename, size_t *file_size);
t_vec2		vec2_rotate(t_vec2 vec, float angle);
t_vec2		vec2_norm(t_vec2 vec);
t_vec2		vec2_mult(t_vec2 a, float scalar);
float		vec2_dot(t_vec2 a, t_vec2 b);
t_vec2		vec2_sum(t_vec2 a, t_vec2 b);
int32_t		vec2_dist(t_vec2 a, t_vec2 b);
t_vec2		vec2_sub(t_vec2 a, t_vec2 b);
float		vec2_max(t_vec2 vec);
int32_t		vec2_idist(t_vec2 a, t_vec2 b);
float		ft_qinvsqrt(float number);
float		ft_qsqrt(float number);

// Prototypes: Utils ----------------------------------------------------------
size_t		ft_itoa_r(int64_t number, char *ptr);
size_t		ft_strlen(const char *s);
uint8_t		ft_isspace(int32_t c);
void		*ft_memcpy(void *vdst, const void *vsrc, size_t length);
void		*ft_memmove(void *vdst, const void *vsrc, size_t length);
void		*ft_memset(void *vdst, const uint8_t byte, size_t length);
int64_t		ft_strtol(const char *str, const char **str_ptr);
float		ft_min(float number1, float number2);
float		ft_max(float number1, float number2);
float		ft_abs(float number);
float		ft_absdiff(float number1, float number2);
float		ft_absclamp(float number, float value);
int64_t		ft_iabsmax(int64_t number1, int64_t number2);
int64_t		ft_imin(int64_t number1, int64_t number2);
int64_t		ft_imax(int64_t number1, int64_t number2);
int64_t		ft_iabs(int64_t number);
int64_t		ft_iclamp(int64_t value, int64_t min, int64_t max);
#endif
