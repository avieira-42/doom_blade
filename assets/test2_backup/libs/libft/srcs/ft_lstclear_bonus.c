/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 01:36:52 by jesusoncrac       #+#    #+#             */
/*   Updated: 2025/04/20 22:43:15 by jesusoncrac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*node;
	t_list	*tmp;

	node = *lst;
	while (node)
	{
		tmp = node->next;
		ft_lstdelone(node, del);
		node = tmp;
	}
	*lst = NULL;
}
/*
void	ft_delcontent(void *content)
{
	free((char *) content);
}
#include <stdio.h>
int	main(int argc, char **argv)
{
	int		i;
	(void)	argc;
	t_list	*lst;
	t_list	*node;
	char	**split;

	i = 0;
	lst = NULL;
	split = ft_split(argv[1], argv[2][0]);
	while(split[i])
	{
		node = ft_lstnew(split[i]);
		if (lst == NULL)
			lst = node;
		else
			ft_lstadd_back(&lst, node);
		i++;
		node = node->next;
	}
	node = NULL;
	ft_lstclear(&lst, ft_delcontent);
	free(split);
}*/
