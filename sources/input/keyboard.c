#include <stdlib.h>
#include "game_types.h"
#include "game_prototypes.h"

int input_keydown(SDL_Scancode keycode, t_game *game)
{
	if (keycode == SDL_SCANCODE_ESCAPE)
	{
		game->quit = true;	// TODO: this leaks
		exit(0);
	}
	game->state.paused ^= (keycode == SDL_SCANCODE_P);
	if (game->state.paused == true)
		return (0);
	if (keycode == SDL_SCANCODE_W || keycode == SDL_SCANCODE_UP)
		game->state.key |= (size_t)key_w;
	if (keycode == SDL_SCANCODE_A)
		game->state.key |= (size_t)key_a;
	if (keycode == SDL_SCANCODE_S || keycode == SDL_SCANCODE_DOWN)
		game->state.key |= (size_t)key_s;
	if (keycode == SDL_SCANCODE_D)
		game->state.key |= (size_t)key_d;
	if (keycode == SDL_SCANCODE_R)
		game->state.key |= (size_t)key_r;
	if (keycode == SDL_SCANCODE_E)
		game->state.key |= (size_t)key_e;
	if (keycode == SDL_SCANCODE_TAB)
		game->state.key |= (size_t)key_tab;
	if (keycode == SDL_SCANCODE_LSHIFT)
		game->state.key |= (size_t)key_shift;
	if (keycode == SDL_SCANCODE_LCTRL)
		game->state.key |= (size_t)key_ctrl;
	if (keycode == SDL_SCANCODE_LEFT)
		game->state.key |= (size_t)key_left;
	if (keycode == SDL_SCANCODE_RIGHT)
		game->state.key |= (size_t)key_right;
	return (0);
}

int input_keyup(SDL_Scancode keycode, t_game *game)
{
	if (keycode == SDL_SCANCODE_W || keycode == SDL_SCANCODE_UP)
		game->state.key &= ~(size_t)key_w;
	if (keycode == SDL_SCANCODE_A)
		game->state.key &= ~(size_t)key_a;
	if (keycode == SDL_SCANCODE_S || keycode == SDL_SCANCODE_DOWN)
		game->state.key &= ~(size_t)key_s;
	if (keycode == SDL_SCANCODE_D)
		game->state.key &= ~(size_t)key_d;
	if (keycode == SDL_SCANCODE_LSHIFT)
		game->state.key &= ~(size_t)key_shift;
	if (keycode == SDL_SCANCODE_LCTRL)
		game->state.key &= ~(size_t)key_ctrl;
	if (keycode == SDL_SCANCODE_R)
		game->state.key &= ~(size_t)key_r;
	if (keycode == SDL_SCANCODE_E)
		game->state.key &= ~(size_t)key_e;
	if (keycode == SDL_SCANCODE_TAB)
		game->state.key &= ~(size_t)key_tab;
	if (keycode == SDL_SCANCODE_LEFT)
		game->state.key &= ~(size_t)key_left;
	if (keycode == SDL_SCANCODE_RIGHT)
		game->state.key &= ~(size_t)key_right;
	return (0);
}
