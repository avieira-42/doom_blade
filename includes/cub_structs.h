/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_structs.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 13:14:34 by adeimlin          #+#    #+#             */
/*   Updated: 2026/02/19 14:06:37 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_STRUCTS_H
# define CUB_STRUCTS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct s_game
{
	const char	*cwd;
}	t_game;

typedef struct s_vec2
{
	uint32_t	col;
	uint32_t	row;
}	t_vec2;

typedef struct s_vec2f
{
	float	x;
	float	y;
}	t_vec2f;

typedef union u_vec2
{
	struct
	{
		uint32_t	x;
		uint32_t	y;
	};
	struct
	{
		float	xf;
		float	yf;
	};
}	t_uvec;


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

typedef union u_32
{
	uint32_t	i;
	float		f;
	char		c[4];
}	t_32;

typedef struct s_ret
{
	union
	{
		void		*ptr;
		uint64_t	u64;
		uint32_t	u32;
		uint8_t		u8;
		int64_t		i64;
		int32_t		i32;
		int8_t		i8;
	};
	int64_t	rvalue;
}	t_ret;

typedef struct s_map
{
	char	*ptr;
	size_t	cols;
	size_t	rows;
	t_vec2	player_pos;
	void	*textures[4];	// NESW order
	t_argb	ceil;
	t_argb	floor;
}	t_map;

#endif
