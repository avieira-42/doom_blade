/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dblylst_new.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:36:36 by avieira-          #+#    #+#             */
/*   Updated: 2025/06/20 22:00:46 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

t_dblylst	*ft_dblylst_new(void *content)
{
	t_dblylst	*new_node;

	new_node = (t_dblylst *) malloc(sizeof(t_dblylst));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}
