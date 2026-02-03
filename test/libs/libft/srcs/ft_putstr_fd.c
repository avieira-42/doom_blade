/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 21:36:04 by jesusoncrac       #+#    #+#             */
/*   Updated: 2025/04/15 21:45:46 by jesusoncrac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	while (*s && fd != -1)
	{
		ft_putchar_fd(*s, fd);
		s++;
	}
}
