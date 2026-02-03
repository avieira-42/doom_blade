/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_matrix.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:38:25 by avieira-          #+#    #+#             */
/*   Updated: 2025/06/21 15:38:51 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_free_matrix(char **matrix)
{
	char	**matrix_ptr;

	matrix_ptr = matrix;
	while (*matrix)
		free(*(matrix++));
	free(matrix_ptr);
}
