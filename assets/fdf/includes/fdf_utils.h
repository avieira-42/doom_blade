/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 10:50:51 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/05 12:36:11 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_UTILS_H
# define FDF_UTILS_H

# include <stdint.h>
# include <stddef.h>
# include <unistd.h>

float	ft_min(float number1, float number2);
float	ft_max(float number1, float number2);
float	ft_abs(float number);
float	ft_absdiff(float number1, float number2);
int64_t	ft_iabsmax(int64_t number1, int64_t number2);

int64_t	ft_imin(int64_t number1, int64_t number2);
int64_t	ft_imax(int64_t number1, int64_t number2);
int64_t	ft_iabs(int64_t number);
int64_t	ft_iabsdiff(int64_t number1, int64_t number2);

void	*ft_bzero(void *dst_void, size_t length);
uint8_t	*ft_setlut256(uint8_t *lut, const char *charset);
size_t	ft_count_tokens(const char *str, const uint8_t *lut, size_t *bytes);
ssize_t	ft_read_size(const char *filename);

#endif
