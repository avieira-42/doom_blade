/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 19:23:19 by jesusoncrac       #+#    #+#             */
/*   Updated: 2025/06/20 21:00:03 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_found_newline(char *line)
{
	int	i;

	i = 0;
	while (line && line[i])
		if (line[i++] == '\n')
			return (1);
	return (0);
}

int	ft_nlen(char *str)
{
	int	i;

	i = 0;
	while (str && str[i] && str[i] != '\n')
		i++;
	if (str && str[i] == '\n')
		i++;
	return (i);
}
