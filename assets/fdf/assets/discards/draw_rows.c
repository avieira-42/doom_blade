/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_rows.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 13:14:57 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/05 13:14:58 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static
void	draw_cols(t_vars *vars, size_t row, uint8_t mode)
{
	size_t	col;

	if (mode == 0)
	{
		col = 0;
		while (col < vars->cols)
			draw_neighbours(vars, row, col++);
	}
	else
	{
		col = vars->cols;
		while (col > 0)
			draw_neighbours(vars, row, --col);
	}
}

static
void	draw_rows(t_vars *vars, const float rz)
{
	size_t			row;
    const uint8_t	row_fwd = sinf(rz) >= 0.0f;
    const uint8_t	col_fwd = cosf(rz) >= 0.0f;

	if (row_fwd)
	{
		row = 0;
		while (row < vars->rows)
			draw_cols(vars, row++, col_fwd);
	}
	else
	{
		row = vars->rows;
		while (row > 0)
			draw_cols(vars, --row, col_fwd);
	}
}