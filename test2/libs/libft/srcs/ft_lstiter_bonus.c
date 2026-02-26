/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 02:26:03 by jesusoncrac       #+#    #+#             */
/*   Updated: 2025/04/21 00:08:46 by jesusoncrac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
/*
void	ft_upcase(void	*str)
{
	char	*s;

	s = (char *) str;
	while (*s)
	{
		*s = ft_toupper(*s);
		s++;
	}
}

#include <stdio.h>
int	main(int argc, char **argv)
{
	int		i;
	(void)	argc;
	t_list	*lst;
	t_list	*new;
	t_list	*node;

	i = 1;
	lst = NULL;
	while (i < argc)
	{
		new = ft_lstnew(argv[i]);
		if (lst == NULL)
			lst = new;
		else
			ft_lstadd_back(&lst, new);
		i++;
		new = new->next;
	}
	new = NULL;
	node = lst;
	while (node)
	{
		printf("%s ", (char *) node->content);
		node = node->next;
	}
	ft_lstiter(lst, ft_upcase);
	while (lst)
	{
		printf("%s ", (char *) lst->content);
		lst = lst->next;
	}
}*/
