/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_structs.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 13:14:34 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/05 13:36:24 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_STRUCTS_H
# define CUB_STRUCTS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_defines.h"

typedef struct s_game
{
	const char	*cwd;
}	t_game;

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

typedef struct s_block
{
	t_mat32	north;
	t_mat32	east;
	t_mat32	south;
	t_mat32	west;
}	t_block;

typedef struct s_map
{
	uint8_t	*ptr;
	size_t	cols;
	size_t	rows;
	t_vec2	player_pos;
	t_block	blocks[4];	// World, Ceil/Floor, Doors, etc...
}	t_map;

#endif
