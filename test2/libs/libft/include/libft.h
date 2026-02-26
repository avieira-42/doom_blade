/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:40:52 by avieira-          #+#    #+#             */
/*   Updated: 2025/11/27 02:49:58 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <stdint.h>
# include <stdarg.h>

# define TRUE 1
# define FALSE 0
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 3
# endif

typedef int	t_bool;

typedef struct s_array
{
	int				*i_array;
	char			*c_array;
	int				length;
}	t_array;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_dblylst
{
	void				*content;
	struct s_dblylst	*prev;
	struct s_dblylst	*next;
	struct s_dblylst	*target;
	int					pos;
	int					moves;
	int					target_pos;
}	t_dblylst;

/* FT_PRINTF */

int			ft_printchar(char c);
int			ft_printstr(char *str);
int			ft_printnbr(int nbr);
int			ft_printunsign_nbr(unsigned int nbr);
int			ft_printhex(unsigned long nbr, char c);
int			ft_printadd(void *ptr);
int			ft_printf(const char *format, ...);

/* GET_NEXT_LINE */

int			ft_found_newline(char *line);
int			ft_nlen(char *str);
char		*get_next_line(int fd);
char		*ft_readline(int fd, char *buf, char *line);
char		*ft_writeline(char *line, char *buf);
void		ft_removeline(char *buf);

/* SORTING */

void		i_merge_sort(t_array middle);
void		i_merge(t_array left, t_array middle, t_array right);

/* MEMORY */
void		ft_free_matrix(char **matrix);
void		*ft_memset(void *s, int c, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);
void		ft_bzero(void *s, size_t n);

/* STRING MANIPULATION */

int			ft_toupper(int c);
int			ft_tolower(int c);
char		*ft_strdup(const char *s);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char		**ft_split(char const *s, char c);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
size_t		ft_strlcat(char *dst, const char *src, size_t size);
void		ft_striteri(char *s, void (*f)(unsigned int, char*));

/* CONVERSION */

long int	ft_atol(char *nptr);
int			ft_atoi(const char *nptr);
char		*ft_itoa(int n);

/* VERIFICATION */

int			ft_issign(char c);
int			ft_isspace(char c);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
int			ft_isascii(int c);
int			ft_isprint(int c);
int			ft_is_int(char **array);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
char		*ft_strnstr(const char *big, const char *little, size_t len);
size_t		ft_strlen(const char *s);

/* FILE DESCRIPTING */

void		ft_putnbr_fd(int n, int fd);
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);

/* LIST */

int			ft_lstsize(t_list *lst);
void		ft_lstadd_front(t_list **lst, t_list *new);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstnew(void *content);
t_list		*ft_lstlast(t_list *lst);
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

/* DOUBLY LIST */

int			ft_dblylst_size(t_dblylst *dblylst);
void		ft_dblylst_addback(t_dblylst *dblylst, t_dblylst *new);
void		ft_dblylst_clear(t_dblylst *dblylst);
t_dblylst	*ft_dblylst_new(void *content);
t_dblylst	*ft_dblylst_last(t_dblylst *dblylst);

#endif
