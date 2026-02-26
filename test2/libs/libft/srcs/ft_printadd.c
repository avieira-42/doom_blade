/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printadd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:47:36 by avieira-          #+#    #+#             */
/*   Updated: 2025/05/14 00:15:04 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_printadd(void *ptr)
{
	unsigned long int	add;

	if (!ptr)
		return (ft_printstr("(nil)"));
	add = (unsigned long int) ptr;
	return (ft_printstr("0x") + ft_printhex(add, 'x'));
}
/*
#include <stdio.h>
int	main(void)
{
	int		a;
	int		*ptr1;
	char	*ptr2;

	a = 1;
	ptr1 = &a;
	ptr2 = "ola";
	printf("%i, %i\n", printf("%p", ptr1), printf("%p", ptr2));
	printf("%i, %i", ft_printadd(ptr1), ft_printadd(ptr2));
}*/
