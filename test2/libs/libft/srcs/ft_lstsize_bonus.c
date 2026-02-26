/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 23:38:27 by jesusoncrac       #+#    #+#             */
/*   Updated: 2025/04/20 02:58:42 by jesusoncrac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_lstsize(t_list *lst)
{
	int	size;

	size = 0;
	while (lst)
	{
		lst = lst->next;
		size++;
	}
	return (size);
}
/*
#include <stdio.h>
int	main(int argc, char **argv)
{
	int		i;
	t_list	*lst;
	t_list	*node;

	i = 1;
	lst = NULL;
	while (i < argc)
	{
		if (lst == NULL)
		{
			node = ft_lstnew(argv[i]);
			lst = node;
		}
		else
		{
			node = ft_lstnew(argv[i]);
			ft_lstadd_front(&lst, node);
		}
		i++;
		node = node->next;
	}
	node = NULL;
	printf("%i", ft_lstsize(lst));
}*/
