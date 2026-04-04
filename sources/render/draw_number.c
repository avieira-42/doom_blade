#include <stdint.h>
#include <stddef.h>
#include "cub_utils.h"

static inline
void	stt_putargb(t_mat32 *frame, size_t x, size_t y, uint32_t color)
{
	if (x >= frame->width || y >= frame->height)
		return ;
	frame->ptr[y * frame->width + x] = color;
}

static
void	stt_draw_digit(t_mat32 frame, size_t xpos, size_t ypos, int digit)
{
	static const uint8_t	digit_font_5x7[10][7] = {
	{14, 17, 19, 21, 25, 17, 14}, {4, 12, 4, 4, 4, 4, 14},
	{14, 17, 1, 2, 4, 8, 31}, {30, 1, 1, 14, 1, 1, 30},
	{2, 6, 10, 18, 31, 2, 2}, {31, 16, 16, 30, 1, 1, 30},
	{14, 16, 16, 30, 17, 17, 14}, {31, 1, 2, 4, 8, 8, 8},
	{14, 17, 17, 14, 17, 17, 14}, {14, 17, 17, 15, 1, 1, 14}};
	size_t					x;
	size_t					y;
	size_t					row;
	size_t					col;

	row = 0;
	while (row < 7)
	{
		y = ypos + row;
		col = 0;
		while (col < 5)
		{
			x = xpos + col;
			if (digit_font_5x7[digit][row] & (1u << (4 - col)))
				stt_putargb(&frame, x, y, 0xFFFFFFFF);
			col++;
		}
		row++;
	}
}

void	draw_number(t_mat32 frame, size_t xpos, size_t ypos, uint32_t value)
{
	size_t			i;
	char			buffer[16];
	const size_t	length = ft_itoa_r(value, buffer);

	i = 0;
	while (i < length)
	{
		stt_draw_digit(frame, xpos + i * 6, ypos, buffer[i] - '0');
		i++;
	}
}
