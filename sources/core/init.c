/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 15:30:14 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/05 15:33:48 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cmlx.h"
#include "cub_structs.h"
#include "cub_utils.h"

int	cub_init(t_xvar *mlx, t_map *map)
{
	
	ft_memset(mlx, 0, sizeof(*mlx));
	ft_memset(map, 0, sizeof(*map));
}
