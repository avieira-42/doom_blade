/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:07:48 by avieira-          #+#    #+#             */
/*   Updated: 2025/04/14 16:32:03 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*array1;
	unsigned char	*array2;

	array1 = (unsigned char *) s1;
	array2 = (unsigned char *) s2;
	if (n > 0)
	{
		while (n)
		{
			if (*array1 != *array2)
				return (*array1 - *array2);
			array1++;
			array2++;
			n--;
		}
	}
	return (0);
}
