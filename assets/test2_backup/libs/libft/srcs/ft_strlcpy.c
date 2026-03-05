/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:32:49 by avieira-          #+#    #+#             */
/*   Updated: 2025/04/14 15:29:03 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t			i;
	size_t			len;
	unsigned char	*s;

	i = 0;
	s = (unsigned char *) src;
	len = ft_strlen(src);
	if (size > 0)
	{
		while (*s && i < size - 1)
		{
			*(dst++) = *(s++);
			i++;
		}
		*dst = '\0';
	}
	return (len);
}
