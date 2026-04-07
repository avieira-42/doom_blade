/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 14:26:12 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/07 16:22:02 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <cub_utils.h>

#define _X20 0x0000000000000000
#define _X21 0x1010101010001000
#define _X22 0x2424000000000000
#define _X23 0x0024247E247E2400
#define _X24 0x083E483C127C0800
#define _X25 0x6264081026460000
#define _X26 0x30485020524C3200
#define _X27 0x1010000000000000
#define _X28 0x0810202020100800
#define _X29 0x1008040404081000
#define _X2A 0x0014083E08140000
#define _X2B 0x000010107C101000
#define _X2C 0x0000000000181020
#define _X2D 0x0000007E00000000
#define _X2E 0x0000000000001800
#define _X2F 0x0204081020408000
#define _X30 0x3C42464A52623C00
#define _X31 0x0818280808083E00
#define _X32 0x3C42020C10207E00
#define _X33 0x3C42021C02423C00
#define _X34 0x040C14247E040400
#define _X35 0x7E407C0202423C00
#define _X36 0x1C20407C42423C00
#define _X37 0x7E02040810101000
#define _X38 0x3C42423C42423C00
#define _X39 0x3C42423E02043800
#define _X3A 0x0000180000180000
#define _X3B 0x0000180000181020
#define _X3C 0x0810204020100800
#define _X3D 0x00007E00007E0000
#define _X3E 0x1008040204081000
#define _X3F 0x3C42020C10001000
#define _X40 0x3C425A5E5A403C00
#define _X41 0x1824427E42424200
#define _X42 0x7C42427C42427C00
#define _X43 0x3C42404040423C00
#define _X44 0x7844424242447800
#define _X45 0x7E40407C40407E00
#define _X46 0x7E40407C40404000
#define _X47 0x3C42404E42423C00
#define _X48 0x4242427E42424200
#define _X49 0x3E08080808083E00
#define _X4A 0x1E04040444443800
#define _X4B 0x4244487048444200
#define _X4C 0x4040404040407E00
#define _X4D 0x42665A4242424200
#define _X4E 0x4262524A46424200
#define _X4F 0x3C42424242423C00
#define _X50 0x7C42427C40404000
#define _X51 0x3C4242424A443A00
#define _X52 0x7C42427C48444200
#define _X53 0x3C42403C02423C00
#define _X54 0x7F08080808080800
#define _X55 0x4242424242423C00
#define _X56 0x4242424242241800
#define _X57 0x424242425A664200
#define _X58 0x4242241824424200
#define _X59 0x4141221408080800
#define _X5A 0x7E02040810207E00
#define _X5B 0x3820202020203800
#define _X5C 0x8040201008040200
#define _X5D 0x3808080808083800
#define _X5E 0x0814220000000000
#define _X5F 0x00000000000000FF
#define _X60 0x1008000000000000
#define _X61 0x00003C023E423E00
#define _X62 0x40405C6242427C00
#define _X63 0x00003C4240423C00
#define _X64 0x02023A4642423E00
#define _X65 0x00003C427E403C00
#define _X66 0x1C20207C20202000
#define _X67 0x00003E42423E023C
#define _X68 0x40405C6242424200
#define _X69 0x0800180808081C00
#define _X6A 0x04000C0404044438
#define _X6B 0x4040444850484400
#define _X6C 0x1808080808081C00
#define _X6D 0x00006C5252525200
#define _X6E 0x00005C6242424200
#define _X6F 0x00003C4242423C00
#define _X70 0x00007C42427C4040
#define _X71 0x00003A46423E0202
#define _X72 0x00005C6240404000
#define _X73 0x00003E403C027C00
#define _X74 0x20207C2020201C00
#define _X75 0x0000424242463A00
#define _X76 0x0000424242241800
#define _X77 0x000042425A5A2400
#define _X78 0x0000422418244200
#define _X79 0x00004242423E023C
#define _X7A 0x00007E0410207E00
#define _X7B 0x0C10106010100C00
#define _X7C 0x1010101010101000
#define _X7D 0x3008080608083000
#define _X7E 0x0000324C00000000
#define _X7F 0x0000000000000000

// This shit is actually norm compliant
static inline
uint64_t	stt_font(uint8_t index)
{
	static const uint64_t	ascii_font[96] = {
		_X20, _X21, _X22, _X23, _X24, _X25, _X26, _X27,
		_X28, _X29, _X2A, _X2B, _X2C, _X2D, _X2E, _X2F,
		_X30, _X31, _X32, _X33, _X34, _X35, _X36, _X37,
		_X38, _X39, _X3A, _X3B, _X3C, _X3D, _X3E, _X3F,
		_X40, _X41, _X42, _X43, _X44, _X45, _X46, _X47,
		_X48, _X49, _X4A, _X4B, _X4C, _X4D, _X4E, _X4F,
		_X50, _X51, _X52, _X53, _X54, _X55, _X56, _X57,
		_X58, _X59, _X5A, _X5B, _X5C, _X5D, _X5E, _X5F,
		_X60, _X61, _X62, _X63, _X64, _X65, _X66, _X67,
		_X68, _X69, _X6A, _X6B, _X6C, _X6D, _X6E, _X6F,
		_X70, _X71, _X72, _X73, _X74, _X75, _X76, _X77,
		_X78, _X79, _X7A, _X7B, _X7C, _X7D, _X7E, _X7F};

	if (index < 32 || index >= 127)
		index = '?';
	return (ascii_font[index - 32]);
}

static
void	stt_draw_char(t_mat32 frame, t_vec2 pos, uint8_t c, uint32_t color)
{
	uint64_t	glyph;
	size_t		row;
	size_t		col;
	uint8_t		row_bits;

	if (c == ' ')
		return ;
	glyph = stt_font(c);
	row = 0;
	frame.ptr += pos.y.u * frame.width + pos.x.u;
	while (row < 8)
	{
		row_bits = (uint8_t)(glyph >> ((7 - row) * 8));
		col = 0;
		while (col < 8)
		{
			if (row_bits & (0x80u >> col))
				frame.ptr[col] = color;
			col++;
		}
		frame.ptr += frame.width;
		row++;
	}
}

static
void	stt_draw_digit(t_mat32 frame, size_t xpos, size_t ypos, int digit)
{
	size_t					x;
	size_t					y;
	size_t					row;
	size_t					col;
	static const uint8_t	font[10][7] = {{14, 17, 19, 21, 25, 17, 14}, \
{4, 12, 4, 4, 4, 4, 14}, {14, 17, 1, 2, 4, 8, 31}, {30, 1, 1, 14, 1, 1, 30}, \
{2, 6, 10, 18, 31, 2, 2}, {31, 16, 16, 30, 1, 1, 30}, \
{14, 16, 16, 30, 17, 17, 14}, {31, 1, 2, 4, 8, 8, 8}, \
{14, 17, 17, 14, 17, 17, 14}, {14, 17, 17, 15, 1, 1, 14}};

	row = 0;
	while (row < 7)
	{
		y = ypos + row;
		col = 0;
		while (col < 5)
		{
			x = xpos + col;
			if (font[digit][row] & (1u << (4 - col)))
				if (x < frame.width && y < frame.height)
					frame.ptr[y * frame.width + x] = 0xFFFFFFFF;
			col++;
		}
		row++;
	}
}

void	draw_ascii(t_mat32 frame, t_vec2 pos, const char *str, uint32_t color)
{
	size_t	i;
	uint8_t	c;

	if (str == NULL || (pos.y.u + 8 > frame.height))
		return ;
	i = 0;
	while (str[i] != 0 && ((pos.x.u + 8) <= frame.width))
	{
		c = (uint8_t) str[i];
		stt_draw_char(frame, pos, c, color);
		pos.x.u += 8;
		i++;
	}
}

void	draw_number(t_mat32 frame, size_t xpos, size_t ypos, uint32_t value)
{
	size_t			i;
	char			buffer[24];
	const size_t	length = ft_itoa_r(value, buffer);

	i = 0;
	while (i < length)
	{
		stt_draw_digit(frame, xpos + i * 6, ypos, buffer[i] - '0');
		i++;
	}
}
