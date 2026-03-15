#ifndef CUB_UTILS_H
# define CUB_UTILS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include "cub_structs.h"

// Draw and Render
void		ft_bilinear_scaling(t_mat32 *src, t_mat32 *dst);
void		ft_integer_scaling(t_mat32 src, t_mat32 dst, size_t factor);
void		ft_integer_scaling_t(t_mat32 src, t_mat32 dst, size_t factor);
int			ft_transpose(t_mat32 *src);
int			cub_draw_image(t_mat32 src, t_mat32 dst, size_t x_corner, size_t y_corner);
t_rayhit	raycast(float camera_x, t_view *cam, t_mat8 *map, t_block *blocks);
void		render_image(t_view *cam, t_mat8 *map, t_block *blocks, t_mat32 render_frame);

// Events
int		cmlx_keydown(int keycode, t_game *game);
int		cmlx_keyup(int keycode, t_game *game);
int		cmlx_mousedown(int button, int32_t x, int32_t y, t_game *game);
int		cmlx_mouseup(int button, int32_t x, int32_t y, t_game *game);
int		cmlx_mousemove(int32_t x1, int32_t y1, t_game *game);
int		cmlx_loop(t_game *game);

// 
int		cub_init(const char *filename, t_game *game, t_memory *memory);
int		cub_cleanup(t_game *game);
int		cub_read_textures(t_xvar *mlx, const char *str, const char **str_ptr, t_block *blocks);
int		cub_read_map(const char *str, t_mat8 *map, t_entity *player);

// Physics
void	cub_update_pos(t_game *game);

// Utils
long	get_time(void);
uint32_t	ft_strtoargb(const char *str, const char **str_ptr);
void		*ft_read_all(const char *filename, size_t *file_size);
int		cub_is_map_enclosed(t_mat8 map);
float	vec2_dot_product(t_vec2 a, t_vec2 b);
t_vec2	vec2_rotate(t_vec2 vec, float angle);
t_vec2	vec2_norm(t_vec2 vec);


// ------------------------------

void		**ft_free_array(void *array, size_t length);
void		*ft_memset(void *vdst, const uint8_t byte, size_t length);
int64_t		ft_memrcmp(const void *vptr1, const void *vptr2, size_t length);
void		*ft_realloc(void *src, size_t src_size, size_t dst_size);
int64_t		ft_strtol(const char *str, const char **str_ptr);

float	ft_min(float number1, float number2);
float	ft_max(float number1, float number2);
float	ft_abs(float number);
float	ft_absdiff(float number1, float number2);
float	ft_clamp(float number, float value);

int64_t	ft_iabsmax(int64_t number1, int64_t number2);
int64_t	ft_imin(int64_t number1, int64_t number2);
int64_t	ft_imax(int64_t number1, int64_t number2);
int64_t	ft_iabs(int64_t number);
int64_t	ft_iabsdiff(int64_t number1, int64_t number2);

float	ft_qinvsqrt(float number);
float	ft_qsqrt(float number);


// Prototypes: IO ------------------------------------------------------------
char		*get_next_line(int fd);
ssize_t		ft_printf(const char *str, ...);
ssize_t		ft_putstr_fd(char *str, int fd);
ssize_t		ft_putnbr_fd(int64_t number, int fd);
void		ft_putchar_fd(char c, int fd);
void		ft_putendl_fd(char *s, int fd);
// Prototypes: Strings -------------------------------------------------------
size_t		ft_strlen(const char *str);
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
