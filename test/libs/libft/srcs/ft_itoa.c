/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 09:18:50 by avieira-          #+#    #+#             */
/*   Updated: 2025/04/14 09:43:01 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	calculate_length(int n)
{
	int	i;

	i = 0;
	if (n == -2147483648)
		return (11);
	if (n == 0)
		return (1);
	if (n < 0)
	{
		n = n * -1;
		i++;
	}
	while (n > 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

static char	*populate_array(char *array, int i, int n)
{
	char	c;

	if (n > 9)
	{
		populate_array(array, i - 1, n / 10);
		n = n % 10;
	}
	c = '0' + n;
	array[i] = c;
	return (array);
}

char	*ft_itoa(int n)
{
	int		length;
	char	*array;

	length = calculate_length(n);
	array = malloc(sizeof(char) * (length + 1));
	if (array == NULL)
		return (NULL);
	if (n == -2147483648)
	{
		n = n + 2000000000;
		array[1] = '2';
	}
	if (n < 0)
	{
		n = n * -1;
		array[0] = '-';
	}
	array[length] = '\0';
	populate_array(array, length - 1, n);
	return (array);
}
/*
#include <stdio.h>
int	main(int argc, char *argv[])
{
	int		i;
	char	*array;

	while (1)
	{
		scanf(" %i", &i);
		array = ft_itoa(i);
		printf("%s\n", array);
	}
}*/
