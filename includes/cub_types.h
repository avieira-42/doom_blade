#ifndef CUB_TYPES_H
# define CUB_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

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

typedef struct s_mat8
{
	uint8_t		*ptr;
	uint16_t	width;
	uint16_t	height;
	uint16_t	depth;
	uint16_t	stride;
}	t_mat8;

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
	float 		f;
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

#endif
