/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 13:14:34 by adeimlin          #+#    #+#             */
/*   Updated: 2026/02/10 11:40:35 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_UTILS_H
# define CUB_UTILS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>

void		**ft_free_array(void *array, size_t length);
void		*ft_memset(void *vdst, const uint8_t byte, size_t length);
int64_t		ft_memrcmp(const void *vptr1, const void *vptr2, size_t length);
void		*ft_realloc(void *src, size_t src_size, size_t dst_size);
ssize_t		ft_error(const char *prefix, const char *suffix, ssize_t rvalue);


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
