#include "cub_types.h"
#include "cub_structs.h"
#include "cub_utils.h"

#define hands_pos_x SCREEN_WIDTH / 4
#define hands_pos_y SCREEN_HEIGHT / 2.5

static
void	stt_reload_handler(t_game *game)
{
	if (game->gun.first_iterator == -1)
	{
		game->assets.reload.iterator = game->gun.ammo * 4;	// REVIEW: Magic number indexing
		game->gun.first_iterator = game->assets.reload.iterator;
	}
	cub_sprite_sheet_animate(game->frame.display, &game->assets.reload,
			(t_vec2){.x.i = hands_pos_x, .y.i = hands_pos_y});	// REVIEW: magic numbers
	if (game->assets.reload.iterator - game->gun.first_iterator == 4	// REVIEW: magic number comparison
			|| game->assets.reload.iterator
			>= game->assets.reload.texture.depth - 1)
	{
		Mix_PlayChannel(1, game->assets.audio.gun_reload, 0);
		game->gun.ammo++;
		game->gun.first_iterator = -1;
	}
	if (game->gun.ammo == game->gun.max_ammo)
	{
		game->assets.reload.start = false;
		game->gun.first_iterator = -1;
		game->assets.reload.iterator = 0;
	}
}

static
void	stt_shooting_handler(t_game *game)
{
	game->assets.reload.start = false;
	game->assets.walk.iterator = 0;
	game->gun.first_iterator = -1;
	if (game->assets.shoot.sound == true)
	{
		Mix_PlayChannel(1, game->assets.audio.gun_shot, 0);
		game->assets.shoot.sound = false;
	}
	cub_sprite_sheet_animate(game->frame.display, &game->assets.shoot,
			(t_vec2){.x.i = hands_pos_x, .y.i = hands_pos_y});	// REVIEW: magic numbers, should be defines
	if (game->assets.shoot.end == true)
	{
		game->assets.shoot.sound = false;
		game->assets.shoot.iterator = 0;
		game->assets.shoot.start = false;
		game->assets.shoot.end = false;
	}
}

// REVIEW: Normally state changes are tracked, like player moving, shooting, etc
// And then animation plays depending on the state the player is in. State transitions make sure that
// this only happens once instead of every tick
static
void	stt_walking_handler(t_game *game)
{
	if (game->state.key & (key_w | key_a | key_s | key_d))	// TODO: Review
	{
		Mix_PlayChannel(-1, game->assets.audio.current_step, 0);	// REVIEW: this spams the footstep audio every render tick, which makes it so high fps has bad audio
		cub_sprite_sheet_animate(game->frame.display, &game->assets.walk,				
			(t_vec2){.x.i = hands_pos_x, .y.i = hands_pos_y});	// REVIEW: magic numbers, should be defines
	}
	else
	{
		game->assets.walk.iterator = 0;
		cub_draw_texture(game->frame.display, game->assets.walk.texture,
			(t_vec2){.x.i = hands_pos_x, .y.i = hands_pos_y});	// REVIEW: magic numbers, should be defines
	}
}

void	hands_render(t_game *game)
{
	if (game->assets.reload.start == true && game->assets.shoot.start == false)
		stt_reload_handler(game);
	else
	{
		if (game->assets.shoot.start == true)
			stt_shooting_handler(game);
		else
			stt_walking_handler(game);
	}
}

void  cards_render(t_game *game)
{
	size_t	frame_size;
	t_mat32	texture;
	t_vec2	pos;

	frame_size = game->assets.ammo.texture.width
		* game->assets.ammo.texture.height;
	texture = game->assets.ammo.texture;
	texture.ptr += frame_size * (game->gun.max_ammo - game->gun.ammo);
	pos = (t_vec2){.x.i = 15, .y.i = 15};
	cub_draw_texture(game->frame.display, texture, pos);

	texture = game->assets.health.texture;
	pos = (t_vec2){.x.u = 90 + 15, .y.u = 15};
	cub_draw_texture(game->frame.display, texture, pos);

	texture = game->assets.pill.texture;
	pos = (t_vec2){.x.i = 180 + 15, .y.i = 15};
	cub_draw_texture(game->frame.display, texture, pos);
}
