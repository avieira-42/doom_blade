/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:11:54 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/05 13:04:27 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "fdf_utils.h"
#include "fdf.h"

static
uint8_t	fdf_error(uint8_t error_code, int32_t fd, void *ptr1, void *ptr2)
{
	if (error_code == 0)
		write(2, "Error: Read failure\n", 20);
	if (error_code == 1)
		write(2, "Error: Failed to allocate memory\n", 33);
	if (error_code == 2)
		write(2, "Error: Rows and Cols mismatch\n", 30);
	free(ptr1);
	free(ptr2);
	if (fd > 0)
		close(fd);
	return (1);
}

static
t_vtx	fdf_atoi(const uint8_t *ustr, size_t index)
{
	t_vtx					vtx;
	int32_t					sign;
	static const uint8_t	lut[256] = {
	['0'] = 0, ['1'] = 1, ['2'] = 2, ['3'] = 3, ['4'] = 4,
	['5'] = 5, ['6'] = 6, ['7'] = 7, ['8'] = 8, ['9'] = 9,
	['A'] = 10, ['B'] = 11, ['C'] = 12, ['D'] = 13, ['E'] = 14, ['F'] = 15,
	['a'] = 10, ['b'] = 11, ['c'] = 12, ['d'] = 13, ['e'] = 14, ['f'] = 15};

	sign = (*ustr == '-') - (*ustr != '-');
	ustr += (*ustr == '-') || (*ustr == '+');
	vtx.x = (int32_t) index;
	vtx.z = 0;
	vtx.color = 0;
	while (lut[*ustr] || *ustr == '0')
		vtx.z = vtx.z * 10 - lut[*ustr++];
	vtx.z *= sign;
	if (*ustr++ == ',')
	{
		ustr += (ustr[0] == '0' && (ustr[1] == 'x' || ustr[1] == 'X')) << 1;
		while (lut[*ustr] || *ustr == '0')
			vtx.color = (vtx.color << 4) + lut[*ustr++];
	}
	else
		vtx.color = 0xFFFFFF;
	return (vtx);
}

static
t_vtx	*fdf_split(const char *str, const char *charset, size_t *count)
{
	t_vtx			*array;
	size_t			length;
	uint8_t			lut[256];
	size_t			tokens;
	const uint8_t	*ustr = (const uint8_t *) str;

	tokens = ft_count_tokens(str, ft_setlut256(lut, charset), NULL);
	array = malloc(tokens * (sizeof(t_vtx) + sizeof(t_vec4)));
	if (array == NULL)
		return (NULL);
	*count = 0;
	while (*ustr != 0)
	{
		while (lut[*ustr] == 1)
			ustr++;
		length = 0;
		while (lut[ustr[length]] == 0)
			length++;
		if (length == 0)
			break ;
		array[*count] = fdf_atoi(ustr, *count);
		(*count)++;
		ustr += length;
	}
	return (array);
}

static
uint8_t	fdf_read_init(const char *str, const char c, t_vars *vars)
{
	size_t			i;
	int32_t			y_index;

	vars->rows = 1;
	while (*str != 0)
		vars->rows += (*str++ == c);
	vars->rows -= vars->rows != 1 && str[-1] == c;
	if (vars->rows == 0 || vars->length % vars->rows != 0)
		return (1);
	vars->cols = vars->length / vars->rows;
	vars->min = INT32_MAX;
	vars->max = INT32_MIN;
	i = 0;
	y_index = 0;
	while (i < vars->length)
	{
		vars->max = ft_imax(vars->vtx[i].z, vars->max);
		vars->min = ft_imin(vars->vtx[i].z, vars->min);
		vars->vtx[i].x = (vars->vtx[i].x % vars->cols);
		y_index += (vars->vtx[i].x == 0) && (i != 0);
		vars->vtx[i].y = y_index;
		i++;
	}
	return (0);
}

uint8_t	fdf_read(t_vars *vars, const char *filename, const char *charset)
{
	char			*buffer;
	size_t			bytes_read;
	const size_t	total_memory = (size_t) ft_read_size(filename);
	const int32_t	fd = open(filename, O_RDONLY);

	if ((ssize_t) total_memory <= 0 || fd < 0)
		return (fdf_error(0, fd, NULL, NULL));
	buffer = malloc(total_memory + 1);
	if (buffer == NULL)
		return (fdf_error(1, fd, NULL, NULL));
	bytes_read = (size_t) read(fd, buffer, total_memory);
	if ((ssize_t) bytes_read <= 0 || bytes_read != total_memory)
		return (fdf_error(0, fd, buffer, NULL));
	buffer[bytes_read] = 0;
	vars->vtx = fdf_split(buffer, charset, &(vars->length));
	if (vars->vtx == NULL)
		return (fdf_error(1, fd, buffer, NULL));
	vars->vec = (t_vec4 *)(vars->vtx + vars->length);
	if (fdf_read_init(buffer, '\n', vars))
		return (fdf_error(2, fd, buffer, vars->vtx));
	free(buffer);
	close(fd);
	return (0);
}
