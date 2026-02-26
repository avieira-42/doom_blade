/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 17:22:17 by jesusoncrac       #+#    #+#             */
/*   Updated: 2025/05/13 23:50:48 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_removeline(char *buf)
{
	int		i;
	char	*ptr;

	i = 0;
	ptr = buf;
	while (buf[i] != '\n' && buf[i])
		i++;
	ptr += i;
	if (buf[i] == '\n')
		ptr++;
	i = 0;
	while (*ptr)
		buf[i++] = *(ptr++);
	while (buf[i])
		buf[i++] = '\0';
}

char	*ft_writeline(char *line, char *buf)
{
	int		len;
	char	*new_line;
	char	*l_ptr;
	char	*nl_ptr;

	l_ptr = line;
	len = ft_nlen(line) + ft_nlen(buf);
	new_line = (char *) malloc(sizeof(char) * (len + 1));
	if (!new_line)
		return (free(line), NULL);
	nl_ptr = new_line;
	while (l_ptr && *l_ptr)
		*(nl_ptr++) = *(l_ptr++);
	while ((nl_ptr - new_line < len))
		*(nl_ptr++) = *(buf++);
	*nl_ptr = '\0';
	return (free(line), new_line);
}

char	*ft_readline(int fd, char *buf, char *line)
{
	ssize_t		bytes_read;

	while (!ft_found_newline(line))
	{
		if (!*buf)
		{
			bytes_read = read(fd, buf, BUFFER_SIZE);
			if (bytes_read == -1)
				return (free(line), NULL);
			if (bytes_read == 0)
				break ;
		}
		line = ft_writeline(line, buf);
		if (!line)
			return (NULL);
		ft_removeline(buf);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	char			*line;
	static char		*buf[1024];

	line = NULL;
	if (fd < 0 || BUFFER_SIZE < 1 || fd > 1024)
		return (NULL);
	if (!buf[fd])
	{
		buf[fd] = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
		if (!buf[fd])
			return (NULL);
	}
	line = ft_readline(fd, buf[fd], line);
	if (!line || !*line)
	{
		free(buf[fd]);
		buf[fd] = NULL;
	}
	return (line);
}
/*
#include <stdio.h>
#include <fcntl.h>

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;

	(void)argc;
	fd = open(argv[1], O_RDONLY);
	while ((line = get_next_line(fd)))
	{
		printf("%s", line);
		free(line);
	}
	free(line);
}*/
