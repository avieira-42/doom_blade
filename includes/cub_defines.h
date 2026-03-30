#ifndef CUB_DEFINES_H
# define CUB_DEFINES_H

# define SCREEN_WIDTH 1280
# define SCREEN_HEIGHT 720
# define RENDER_WIDTH 640
# define RENDER_HEIGHT 360
# define UPSCALING_FACTOR 2

# define TEX_HEIGHT 256
# define NUM_BLOCKS 4
# define NUM_ENEMIES 1
# define EPS 0.00000011920929f
# define PI 3.141592653589793238462643383279502884L

// Game Vars
# define HITREG_AREA 2		// Number of pixels to check in center grid, must be even
# define NEAR_RADIUS 0.1f	// Determines how near for texture culling
# define MELEE_RANGE 0.25f	// Determines range enemy will approach the player and hit
# define SPRINT_SPEED 0.3f
# define PLAYER_SPEED 4.0f
# define PLAYER_ACCEL 0.33f
# define ENEMY_SPEED 2.5f
# define ENEMY_ACCEL 0.5f
# define SPEED_THR 4.1f		// Speed threshold where animation goes from normal to fast
# define HALT_THR 0.5f		// 
# define ANIM_TIME 60000	// Time between anim frames

# define AMMO_COUNT 8
# define PLAYER_HEALTH 128
# define RELOAD_CYCLE 4
# define PILL_COUNT 1

enum e_audio_channel
{
	ch_steps = 1,
	ch_shot = 2,
	ch_reload = 3,
	ch_no_ammo = 4,
};

// first call of 
enum e_player_state
{
	st_idle = 0,
	st_run = 1 << 0,
	st_sprint = 1 << 1,
	st_reloading = 1 << 2,
	st_shooting = 1 << 3,
	st_interacting = 1 << 4
};

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
	key_p = 1u << 11,
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
