/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dblylst_size.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <aiviera-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:43:38 by avieira-          #+#    #+#             */
/*   Updated: 2025/06/24 16:59:27 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_dblylst_size(t_dblylst *dblylst)
{
	int				size;
	t_dblylst		*dblylst_iter;

	size = 0;
	dblylst_iter = dblylst;
	if (dblylst == NULL)
		return (size);
	size++;
	if (dblylst->next == dblylst)
		return (size);
	while (dblylst_iter->next != dblylst)
	{
		dblylst_iter = dblylst_iter->next;
		size++;
	}
	return (size);
}
