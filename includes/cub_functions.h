/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_functions.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 13:46:57 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/05 14:01:42 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_FUNCTIONS_H
# define CUB_FUNCTIONS_H

#include "cub_structs.h"

void	ft_bilinear_scaling(t_mat32 *src, t_mat32 *dst);
int		ft_transpose(t_mat32 *src, void *dst);

#endif