#ifndef CMLX_BASE_H
# define CMLX_BASE_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# ifdef __EMSCRIPTEN__
# include <SDL.h>
# include <SDL_image.h>
# include <SDL_mixer.h>
# else
# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include <SDL2/SDL_scancode.h>
# include <SDL2/SDL_mixer.h>
# endif

# include "base.h"

typedef struct s_str
{
	char	*ptr;
	size_t	length;
}	t_str;

typedef struct s_mat
{
	void		*ptr;
	uint16_t	width;
	uint16_t	height;
	uint16_t	depth;
	uint8_t		type_size;
	uint8_t		flags;	// MSB -> LSB: float, transposed, 
}	t_mat;

typedef struct s_mat32
{
	uint32_t	*ptr;
	uint16_t	width;
	uint16_t	height;
	uint16_t	depth;
	uint16_t	stride;
}	t_mat32;

typedef union t_32
{
	float		f;
	int32_t		i;
	uint32_t	u;
	struct
	{
		uint8_t	b;	// LSB
		uint8_t	g;
		uint8_t	r;
		uint8_t	a;	// MSB
	};
}	t_32;

typedef struct s_vec2
{
	t_32	x;
	t_32	y;
}	t_vec2;

typedef struct s_vec3
{
	t_32	x;
	t_32	y;
	t_32	z;
}	t_vec3;

typedef struct s_vec4
{
	t_32	x;
	t_32	y;
	t_32	z;
	t_32	w;
}	t_vec4;

// 0xAARRGGBB
typedef union u_argb
{
	uint32_t	i;
	struct
	{
		uint8_t	b;
		uint8_t	g;
		uint8_t	r;
		uint8_t	a;
	};
}	t_argb;

typedef struct s_event_list
{
	int		mask;
	int		(*hook)();
	void	*param;
}	t_event_list;

typedef struct s_img
{
	int				size_line;
	int				bpp;
	int				width;
	int				height;
	int				type;
	int				format;
	char			*data;
}	t_img;

#endif
