/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_defines.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 13:14:34 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/13 16:56:34 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_DEFINES_H
# define CUB_DEFINES_H

# define SCREEN_WIDTH 1280
# define SCREEN_HEIGHT 720
# define RENDER_WIDTH 320
# define RENDER_HEIGHT 180
# define UPSCALING_FACTOR 4
# define NUM_BLOCKS 4
# define EPS 0.00000011920929f
# define PI 3.141592653589793238462643383279502884L

// Keys

enum e_keys
{
	key_w = 1u << 1,
	key_s = 1u << 2,
	key_a = 1u << 3,
	key_d = 1u << 4,
};

// Colors
enum e_colors
{
	rgb_black = 0x000000u,
	rgb_red = 0xFF0000u,
	rgb_green = 0x00FF00u,
	rgb_blue = 0x0000FFu,
	rgb_white = 0xFFFFFFu,
	rgb_yellow = 0xFFFF00u,
	rgb_cyan = 0x00FFFFu,
	rgb_magenta = 0xFF00FFu,
	rgb_gray = 0x808080u,
	rgb_dark_gray = 0x404040u,
	rgb_light_gray = 0xC0C0C0u,
	rgb_maroon = 0x800000u,
	rgb_olive = 0x808000u,
	rgb_navy = 0x000080u,
	rgb_teal = 0x008080u,
	rgb_purple = 0x800080u,
	rgb_orange = 0xFFA500u,
	rgb_gold = 0xFFD700u,
	rgb_pink = 0xFFC0CBu,
	rgb_brown = 0xA52A2Au,
	rgb_sky_blue = 0x87CEEBu,
	rgb_turquoise = 0x40E0D0u,
	rgb_violet = 0xEE82EEu,
	rgb_indigo = 0x4B0082u,
	rgb_beige = 0xF5F5DCu,
	rgb_coral = 0xFF7F50u,
	rgb_salmon = 0xFA8072u,
	rgb_lavender = 0xE6E6FAu,
	rgb_mint = 0x98FF98u
};

#endif
