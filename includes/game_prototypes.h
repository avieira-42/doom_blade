#ifndef GAME_PROTOTYPES_H
# define GAME_PROTOTYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>
#include "game_types.h"

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
uint8_t		cub_advance_animation(t_sheet *sheet, long dt);
void		cub_draw_texture(t_mat32 frame, t_mat32 image, uint32_t xc, uint32_t yc);
void		cub_draw_sheet(t_mat32 frame, const t_sheet *sheet, uint32_t xc, uint32_t yc);
t_sides		cub_center_clip(t_mat32 frame, t_vec2 draw_pos, t_vec2 size);
void		draw_ascii(t_mat32 frame, uint32_t x, uint32_t y, const char *str, uint32_t color);
void		draw_number(t_mat32 frame, size_t xpos, size_t ypos, uint32_t value, uint32_t color);

// Audio
void		cub_play_audio(t_player *player, t_audio *audio, t_game *game, long dt);

// Input
int			input_keydown(SDL_Scancode keycode, t_game *game);
int			input_keyup(SDL_Scancode keycode, t_game *game);
int			input_mouseup(uint8_t button, t_game *game);
int			input_mousedown(uint8_t button, t_game *game);
int			input_mousemove(float xrel, float yrel, t_game *game);
void		input_handler(t_game *game, t_player *player);
void		sdl_input_update(t_game *game);

// Parsing
int			cub_init(const char *filename, t_game *game, t_memory *memory);
int			cub_cleanup(t_game *game, const char *msg);
int			cub_parse_textures(t_game *game, const char **str_ptr);
int			cub_read_map(t_game *game, const char *str, t_map *map, t_player *player);
t_sheet		cub_readsheet(t_game *game, const char *base_path, size_t count, long frame_time);
int			cub_read_texture(t_game *game, t_mat32 *dst, const char *filename, const char **filename_ptr);

// Scaling
void		ft_integer_scaling(t_mat32 src, t_mat32 dst, size_t factor);
void		ft_integer_scaling_t(t_mat32 src, t_mat32 dst, size_t factor);
void		ft_near_scale_t(t_mat32 dst, t_mat32 src);

// Prototypes: Cub Utils -----------------------------------------------------
size_t		cub_first_neighbour(t_vec2 pos, t_vec2 dir, uint32_t stride);

t_vec2		random_valid_pos(t_map *map);
void		ft_rng_init(void);
uint64_t	ft_rand(void);
float		ft_randf(void);
long		get_time(void);
uint32_t	ft_strtoargb(const char *str, const char **str_ptr);
void		*ft_read_all(const char *filename, size_t *file_size);
float		ft_qinvsqrt(float number);
float		ft_qsqrt(float number);

// Prototypes: Vec2 Utils -----------------------------------------------------
t_vec2		vec2_rotate(t_vec2 vec, float angle);
t_vec2		vec2_norm(t_vec2 vec);
t_vec2		vec2_mult(t_vec2 a, float scalar);
float		vec2_dot(t_vec2 a, t_vec2 b);
t_vec2		vec2_sum(t_vec2 a, t_vec2 b);
int32_t		vec2_dist(t_vec2 a, t_vec2 b);
t_vec2		vec2_sub(t_vec2 a, t_vec2 b);
float		vec2_max(t_vec2 vec);
int32_t		vec2_idist(t_vec2 a, t_vec2 b);

// Prototypes: Utils ----------------------------------------------------------
size_t		ft_itoa_r(int64_t number, char *ptr);
int64_t		ft_strtol(const char *str, const char **str_ptr);
#endif
