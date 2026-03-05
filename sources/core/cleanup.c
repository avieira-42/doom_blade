/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 15:23:59 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/05 16:49:12 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "cmlx.h"
#include "cub_defines.h"
#include "cub_structs.h"

// Cleanup process
static
int	stt_cleanup(t_game *game)
{
	size_t	i;

	free(map->ptr);
	i = 0;
	while (i < NUM_BLOCKS)
	{
		free(map->blocks[i].north.ptr);
		free(map->blocks[i].south.ptr);
		free(map->blocks[i].east.ptr);
		free(map->blocks[i].west.ptr);
	}
}

int	cub_cleanup(t_xvar *mlx, t_map *map)
{
	
}