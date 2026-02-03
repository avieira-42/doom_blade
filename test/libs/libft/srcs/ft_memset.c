/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 23:40:41 by avieira-          #+#    #+#             */
/*   Updated: 2025/04/10 15:57:30 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*b;

	i = 0;
	b = (unsigned char *) s;
	while (i < n)
	{
		b[i] = c;
		i++;
	}
	return (s);
}
