/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_basic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:50:30 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/16 12:15:59 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#ifndef FT_IO_BUFSIZE
# define FT_IO_BUFSIZE 4096
#endif

// Returns -1 on OOM
ssize_t	ft_read(int fd, void *buffer, void *end, size_t read_size)
{
	ssize_t			bytes_read;
	char			*ptr;
	const ptrdiff_t	bytes_free = (char *)end - (char *)buffer;

	if (bytes_free <= 0)
		return (-(bytes_free < 0));
	ptr = (char *) buffer;
	if ((size_t) bytes_free < read_size)
		read_size = (size_t) bytes_free;
	while (true)
	{
		bytes_read = read(fd, ptr, read_size);
		if (bytes_read > 0)
		{
			read_size -= (size_t) bytes_read;
			ptr += (size_t) bytes_read;
			if (read_size == 0)
				return ((ssize_t)(ptr - (char *)buffer));
		}
		else if (bytes_read == 0)
			break ;
		else if (errno != EINTR)
			return (-1);
	}
	return ((ssize_t)(ptr - (char *)buffer));
}

size_t	ft_read_size(const char *filename)
{
	const int	fd = open(filename, O_RDONLY);
	uint8_t		buffer[16 * 1024];
	uint8_t		*buffer_end;
	ssize_t		bytes_read;
	size_t		bytes_total;

	if (fd < 0)
		return (SIZE_MAX);
	bytes_total = 0;
	buffer_end = buffer + sizeof(buffer);
	bytes_read = ft_read(fd, buffer, buffer_end, sizeof(buffer));
	while (bytes_read > 0)
	{
		bytes_total += (size_t)bytes_read;
		bytes_read = ft_read(fd, buffer, buffer_end, sizeof(buffer));
	}
	close(fd);
	if (bytes_read == 0)
		return (bytes_total);
	else
		return (SIZE_MAX);
}

// Reads an entire file and null terminates it
void	*ft_read_all(const char *filename, size_t *file_size)
{
	int		fd;
	uint8_t	*buffer;
	ssize_t	bytes_read;
	size_t	bytes_total;

	if (file_size != NULL)
		*file_size = SIZE_MAX;
	bytes_total = ft_read_size(filename);
	if (bytes_total == SIZE_MAX)
		return (NULL);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	buffer = malloc(bytes_total + 1);
	if (buffer == NULL)
		return (close(fd), NULL);
	bytes_read = ft_read(fd, buffer, buffer + bytes_total, bytes_total);
	if ((size_t)bytes_read != bytes_total)
		return (close(fd), free(buffer), NULL);
	buffer[bytes_total] = 0;
	if (file_size != NULL)
		*file_size = bytes_total;
	return (close(fd), buffer);
}

// static
// void	*stt_read_file(const char *filename, size_t bytes_total)
// {
// 	const int	fd = open(filename, O_RDONLY);
// 	uint8_t		*buffer;

// 	if (fd < 0)
// 		return (NULL);
// 	buffer = malloc(bytes_total + 1);
// 	if (buffer == NULL)
// 		return (close(fd), NULL);
// 	if (ft_read(fd, buffer, buffer + bytes_total, bytes_total))
// 		return (close(fd), NULL);
// 	close(fd);
// 	buffer[bytes_total] = 0;
// 	return (buffer);
// }

// // TODO: review
// void	*ft_read_all(const char *filename, size_t *file_size)
// {
// 	const int	fd = open(filename, O_RDONLY);
// 	uint8_t		*buffer;
// 	ssize_t		bytes_read;
// 	size_t		bytes_total;

// 	if (fd < 0)
// 		return (NULL);
// 	buffer = malloc(FT_IO_BUFSIZE);
// 	if (buffer == NULL)
// 		return (close(fd), NULL);
// 	bytes_total = 0;
// 	bytes_read = read(fd, buffer, FT_IO_BUFSIZE);
// 	while (bytes_read > 0)
// 	{
// 		bytes_total += (size_t)bytes_read;
// 		bytes_read = read(fd, buffer, FT_IO_BUFSIZE);
// 	}
// 	if (file_size != NULL)
// 		*file_size = (size_t)bytes_total;
// 	if (bytes_read < 0)
// 		return (close(fd), free(buffer), NULL);
// 	buffer[bytes_total] = 0;
// 	if (bytes_total + 1 < FT_IO_BUFSIZE)
// 		return (close(fd), buffer);
// 	return (close(fd), free(buffer), stt_read_file(filename, bytes_total));
// }
