/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:58:18 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/05 12:36:08 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <stdint.h>
# include <stddef.h>
# include "fdf_structs.h"

# define WIDTH 1600
# define HEIGHT 900

uint8_t	fdf_read(t_vars *vars, const char *filename, const char *charset);
uint8_t	fdf_init(t_vars *vars, const char *filename, const char *charset);
void	fdf_render_frame(t_vars *vars);
void	draw_neighbours(t_vars *vars, size_t row, size_t col);

void	fdf_reset(t_vars *vars);
void	fdf_preset1(t_vars *vars);
void	fdf_preset2(t_vars *vars);
void	fdf_preset3(t_vars *vars);
void	fdf_preset4(t_vars *vars);

int		cmlx_loop(t_vars *vars);
int		cmlx_keydown(int keycode, t_vars *vars);
int		cmlx_keyup(int keycode, t_vars *vars);
int		cmlx_mousedown(int button, int32_t x, int32_t y, t_vars *vars);
int		cmlx_mouseup(int button, int32_t x, int32_t y, t_vars *vars);
int		cmlx_mousemove(int32_t x1, int32_t y1, t_vars *vars);


#endif
