/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printunsign_nbr.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:18:10 by avieira-          #+#    #+#             */
/*   Updated: 2025/05/14 00:17:43 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_printunsign_nbr(unsigned int nbr)
{
	int	count;

	count = 0;
	if (nbr > 9)
		count += ft_printunsign_nbr(nbr / 10);
	nbr = nbr % 10;
	return (count += ft_printchar('0' + nbr));
}
/*
#include <stdio.h>
int	main(int argc, char **argv)
{
unsigned int	i;

while (1)
{
scanf("%u", &i);
printf(", %i", printf("%u", i));
printf("\n");
printf(", %i", ft_printunsign_nbr(i));
printf("\n");
}
}*/
