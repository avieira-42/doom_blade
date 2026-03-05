/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:02:15 by avieira-          #+#    #+#             */
/*   Updated: 2025/04/11 15:27:16 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len_d;
	size_t	len_s;

	i = 0;
	len_d = ft_strlen(dst);
	len_s = ft_strlen(src);
	if (size <= len_d)
		return (size + len_s);
	while (src[i] && i + len_d < size - 1)
	{
		dst[len_d + i] = src[i];
		i++;
	}
	dst[len_d + i] = '\0';
	return (len_d + len_s);
}
/*
#include <stdio.h>
int	main(int argc, char **argv)
{
	char	dst[10];
	int		i;

	i = ft_strlcat(dst, argv[1], 11);
	printf("%s %i", dst, i);
}*/
