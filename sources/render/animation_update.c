#include "cub_types.h"
#include "cub_structs.h"
#include "cub_utils.h"
#include "cmlx.h"


// REVIEW: One file containing 13 functions is cancer for merge. please split
static
void	stt_frame_pixel_put(t_mat32 frame, int32_t x, int32_t y, uint32_t color)
{
	uint32_t	*dst;
	if (x < 0 || x >= frame.width || y < 0 || y >= frame.height)
		return ;
	dst = frame.ptr + x + frame.width * y;
	*(unsigned int *)dst = color;
}

void	cub_draw_texture(t_mat32 frame, t_mat32 image, t_vec2 pos, float scale)	// REVIEW: scale is unused
{
	size_t		x;
	size_t		y;
	uint32_t	color;

	y = 0;
	while (y < image.height)
	{
		x = 0;
		while (x < image.width)
		{
			color = image.ptr[x + image.width * y];	// REVIEW: here you index manually but below use a helper to index
			if (color != 2228223 && color != 1441791)	// REVIEW: colors are hex coded normally
				stt_frame_pixel_put(frame, pos.x.i + x, pos.y.i + y, color);	// REVIEW: signed and unsigned integers mixing, potential giga bug
			x++;
		}
		y++;
	}
}

void cub_sprite_sheet_update(t_sheet *sheet)
{
	sheet->counter++;
	if (sheet->counter >= sheet->loops_per_sprite)
	{
		sheet->counter = 0;
		sheet->iterator++;
		if (sheet->iterator >= sheet->texture.depth)
		{
			sheet->iterator = 0;
			sheet->end = true;
		}
	}
}

// REVIEW: design bug makes it so rendering mutates animation state. 
// Animation state should occur regardless of rendering, because things that aren't visible aren't rendered but still loop
void cub_sprite_sheet_animate(t_mat32 frame, t_sheet *sheet, t_vec2 pos)
{
	size_t  frame_size;
	t_mat32 texture;

	frame_size = sheet->texture.width * sheet->texture.height;
	texture = sheet->texture;
	texture.ptr += sheet->iterator * frame_size;
	cub_sprite_sheet_update(sheet);
	cub_draw_texture(frame, texture, pos, 1.0f);
}

static
void	stt_reload_handler(t_game *game)
{
	if (game->gun.first_iterator == -1)
	{
		game->assets.reload.iterator = game->gun.ammo * 4;	// REVIEW: Magic number indexing
		game->gun.first_iterator = game->assets.reload.iterator;
	}
	cub_sprite_sheet_animate(game->frame.display, &game->assets.reload,
			(t_vec2){.x = { .i = SCREEN_WIDTH / 5 }, .y = {.i = SCREEN_HEIGHT / 2.5}});	// REVIEW: magic numbers
	if (game->assets.reload.iterator - game->gun.first_iterator == 4	// REVIEW: magic number comparison
			|| game->assets.reload.iterator >= game->assets.reload.texture.depth - 1)
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

// static
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
			(t_vec2){.x = { .i = SCREEN_WIDTH / 5 }, .y = {.i = SCREEN_HEIGHT / 2.5}});	// REVIEW: magic numbers, should be defines
	if (game->assets.shoot.end == true)
	{
		game->assets.shoot.sound = false;
		game->assets.shoot.end = false;	// REVIEW: end is set twice
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
			(t_vec2){.x = { .i = SCREEN_WIDTH / 5 }, .y = {.i = SCREEN_HEIGHT / 2.5}});	// REVIEW: magic numbers, should be defines
	}
	else
	{
		game->assets.walk.iterator = 0;
		cub_draw_texture(game->frame.display, game->assets.walk.texture,
			(t_vec2){.x = { .i = SCREEN_WIDTH / 5 }, .y = {.i = SCREEN_HEIGHT / 2.5}}, 1);	// REVIEW: magic numbers, should be defines
	}
}

// static
void	stt_hands_render(t_game *game)
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

static
void  stt_cards_render(t_game *game)
{
	size_t  frame_size;	// REVIEW: tab instead of spaces
	t_mat32 texture;
	t_vec2	pos;

	frame_size = game->assets.ammo.texture.width * game->assets.ammo.texture.height;
	texture = game->assets.ammo.texture;
	texture.ptr += frame_size * (game->assets.ammo.texture.depth - game->gun.ammo - 1);	// REVIEW: Unsafe indexing pattern. Max ammo exists, why is tex depth being used
	pos = (t_vec2){.x = {.i = 15}, .y = {.i = 15}};										// REVIEW: could count bullets_shot instead to simplify indexing and compare against max ammo
	cub_draw_texture(game->frame.display, texture, pos, 1);

	texture = game->assets.health.texture;
	pos = (t_vec2){.x = {.i = 90 + 15}, .y = {.i = 15}};		// REVIEW: these should be unsigned numbers, that way you can avoid < 0 checks in putpixel
	cub_draw_texture(game->frame.display, texture, pos, 1);

	texture = game->assets.pill.texture;
	pos = (t_vec2){.x = {.i = 180 + 15}, .y = {.i = 15}};		// REVIEW: magic numbers
	cub_draw_texture(game->frame.display, texture, pos, 1);
}

static
void	stt_quad_draw(t_game *game, t_vec2 pos, t_vec2 size, int32_t color, int32_t bound, t_vec2 map_center)
{
	const t_vec2	limit = (t_vec2){.x = {.i = pos.x.i + size.x.i},	// REVIEW: you can do (t_vec2){.x.i = pos.x.i, ..} instead
									.y = {.i =pos.y.i + size.y.i}};
	int32_t			x;
	int32_t			y;

	y = pos.y.i;
	while (y <= limit.y.i)	// REVIEW: off by one limits
	{
		x = pos.x.i;
		while (x <= limit.x.i)	// REVIEW: off by one limits
		{
			if ((x == pos.x.i || x == limit.x.i
					|| y == pos.y.i || y == limit.y.i)
					&& vec2_dist(map_center, (t_vec2){.x = {.i = x}, .y = {.i = y}}) <= bound * bound)
				stt_frame_pixel_put(game->frame.display, x, y, color);
			x++;
		}
		y++;
	}
}

static
void	stt_blocks_render(t_game *game, t_vec2 pos, int32_t bound, t_vec2 map_center)
{
	const t_vec2	size = (t_vec2){.x = {.i = game->assets.radar_l0.texture.width / 12},	// REVIEW: Magic numbers
									.y = {.i = game->assets.radar_l0.texture.height / 12}};
	t_vec2			map_pos;
	int32_t			x;
	int32_t			y;
	t_vec2			draw_pos;

	y = game->player.cam.pos.y.f - 6;			// REVIEW: Magic numbers and mixing floats with integers unsafely
	while (y <= game->player.cam.pos.y.f + 6)	// REVIEW: this will guaranteed cause off by one hard bugs
	{
		x = game->player.cam.pos.x.f - 6;
		while (x <= game->player.cam.pos.x.f + 6)
		{
			if (y >= 0 && y < game->map.height
					&& x >= 0 && x < game->map.width)
			{
				draw_pos = (t_vec2){.x = {.i = x - game->player.cam.pos.x.f + 6}, .y = {.i = y - game->player.cam.pos.y.f + 6}};
				map_pos = (t_vec2){.x = {.i = pos.x.i + draw_pos.x.i * size.x.i}, .y = {.i = pos.y.i + draw_pos.y.i * size.y.i}};
				if (game->map.ptr[x + game->map.width * y] == 1)
				stt_quad_draw(game, map_pos, size, 0x440044, bound, map_center);	// REVIEW: magic numbers for colors despite having enums for RGBS
			}
			x++;
		}
		y++;
	}
}

static
void	stt_map_render(t_game *game)
{
	const t_vec2	pos = (t_vec2){.x = {.i = SCREEN_WIDTH - 255}, .y = {.i = 5}};	// REVIEW: magic numbers for pos
	const t_vec2	pos4 = (t_vec2){.x = {.i = pos.x.i + game->assets.radar_l0.texture.width}, .y = {.i = pos.y.i + game->assets.radar_l0.texture.height}};
	const t_vec2	bound = (t_vec2){.x = {.i = (pos4.x.i - pos.x.i) / 2}, .y = {.i = (pos4.y.i - pos.y.i) / 2}};
	const t_vec2	map_center = (t_vec2){.x = {.i = pos.x.i + bound.x.i}, .y = {.i = pos.y.i + bound.y.i}};
	const t_vec2	p_pos = (t_vec2){.x = {.i = pos.x.i + bound.x.i - 10}, .y = {.i = pos.y.i + bound.y.i - 10}};

	/* render radar (spritesheet to be fragmented into 2 seperate layers
	that are rendered one befor the map and the other after)*/
	cub_sprite_sheet_animate(game->frame.display, &game->assets.radar_l0, pos);

	/* render blocks */
	stt_blocks_render(game, pos, bound.x.i, map_center);
	stt_quad_draw(game, p_pos, (t_vec2){.x = {.i = 10}, .y = {.i = 10}}, 0x003300, bound.x.i, map_center);

	/* render second layer */
	cub_sprite_sheet_animate(game->frame.display, &game->assets.radar_l1, pos);
}

void	animate_hud(t_game *game)	// REVIEW: all functions take the game god struct, no encapsulation and separation of concerns is even attempted
{
	stt_hands_render(game);
	stt_cards_render(game);
	stt_map_render(game);

	// debug >>>>>>
	//cub_draw_texture(game->frame.display, game->assets.walk.texture, (t_vec2){.x = {.i = 0}, .y = {.i = 0}}, 1.3);
	//cub_draw_texture(game->frame.display, game->assets.shoot.texture, (t_vec2){.x = {.i = 0}, .y = {.i = 0}}, 1.3);
	//cub_draw_texture(game->frame.display, game->assets.reload.texture, (t_vec2){.x = {.i = 0}, .y = {.i = 0}}, 1.3);
	/*size_t	frame_size = game->assets.walk.texture.width * game->assets.walk.texture.height;
	t_mat32 texture = game->assets.walk.texture;
	texture.ptr += frame_size;
	cub_draw_texture(game->frame.display, texture, (t_vec2){.x = {.i = 0}, .y = {.i = 0}}, 1.3);*/
	// <<<<<<< debug
}
