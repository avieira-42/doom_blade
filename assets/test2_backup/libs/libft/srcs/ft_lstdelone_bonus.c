/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 00:19:50 by jesusoncrac       #+#    #+#             */
/*   Updated: 2025/04/20 16:57:18 by jesusoncrac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	del(lst->content);
	free(lst);
}
/*
void	ft_delcontent(void *content)
{
	free((char *)content);
}

#include <stdio.h>
int	main(int argc, char **argv)
{
	int		i;
	(void)	argc;
	t_list	*lst;
	t_list	*node;
	t_list	*index;
	t_list	*del;
	char	**split;


	i = 0;
	lst = NULL;
	split = ft_split(argv[1], argv[2][0]);
	while (split[i])
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
	index = lst;
	while (index->next)
	{
		if (index->next->content == split[3])
		{
			del = index->next->next;
			ft_lstdelone(index->next, ft_delcontent);
			index->next = del;
		}
		index = index->next;
	}
	while (lst)
	{
		printf("%s ", (char *) lst->content);
		lst = lst->next;
	}
}*/
