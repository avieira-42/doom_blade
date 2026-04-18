#include "cub_structs.h"
#include "cub_utils.h"

// 4 ButtonPress
// 12345 = LMB MMB RMB WHEELUP WHEELDOWN
int	cmlx_mousedown(uint8_t button, t_game *game)
{
	if (button == SDL_BUTTON_LEFT)
		game->state.key |= (size_t) key_lmb;
	return (0);
}

// 5 ButtonRelease
int	cmlx_mouseup(uint8_t button, t_game *game)
{
	if (button == SDL_BUTTON_LEFT)
		game->state.key &= ~(size_t)key_lmb;
	return (0);
}

#ifdef __EMSCRIPTEN__
int cmlx_mousemove(Sint32 xrel, Sint32 yrel, t_game *game)
{
	float dx = (float)xrel * 0.002f;   // adjust sensitivity
	float dy = (float)yrel;

	game->player.cam.dir   = vec2_rotate(game->player.cam.dir, dx);
	game->player.cam.plane = vec2_rotate(game->player.cam.plane, dx);
	game->frame.offset = CLAMP(game->frame.offset + dy, -OFFSET_MAX, OFFSET_MAX);
	return (0);
}
#else
int	cmlx_mousemove(Sint32 x, Sint32 y, t_game *game)
{
	float		dx;
	float		dy;

	dx = (float)(x - ((float)s_width / 2)) * (1.0f / 512.0f);
	dy = ((float)y - ((float)s_height / 2));
	game->player.cam.dir = vec2_rotate(game->player.cam.dir, dx);
	game->player.cam.plane = vec2_rotate(game->player.cam.plane, dx);
	game->frame.offset = CLAMP(game->frame.offset + dy, -OFFSET_MAX, OFFSET_MAX);
	SDL_WarpMouseInWindow(game->window, s_width / 2, s_height / 2);
	return (0);
}
#endif
