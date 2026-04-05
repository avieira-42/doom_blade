/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 11:31:21 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/05 11:32:40 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include "cub_defines.h"

void	cub_clean_texture(uint32_t *ptr, size_t length)
{
	size_t	i;

	i = 0;
	while (i < length)
	{
		if (ptr[i] == ALPHA_SENTINEL1 || ptr[i] == ALPHA_SENTINEL2)
			ptr[i] = 0;
		i++;
	}
}
