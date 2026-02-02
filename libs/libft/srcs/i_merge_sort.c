/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i_merge_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:35:26 by avieira-          #+#    #+#             */
/*   Updated: 2025/06/28 12:14:08 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	i_merge_sort(t_array middle)
{
	t_array	left;
	t_array	right;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (middle.length > 1)
	{
		left.length = middle.length / 2;
		right.length = middle.length / 2 + middle.length % 2;
		left.i_array = malloc(left.length * sizeof(int));
		right.i_array = malloc(right.length * sizeof(int));
		if (!left.i_array || !right.i_array)
			return (free(left.i_array), free(right.i_array));
		while (i < left.length)
			left.i_array[j++] = middle.i_array[i++];
		j = 0;
		while (i < middle.length)
			right.i_array[j++] = middle.i_array[i++];
		i_merge_sort(left);
		i_merge_sort(right);
		i_merge(left, middle, right);
	}
}

void	i_merge(t_array left, t_array middle, t_array right)
{
	int		l;
	int		m;
	int		r;

	l = 0;
	m = 0;
	r = 0;
	while (m < middle.length)
	{
		if (l < left.length && r < right.length
			&& left.i_array[l] <= right.i_array[r])
			middle.i_array[m++] = left.i_array[l++];
		else if (r < right.length && l < left.length
			&& right.i_array[r] <= left.i_array[l])
			middle.i_array[m++] = right.i_array[r++];
		else if (l < left.length && r >= right.length)
			middle.i_array[m++] = left.i_array[l++];
		else if (r < right.length && l >= left.length)
			middle.i_array[m++] = right.i_array[r++];
	}
	free(left.i_array);
	free(right.i_array);
}
/* NEED TO CHECK ALL THE MALLOCS CREATED WITH MERGE SORT
 * HOW TO HANDLE WHEN A LOT OF MALLOCS WERE CREATED RECURSIVELY AND ONE FAILS?*/
/* int  main(int argc, char **argv) {
	int          i;
	int          j;
	t_array      middle;

	i = 1;
	j = 0;
	middle.length = argc - 1;
	middle.i_array = malloc((argc - 1) * sizeof(int));
	while (i < argc) middle.i_array[j++] = ft_atoi(argv[i++]);
	i = 0;
	i_merge_sort(middle);
	while (i < argc - 1)
		ft_printf("%i, ", middle.i_array[i++]);
	free(middle.i_array);
}*/
