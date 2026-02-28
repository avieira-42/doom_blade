/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmlx_open.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 11:33:01 by adeimlin          #+#    #+#             */
/*   Updated: 2026/02/11 11:38:43 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "cub_structs.h"
#include "cub_utils.h"
#include "cmlx.h"
#include "mlx.h"

t_img	cmlx_xpm_to_image(t_xvar *mlx, const char *path)
{
	int		width;
	int		height;
	t_img	*img_ptr;
	t_img	img;

	img.data = NULL;
	img_ptr = mlx_xpm_file_to_image(mlx, path, &width, &height);
	if (img_ptr == NULL)
		return (img);
	img = *img_ptr;
	free(img_ptr);
	return (img);
}
