/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:31:37 by avieira-          #+#    #+#             */
/*   Updated: 2025/06/25 16:36:17 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_is_int(char **array)
{
	while (*array)
	{
		if (ft_atol(*array) > INT_MAX || ft_atol(*(array++)) < INT_MIN)
			return (0);
	}
	return (1);
}
