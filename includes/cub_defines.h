#ifndef CUB_DEFINES_H
# define CUB_DEFINES_H

# define UPSCALE 2

enum e_render
{
	r_width = 640,
	r_height = 360,
	s_width = r_width * UPSCALE,
	s_height = r_height * UPSCALE,
};

# define TEX_SIZE 128
# define NUM_BLOCKS 4
# define EPS 0.00000011920929f
# define PI 3.141592653589793238462643383279502884L
# define PI_DEG 0.01745329251994329576923690768488612713439L

// Game Vars
# define HITREG_AREA 2		// Number of pixels to check in center grid, must be even
# define NEAR_RADIUS 0.1f	// Determines how near for texture culling
# define MELEE_RANGE 0.25f	// Determines range enemy will approach the player and hit
# define PLAYER_WALK_SPEED 0.8f
# define PLAYER_RUN_SPEED 3.2f
# define PLAYER_ACCEL 0.33f
# define ENEMY_SPEED 2.5f
# define ENEMY_ACCEL 0.5f
# define ENEMY_SCALE 1.0f
# define SPEED_THR 4.1f		// Speed threshold where animation goes from normal to fast
# define HALT_THR 0.5f
# define ANIM_TIME 60000	// Time between anim frames

// Difficulty
# define PLAYER_DAMAGE 25
# define ENEMY_ATTACK_DIST 1
# define ENEMY_ATTACK_AGRESS 0.9f
# define ENEMY_DAMAGE 5
# define HEAL_VALUE 40				// Heal on kill value
# define NUM_ENEMIES 32

// RADAR SPECS
# define RADAR_ICON_SIZE 4
# define RADAR_CELL_COLOR rgb_shy_red
# define RADAR_PLAYER_COLOR rgb_shy_green
# define RADAR_ENEMY_COLOR rgb_shy_purple
# define RADAR_BORDER_SIZE 0
# define RADAR_FOV_COLOR rgb_shy_yellow

# define MOUSE_SENS 0.001953125f	// 	1/512
# define RESPAWN_TIMER 8000000
# define AMMO_COUNT 8
# define PLAYER_HEALTH 100
# define RELOAD_CYCLE 4

# define ALPHA_SENTINEL1 0x000021FFFF
# define ALPHA_SENTINEL2 0x000015FFFF

// Mouse Movement
# define OFFSET_MAX 650

enum e_player_state
{
	st_idle = 0,
	st_run = 1 << 0,
	st_sprint = 1 << 1,
	st_reloading = 1 << 2,
	st_shooting = 1 << 3,
	st_interacting = 1 << 4,
	st_shot = 1 << 6,
};

enum e_enemy_state
{
	e_idle = 0,
	e_running = 1 << 0,
	e_seen = 1 << 1,
	e_shooting = 1 << 2,
	e_hit = 1 << 3,
	e_hurt = 1 << 4,	
	e_dying = 1 << 5,
	e_dead = 1 << 6
};

enum e_minimap_state
{
	st_off = 0,
	st_raising = 1 << 0,
	st_checking = 1 << 1,
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
	key_e = 1u << 11,
	key_tab = 1u << 12,
	key_left = 1u << 13,
	key_right = 1u << 14
};

// Colors
enum e_colors
{
	rgb_black = 0x000000u,
	rgb_red = 0xFF0000u,
	rgb_shy_red = 0x330000u,
	rgb_shy_purple = 0x330033,
	rgb_shy_orange = 0x330A00u,
	rgb_green = 0x00FF00u,
	rgb_shy_green = 0x003300u,
	rgb_blue = 0x0000FFu,
	rgb_white = 0xFFFFFFu,
	rgb_yellow = 0xFFFF00u,
	rgb_shy_yellow = 0x555500u,
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
