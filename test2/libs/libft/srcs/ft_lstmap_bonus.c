/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:10:35 by avieira-          #+#    #+#             */
/*   Updated: 2025/04/21 12:55:49 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	void	*tmp;
	t_list	*new;
	t_list	*node;

	if (!f || !del)
		return (NULL);
	node = NULL;
	while (lst)
	{
		tmp = f(lst->content);
		new = ft_lstnew(tmp);
		if (!new)
		{
			free(tmp);
			ft_lstclear(&node, del);
			return (NULL);
		}
		ft_lstadd_back(&node, new);
		lst = lst->next;
	}
	return (node);
}
/*
void	*ft_capdup(void *str)
{
	char	*s;
	char	*cap;
	char	*s_cap;

	s = (char *) str;
	if (!str)
		return (NULL);
	cap = (char *) malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!cap)
		return (NULL);
	s_cap = cap;
	while (*s)
	{
		*(cap) = (char) ft_toupper(*(s));
		cap++;
		s++;
	}
	*cap = '\0';
	return (s_cap);
}

void	ft_delete(void *content)
{
	free(content);
}

#include <stdio.h>
int	main(int argc, char **argv)
{
	int		i;
	(void)	argc;
	t_list	*lst;
	t_list	*node;
	t_list	*new_list;

	i = 1;
	lst = NULL;
	while (i < argc)
	{
		node = ft_lstnew(argv[i]);
		if (!lst)
			lst = node;
		else
			ft_lstadd_back(&lst, node);
		i++;
		node = node->next;
	}
	node = NULL;
	new_list = ft_lstmap(lst, ft_capdup, ft_delete);
	while (new_list)
	{
		printf("%s ", (char *) new_list->content);
		new_list = new_list->next;
	}
}*/
