/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dblylst_last.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 13:46:59 by a-soeiro          #+#    #+#             */
/*   Updated: 2025/06/21 15:39:08 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

t_dblylst	*ft_dblylst_last(t_dblylst *dblylst)
{
	t_dblylst	*dblylst_iter;

	dblylst_iter = dblylst;
	while (dblylst_iter->next != dblylst)
		dblylst_iter = dblylst_iter->next;
	return (dblylst_iter);
}
