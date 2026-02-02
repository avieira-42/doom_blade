/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:11:28 by avieira-          #+#    #+#             */
/*   Updated: 2025/04/30 17:46:25 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_printnbr(int nbr)
{
	int		count;
	char	*str;

	str = ft_itoa(nbr);
	count = ft_printstr(str);
	free(str);
	return (count);
}
