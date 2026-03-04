/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_read.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 14:18:12 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/03 14:18:31 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


static inline
int	stt_read_color(t_xvar *mlx, t_mat *texture, const char **filename_ptr)
{
	size_t		i;
	const char	*filename = *filename_ptr;
	uint32_t	color;
	uint32_t	*ptr;

	while (ft_isspace(*filename))
		filename++;
	if (filename[0] == '.' && filename[1] == '/')
		return (stt_read_xpm(mlx, texture, filename_ptr));
	color = ft_strtoargb(filename, filename_ptr);
	ptr = malloc(RENDER_HEIGHT * sizeof(uint32_t));				// Creates a column of colors
	if (ptr == NULL)
		return (-1);	// TODO: Print error
	texture->ptr = (void *) ptr;
	texture->cols = RENDER_HEIGHT;
	texture->rows = 1;
	texture->type_size = sizeof(uint32_t);
	i = 0;
	while (i < RENDER_HEIGHT)
		ptr[i++] = color;
	return (0);
}

static inline
int	stt_match_texture(t_xvar *mlx, t_map *map, const char **filename_ptr)
{
	int			rvalue;
	const char	*str = *filename_ptr;

	rvalue = 0;
	if (str[0] == 'N' && str[1] == 'O')
		rvalue = stt_read_xpm(mlx, map->textures + 0, filename_ptr);
	else if (str[0] == 'E' && str[1] == 'A')
		rvalue = stt_read_xpm(mlx, map->textures + 1, filename_ptr);
	else if (str[0] == 'S' && str[1] == 'O')
		rvalue = stt_read_xpm(mlx, map->textures + 2, filename_ptr);
	else if (str[0] == 'W' && str[1] == 'E')
		rvalue = stt_read_xpm(mlx, map->textures + 3, filename_ptr);
	else if (str[0] == 'F')
		rvalue = stt_read_color(mlx, map->textures + 4, filename_ptr);	// alpha channel is unused in parsing, can signal duplicate state
	else if (str[0] == 'C')
		rvalue = stt_read_color(mlx, map->textures + 5, filename_ptr);	// alpha channel is unused in parsing, can signal duplicate state
	return (rvalue);
}

int	cub_read(const char *str, t_map *map)
{
	size_t	i;
	uint8_t	matches[6];
	int		result;

	i = 0;
	while (str[i] != 0)
	{
		result = stt_match_texture(input + i);
		if (result != 0)
		{
			
		}
	}
}