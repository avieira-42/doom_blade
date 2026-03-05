/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printstr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:10:09 by avieira-          #+#    #+#             */
/*   Updated: 2025/05/14 00:17:30 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_printstr(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (ft_printstr("(null)"));
	while (*str)
		i += ft_printchar(*str++);
	return ((i));
}
