/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:05:23 by jesusoncrac       #+#    #+#             */
/*   Updated: 2025/04/21 00:23:28 by jesusoncrac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (new)
	{
		if (*lst)
			ft_lstlast(*lst)->next = new;
		else
			*lst = new;
	}
}
/*
#include <stdio.h>
int	main(int argc, char **argv)
{
	int		i;
	(void)	argc;
	t_list	*lst;
	t_list	*node;

	i = 1;
	lst = NULL;
	while (i < argc)
	{
		node = ft_lstnew(argv[i]);
		ft_lstadd_back(&lst, node);
		i++;
		node = node->next;
	}
	node = NULL;
	while (lst)
	{
		printf("%s ", (char *) lst->content);
		lst = lst->next;
	}
}*/
