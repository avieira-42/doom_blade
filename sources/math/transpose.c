/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transpose.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 13:19:14 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/05 15:21:16 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "cub_utils.h"

static inline
void	stt_transpose(uint32_t *src, uint32_t *dst, uint32_t cols, uint32_t rows)
{
	uint32_t		i;
	uint32_t		j;

	i = 0;
	while (i < rows)
	{
		j = 0;
		while (j < cols)
		{
			dst[i * cols + j] = src[j * cols + i];
			j++;
		}
		i++;
	}
}

// Possible to remove malloc requirement with block transpose
// Passing a NULL ptr to dst mallocs the required buffer memory
int	ft_transpose(t_mat32 *src, void *dst)
{
	uint32_t		tmp;
	const size_t	mat_size = src->cols * src->rows * sizeof(uint32_t);

	if (dst == NULL)
	{
		dst = malloc(mat_size);
		if (dst == NULL)
			return (-1);
		stt_transpose(src->ptr, dst, src->cols, src->rows);
		ft_memcpy(src->ptr, dst, mat_size);
		free(dst);
	}
	else
	{
		stt_transpose(src->ptr, dst, src->cols, src->rows);
		ft_memcpy(src->ptr, dst, mat_size);
	}
	tmp = src->cols;
	src->cols = src->rows;
	src->rows = tmp;
	return (0);
}
