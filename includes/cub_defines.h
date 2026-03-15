#ifndef CUB_DEFINES_H
# define CUB_DEFINES_H

# define SCREEN_WIDTH 1280
# define SCREEN_HEIGHT 720
# define RENDER_WIDTH 640
# define RENDER_HEIGHT 360
# define UPSCALING_FACTOR 2
# define NUM_BLOCKS 4
# define EPS 0.00000011920929f
# define PI 3.141592653589793238462643383279502884L

# define SPRINT_SPEED 0.5f
# define MOVE_SPEED 0.033f

// Keys
enum e_keys
{
	key_esc = 1u,
	key_w = 1u << 1,
	key_s = 1u << 2,
	key_a = 1u << 3,
	key_d = 1u << 4,
	key_space = 1u << 5,
	key_shift = 1u << 6,
	key_ctrl = 1u << 7,
	key_lmb = 1u << 8,
	key_rmb = 1u << 9,
	key_r = 1u << 10,
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
