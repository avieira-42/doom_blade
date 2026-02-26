/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 00:04:01 by avieira-          #+#    #+#             */
/*   Updated: 2025/04/17 17:27:47 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*ptr;

	ptr = (char *) s + ft_strlen(s);
	while (ptr != s)
	{
		if (*ptr == (unsigned char) c)
			return (ptr);
		ptr--;
	}
	if (*ptr == (unsigned char) c)
		return (ptr);
	return (0);
}
/*
#include <stdio.h>

int	main(int argc, char **argv)
{
	printf("%s", ft_strrchr(argv[1], 'r'));
}*/
