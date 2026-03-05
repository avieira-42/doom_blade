/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:12:56 by avieira-          #+#    #+#             */
/*   Updated: 2025/06/20 20:21:49 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

long int	ft_atol(char *nptr)
{
	long int		s;
	long int		nb;
	unsigned int	i;

	s = 1;
	nb = 0;
	i = 0;
	while (nptr[i] == ' ' || (nptr[i] >= '\t' && nptr[i] <= '\r'))
		i++;
	while (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			s *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		nb = nptr[i] - '0' + nb * 10;
		i++;
	}
	return (nb * s);
}
/*
#include <stdio.h>
int	main(int argc, char **argv)
{
	printf("%li", ft_atol(argv[1]));
}*/
