#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct s_mat
{
	void		*ptr;
	uint16_t	rows;
	uint16_t	cols;
	uint16_t	depth;
	uint8_t		type_size;
	uint8_t		flags;	// MSB -> LSB: float, transposed, 
}	t_mat;

typedef struct s_mat32
{
	uint32_t	*ptr;
	uint16_t	rows;
	uint16_t	cols;
	uint16_t	depth;
	uint16_t	flags;
}	t_mat32;

typedef struct s_mat2
{
	uint32_t	p00;
	uint32_t	p01;
	uint32_t	p10;
	uint32_t	p11;
}	t_mat2;

typedef union u_vec2
{
	struct
	{
		float		x;
		float		y;
	};
	struct
	{
		uint32_t	ix;
		uint32_t	iy;
	};
}	t_vec2;

typedef union u_vec3
{
	struct
	{
		float		x;
		float		y;
		float		z;
	};
	struct
	{
		uint32_t	ix;
		uint32_t	iy;
		uint32_t	iz;
	};
}	t_vec3;

typedef union u_vec4
{
	struct
	{
		float		x;
		float		y;
		float		z;
		float		w;
	};
	struct
	{
		uint32_t	ix;
		uint32_t	iy;
		uint32_t	iz;
		uint32_t	iw;
	};
}	t_vec4;

typedef struct s_vecf
{
	float	x;
	float	y;
}	t_vecf;