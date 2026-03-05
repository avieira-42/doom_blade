/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 00:27:02 by jesusoncrac       #+#    #+#             */
/*   Updated: 2025/04/20 02:58:23 by jesusoncrac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	new->next = *lst;
	*lst = new;
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
		node = node->next;
		i++;
	}
	node = NULL;
	while (lst)
	{
		printf("%s", (char *) lst->content);
		lst = lst->next;
	}
}*/
