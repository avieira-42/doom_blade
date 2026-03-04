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

// typedef union u_vec2
// {
// 	struct
// 	{
// 		float		x;
// 		float		y;
// 	};
// 	struct
// 	{
// 		uint32_t	ux;
// 		uint32_t	uy;
// 	};
// 	struct
// 	{
// 		int32_t		ix;
// 		int32_t		iy;
// 	};
// }	t_vec2;

// typedef union u_vec3
// {
// 	struct
// 	{
// 		float		x;
// 		float		y;
// 		float		z;
// 	};
// 	struct
// 	{
// 		uint32_t	ux;
// 		uint32_t	uy;
// 		uint32_t	uz;
// 	};
// 	struct
// 	{
// 		int32_t		ix;
// 		int32_t		iy;
// 		int32_t		iz;
// 	};
// }	t_vec3;

// typedef union u_vec4
// {
// 	struct
// 	{
// 		float		x;
// 		float		y;
// 		float		z;
// 		float		w;
// 	};
// 	struct
// 	{
// 		uint32_t	ux;
// 		uint32_t	uy;
// 		uint32_t	uz;
// 		uint32_t	uw;
// 	};
// 	struct
// 	{
// 		int32_t		ix;
// 		int32_t		iy;
// 		int32_t		iz;
// 		int32_t		iw;
// 	};
// }	t_vec4;
