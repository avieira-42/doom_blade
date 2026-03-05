/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_structs.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 13:14:34 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/05 16:47:26 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_STRUCTS_H
# define CUB_STRUCTS_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include "cmlx.h"
# include "cub_defines.h"
# include "cub_types.h"

typedef struct s_block
{
	t_mat32	north;
	t_mat32	east;
	t_mat32	south;
	t_mat32	west;
}	t_block;

typedef struct	s_cam
{
	t_vec2	pos;
	t_vec2	dir;
	int32_t	half_len;
	int32_t	dist_mod;
	float	dist;
}	t_cam;

typedef struct s_player
{
	t_vec2		dir;
	t_vec2		pos;
	t_vec2		ori;
	t_vec2		mouse_mov;
	int32_t		dir_mod;
	float		speed;
	float		speed_mod;
}	t_player;

typedef struct s_game
{
	//	definitive
	t_xvar		*mlx;	// win_ptr is inside mlx already
	// void		*win;
	t_cam		cam;
	t_player	player;
	t_mat8		map;
	t_block		blocks[NUM_BLOCKS];	// World, Ceil/Floor, Doors, etc...
	// t_img	frame;
	//	maybe tmp
	int32_t		vd;
	long long	t0;
	float		dt;
	int32_t		start;
	int32_t		here;
}	t_game;

#endif
