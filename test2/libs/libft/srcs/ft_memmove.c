/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:24:58 by avieira-          #+#    #+#             */
/*   Updated: 2025/04/11 15:57:20 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*d;
	const unsigned char	*s;

	i = n;
	s = (const unsigned char *) src;
	d = (unsigned char *) dest;
	if (dest == NULL && src == NULL)
		return (0);
	if (dest < src || src + n < dest)
	{
		while (i--)
			*(d++) = *(s++);
	}
	else
	{
		s = s + (n - 1);
		d = d + (n - 1);
		while (n--)
			*d-- = *s--;
	}
	return (dest);
}
/*
#include <stdio.h>

int	main(int argc, char *argv[])
{
	(void) argc;
	if (argv[2][0] == 'a')
		ft_memmove(&argv[1][3], argv[1], 3);
	else if (argv[2][0] == 'b')
		ft_memmove(&argv[1][3], argv[1], 2);
	else if (argv[2][0] == 'c')
		ft_memmove(argv[1], &argv[1][3], 5);
	else if (argv[2][0] == 'd')
		ft_memmove(&argv[1][3], &argv[1][3], 2);
	printf("%s", &argv[1][3]);
}*/
