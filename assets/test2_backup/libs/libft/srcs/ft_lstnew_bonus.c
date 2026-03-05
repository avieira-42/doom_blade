/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 23:52:09 by jesusoncrac       #+#    #+#             */
/*   Updated: 2025/04/20 01:59:41 by jesusoncrac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_node;

	new_node = (t_list *) malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}
/*
#include <stdio.h>
int	main(int argc, char **argv)
{
	(void) argc;
	t_list	*lst;

	lst = NULL;
	if (lst == NULL)
		lst = ft_lstnew(argv[1]);
	printf("%s", (char *)lst->content);
}*/
