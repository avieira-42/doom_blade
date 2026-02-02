/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dblylst_addback.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:40:18 by avieira-          #+#    #+#             */
/*   Updated: 2025/06/20 16:46:26 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h" 

void	ft_dblylst_addback(t_dblylst *dblylst, t_dblylst *new)
{
	t_dblylst	*last;

	last = ft_dblylst_last(dblylst);
	dblylst->prev = new;
	last->next = new;
	new->prev = last;
	new->next = dblylst;
}
