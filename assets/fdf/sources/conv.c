/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:07:21 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/05 17:17:56 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>

typedef struct t_matrix
{
	float	*data;
	size_t	rows;
	size_t	cols;
}	t_mat;

static inline
float	ft_fma(t_mat input, float *kernel)
{
	float	result;

	result = input.data[]
}

void	ft_conv(t_mat input, float *kernel)
{
	
}