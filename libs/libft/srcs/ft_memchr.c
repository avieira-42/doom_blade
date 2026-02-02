/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:31:21 by avieira-          #+#    #+#             */
/*   Updated: 2025/04/14 16:06:27 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*b;
	unsigned char	ch;

	b = (unsigned char *) s;
	ch = (unsigned char) c;
	while (n)
	{
		if (*b == ch)
			return (b);
		b++;
		n--;
	}
	return (NULL);
}
