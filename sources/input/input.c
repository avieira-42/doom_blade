#include "cub_structs.h"
#include "cub_utils.h"

static
void	stt_moving(t_game *game, t_player *player)
{
	float	spd_forward;
	float	spd_sideway;

	if (game->state.key & (key_w | key_s | key_d | key_a))
		player->state |= st_run;
	else
		player->state &= ~(size_t)st_run;
	spd_forward = !!(game->state.key & key_w) - !!(game->state.key & key_s);
	spd_sideway = !!(game->state.key & key_d) - !!(game->state.key & key_a);
	game->player.spd_forward = spd_forward;
	game->player.spd_sideway = spd_sideway;
	if (game->state.key & key_shift)
		game->player.spd_max = PLAYER_WALK_SPEED;
	else
		game->player.spd_max = PLAYER_RUN_SPEED;
	if (game->state.key & key_left)
	{
		game->player.cam.dir = vec2_rotate(game->player.cam.dir, -PI_DEG);
		game->player.cam.plane = vec2_rotate(game->player.cam.plane, -PI_DEG);
	}
	if (game->state.key & key_right)
	{
		game->player.cam.dir = vec2_rotate(game->player.cam.dir, PI_DEG);
		game->player.cam.plane = vec2_rotate(game->player.cam.plane, PI_DEG);
	}
}

// A player that is reloading cannot be shooting or checking minimap
static inline
void	stt_reloading(t_player *player)
{
	if (player->ammo >= AMMO_COUNT)
		return ;
	if (!(player->state & (st_reloading | st_shooting | st_radar)))
	{
		player->state = st_reloading;	// Discards all other states, two handed action
		player->hands.reload.index = player->ammo * RELOAD_CYCLE;
	}
}

static inline
void	stt_shooting(t_player *player)
{
	if (player->ammo <= 0)
		return ;
	if (!(player->state & st_shooting))
	{
		player->ammo--;
		player->state &= (st_radar);	// Only radar state is kept
		player->state |= (st_shooting | st_shot);
	}
}

// A player may be shooting and checking radar
static inline
void	stt_minimap(t_game *game, t_player *player)
{
	if (!(game->state.key & key_tab))
	{
		player->hands.radar.index = 0;
		player->hands.radar.frame_dt = 0;
		player->state &= ~(size_t)(st_radar);
		return ;
	}
	if (!(player->state & (st_reloading | st_radar)))
	{
		player->state |= st_radar;
	}
}

void	input_handler(t_game *game, t_player *player)
{
	stt_moving(game, player);
	if (game->state.key & key_lmb)
		stt_shooting(player);
	if (game->state.key & key_r)
		stt_reloading(player);
	stt_minimap(game, player);
	if (game->state.key & key_e)
	{
		player->state |= st_interacting;
		game->state.key &= ~(size_t) key_e;
	}
}

void	sdl_input_update(t_game *game)
{
	SDL_Event		event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				cmlx_keydown(event.key.keysym.scancode, game);
				break;
			case SDL_KEYUP:
				cmlx_keyup(event.key.keysym.scancode, game);
				break;
			case SDL_MOUSEBUTTONDOWN:
				cmlx_mousedown(event.button.button, game);
				break;
			case SDL_MOUSEBUTTONUP:
				cmlx_mouseup(event.button.button, game);
				break;
			case SDL_MOUSEMOTION:
				cmlx_mousemove(event.motion.x, event.motion.y, game);
				break;
			default:
				break;
		}
	}
}

