#include "game_types.h"
#include "game_prototypes.h"

int	input_mousedown(uint8_t button, t_game *game)
{
	if (button == SDL_BUTTON_LEFT)
		game->state.key |= (size_t)key_lmb;
	return (0);
}

int	input_mouseup(uint8_t button, t_game *game)
{
	if (button == SDL_BUTTON_LEFT)
		game->state.key &= ~(size_t)key_lmb;
	return (0);
}

int input_mousemove(float xrel, float yrel, t_game *game)
{
	const float dx = xrel / 512.0f;
	const float dy = yrel;

	game->player.cam.dir = vec2_rotate(game->player.cam.dir, dx);
	game->player.cam.plane = vec2_rotate(game->player.cam.plane, dx);
	game->frame.offset = CLAMP(game->frame.offset + dy, -OFFSET_MAX, OFFSET_MAX);
	return (0);
}
