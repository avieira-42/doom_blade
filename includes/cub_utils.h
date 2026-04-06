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

// New (to be sorted)
void		cub_update_game(t_game *game, long dt);
void		cub_update_pos(t_game *game, float dt);
void		cub_draw_enemies(t_game *game, long dt);

// Draw and Render
void		raycast(t_view *cam, t_map *map, t_rayhit *rays);
void		planecast(t_frame *frame, t_mat32 floor, t_mat32 ceil, t_view *cam);
t_sheet		*cub_actions(t_player *player, t_map *map, long dt);


// Draw Utils
uint8_t		cub_advance_animation(t_sheet *sheet, long dt);
void		pixel_swap(t_mat32 frame, int32_t x, int32_t y, uint32_t color);
void		quad_draw(t_mat32 frame, t_quad quad);
void		line_draw(t_mat32 frame, t_vec2 a, t_vec2 b, int color);
void		cub_draw_texture(t_mat32 frame, t_mat32 image, uint32_t xc, uint32_t yc);
void		cub_draw_sheet(t_mat32 frame, const t_sheet *sheet, uint32_t xc, uint32_t yc);
t_sides		cub_center_clip(t_mat32 frame, t_vec2 draw_pos, t_vec2 size);

// Draw
void		cub_draw_radar(t_game *game, t_mat32 render, t_hands *hands, long dt);
void		cub_draw_world(t_game *game);
void		draw_number(t_mat32 frame, size_t xpos, size_t ypos, uint32_t value);
void		cub_draw_hands(t_mat32 frame, t_game *game, long dt);
void		cub_draw_blood(t_mat32 frame, t_game *game);
void		cub_draw_crosshair(uint32_t *ptr);

int			ft_transpose(t_mat32 *src);
void		sqr_transpose(uint32_t *src, uint32_t length);
int			ft_transpose_img(uint32_t *ptr, size_t width, size_t height);	// TMP
uint32_t	ft_bilerp_argb(const t_mat32 *src, t_vec2 norm_pos);
uint32_t	ft_bilerp_argb_t(const t_mat32 *src, t_vec2 norm_pos);
void		ft_integer_scaling(t_mat32 src, t_mat32 dst, size_t factor);
void		ft_integer_scaling_t(t_mat32 src, t_mat32 dst, size_t factor);

// Events
int			cmlx_keydown(int keycode, t_game *game);
int			cmlx_keyup(int keycode, t_game *game);
int			cmlx_mousedown(int button, int32_t x, int32_t y, t_game *game);
int			cmlx_mouseup(int button, int32_t x, int32_t y, t_game *game);
int			cmlx_mousemove(t_game *game);
int			cmlx_loop(t_game *game);
void		input_handler(t_game *game, t_player *player);

// init
int			cub_init(const char *filename, t_game *game, t_memory *memory);
int			cub_cleanup(t_game *game, const char *msg);
int			cub_parse_textures(t_game *game, const char *str, const char **str_ptr, t_block *blocks);
int			cub_read_map(t_game *game, const char *str, t_map *map, t_player *player);
t_sheet		cub_readsheet(t_game *game, const char *base_path, size_t count, long frame_time);
void		enemy_init(t_game *game, t_enemy enemies[NUM_ENEMIES]);


// Utils
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
int32_t		vec2_idist(t_vec2 a, t_vec2 b);
int			cub_read_texture(t_xvar *mlx, t_mat32 *texture, const char *filename, const char **filename_ptr);
// ------------------------------

void		**ft_free_array(void *array, size_t length);
void		*ft_memset(void *vdst, const uint8_t byte, size_t length);
int64_t		ft_memrcmp(const void *vptr1, const void *vptr2, size_t length);
void		*ft_realloc(void *src, size_t src_size, size_t dst_size);
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
float		ft_qinvsqrt(float number);
float		ft_qsqrt(float number);

// Prototypes: IO ------------------------------------------------------------
size_t		ft_itoa_r(int64_t number, char *ptr);
char		*get_next_line(int fd);
ssize_t		ft_printf(const char *str, ...);
ssize_t		ft_putstr_fd(char *str, int fd);
ssize_t		ft_putnbr_fd(int64_t number, int fd);
void		ft_putchar_fd(char c, int fd);
void		ft_putendl_fd(char *s, int fd);
// Prototypes: Strings -------------------------------------------------------
size_t		ft_strlen(const char *s);
int32_t		ft_strncmp(const char *str1, const char *str2, size_t length);
size_t		ft_strlcpy(char *dst, const char *src, size_t dst_size);
size_t		ft_strlcat(char *dst, const char *src, size_t dst_size);
// Prototypes: String Bools --------------------------------------------------
uint8_t		ft_isalpha(int32_t c);
uint8_t		ft_isdigit(int32_t c);
uint8_t		ft_isspace(int32_t c);
uint8_t		ft_isprint(int32_t c);
uint8_t		ft_isascii(int32_t c);
char		ft_isalnum(int c);
// Prototypes: String Copy ---------------------------------------------------
char		*ft_strcpy(char *dst, const char *src);
char		*ft_strncpy(char *dst, const char *src, size_t length);
char		*ft_strdup(const char *src);
char		*ft_substr(const char *str, size_t start, size_t length);
// Prototypes: String Find ---------------------------------------------------
const char	*ft_strchr(const char *str, uint8_t c);
const char	*ft_strrchr(const char *str, uint8_t c);
const char	*ft_strstr(const char *str, const char *substr);
const char	*ft_strnstr(const char *str, const char *substr, size_t length);
// Prototypes: String Join ---------------------------------------------------
char		*ft_strcat(char *dst, const char *src);
char		*ft_strncat(char *dst, const char *src, size_t length);
char		*ft_strjoin(const char *src1, const char *src2);
// Prototypes: String Operations ---------------------------------------------
char		**ft_split(const char *str, const char c);
char		*ft_strtrim(const char *str, const char *charset);
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
// Prototypes: String Utils --------------------------------------------------
size_t		ft_count_words(const char *str, const char c);
int64_t		ft_atoi(const char *num_str);
char		*ft_itoa(int64_t number);
char		ft_tolower(int c);
char		ft_toupper(int c);
// Prototypes: Memory --------------------------------------------------------
void		*ft_calloc(size_t arr_size, size_t type_size);
void		*ft_bzero(void *vdst, size_t length);
void		*ft_memcpy(void *vdst, const void *vsrc, size_t length);
void		*ft_memmove(void *vdst, const void *vsrc, size_t length);
int64_t		ft_memcmp(const void *vptr1, const void *vptr2, size_t length);
const void	*ft_memchr(const void *vptr, uint8_t byte, size_t length);

#endif
