/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:11:01 by avieira-          #+#    #+#             */
/*   Updated: 2025/04/19 14:06:19 by jesusoncrac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static int	ft_countwords(char const *s, char c)
{
	int	i;
	int	word;

	i = 0;
	word = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			word++;
		i++;
	}
	return (word);
}

static char	**ft_clear(char **split, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		free(split[j]);
		j++;
	}
	free(split);
	return (NULL);
}

static char	**ft_populate(char **split, char const *s, char sep, int words)
{
	int		i;
	int		len;
	int		start;

	i = 0;
	start = 0;
	while (words--)
	{
		len = 0;
		while (s[start] == sep && s[start])
			start++;
		while (s[start + len] != sep && s[start + len])
			len++;
		split[i] = ft_substr(s, start, len);
		if (!split[i])
			return (ft_clear(split, i));
		start += len + 1;
		i++;
	}
	split[i] = (NULL);
	return (split);
}

char	**ft_split(char const *s, char c)
{
	int		words;
	char	**split;

	if (!s)
		return (NULL);
	words = ft_countwords(s, c);
	split = (char **) malloc(sizeof(char *) * (words + 1));
	if (!split)
		return (NULL);
	return (ft_populate(split, s, c, words));
}
/*
#include <stdio.h>
int	main(int argc, char **argv)
{
	int	i;
	(void) argc;
	char	**split;

	i = 0;
	split = ft_split(argv[1], argv[2][0]);
	while (split[i])
	{
		printf("%s\n", split[i]);
		i++;
	}
	ft_clear(split, ft_countwords(argv[1], argv[2][0]));
}*/
