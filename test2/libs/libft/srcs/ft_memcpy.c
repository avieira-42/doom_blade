/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:06:26 by avieira-          #+#    #+#             */
/*   Updated: 2025/04/11 16:01:30 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*d;
	const unsigned char	*s;

	i = 0;
	s = (unsigned char *) src;
	d = (unsigned char *) dest;
	if (dest == NULL && src == NULL)
		return (0);
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}
/*
#include <stdio.h>
int	main(void)
{
	int	i = 0;
	size_t n;
	unsigned int		src[] = {16, 4, 21};
	unsigned int		dest[10];
	
	n = sizeof(int) * 3;
	ft_memcpy(dest, src, n);
	while (i < 3)
	{	
		printf("%i\n", dest[i]);
		i++;
	}
}*/
