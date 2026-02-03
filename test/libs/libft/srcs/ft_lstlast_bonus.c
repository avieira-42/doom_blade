/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 00:48:11 by jesusoncrac       #+#    #+#             */
/*   Updated: 2025/04/20 03:13:50 by jesusoncrac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
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
		if (lst == NULL)
			lst = node;
		else
			ft_lstadd_front(&lst, node);
		i++;
		node = node->next;
	}
	node = NULL;
	printf("%s", (char *) ft_lstlast(lst)->content);
}*/
