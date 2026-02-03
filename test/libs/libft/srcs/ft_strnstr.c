/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:26:26 by avieira-          #+#    #+#             */
/*   Updated: 2025/04/14 07:58:50 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	char	*b;
	char	*l;

	i = 0;
	b = (char *) big;
	l = (char *) little;
	if (l[i] == '\0')
		return (b);
	while (b[i] != '\0' && i < len)
	{
		j = 0;
		while (l[j] == b[i + j] && i + j < len)
		{
			if (l[j + 1] == '\0')
				return (&b[i]);
			j++;
		}
		i++;
	}
	return (0);
}
