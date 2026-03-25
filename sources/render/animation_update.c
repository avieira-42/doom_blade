#include "cub_types.h"
#include "cub_structs.h"
#include "cub_utils.h"
#include "cmlx.h"

static
void	stt_frame_pixel_put(t_mat32 frame, int32_t x, int32_t y, uint32_t color)
{
	uint32_t	*dst;
	if (x < 0 || x >= frame.width || y < 0 || y >= frame.height)
		return ;
	dst = frame.ptr + x + frame.width * y;
	*(unsigned int *)dst = color;
}

void	cub_draw_texture(t_mat32 frame, t_mat32 image, t_vec2 pos, float scale)
{
	size_t	  x;
	size_t	  y;
	uint32_t	color;  

	y = 0;
	while (y < image.height)
	{
		x = 0;
		while (x < image.width)
		{
			color = image.ptr[x + image.width * y];
			if (color != 2228223 && color != 1441791)
				stt_frame_pixel_put(frame, pos.x.i + x, pos.y.i + y, color);
			x++;
		}
		y++;
	}
}

void cub_sprite_sheet_update(t_sheet *sheet)
{
	sheet->counter++;
	// printf("loops:%i\n", sheet->loops_per_sprite);
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

// TODO: Add Scale to cub_draw_texture
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
		game->assets.reload.iterator = game->gun.ammo * 4;
		game->gun.first_iterator = game->assets.reload.iterator;
	}
	cub_sprite_sheet_animate(game->frame.display, &game->assets.reload,
			(t_vec2){.x = { .i = SCREEN_WIDTH / 5 }, .y = {.i = SCREEN_HEIGHT / 2.5}});
	if (game->assets.reload.iterator - game->gun.first_iterator == 4
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
			(t_vec2){.x = { .i = SCREEN_WIDTH / 5 }, .y = {.i = SCREEN_HEIGHT / 2.5}});
	if (game->assets.shoot.end == true)
	{
		game->assets.shoot.sound = false;
		game->assets.shoot.end = false;
		game->assets.shoot.iterator = 0;
		game->assets.shoot.start = false;
		game->assets.shoot.end = false;
	}
}

// static
void	stt_walking_handler(t_game *game)
{
	if (game->state.key & (key_w | key_a | key_s | key_d))	// TODO: Review
	{
		Mix_PlayChannel(-1, game->assets.audio.current_step, 0);
		cub_sprite_sheet_animate(game->frame.display, &game->assets.walk,
			(t_vec2){.x = { .i = SCREEN_WIDTH / 5 }, .y = {.i = SCREEN_HEIGHT / 2.5}});
	}
	else
	{
		game->assets.walk.iterator = 0;
		cub_draw_texture(game->frame.display, game->assets.walk.texture,
			(t_vec2){.x = { .i = SCREEN_WIDTH / 5 }, .y = {.i = SCREEN_HEIGHT / 2.5}}, 1);
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

// static
void  stt_cards_render(t_game *game)
{
	size_t  frame_size;
	t_mat32 texture;
	t_vec2	pos;

	frame_size = game->assets.ammo.texture.width * game->assets.ammo.texture.height;
	texture = game->assets.ammo.texture;
	texture.ptr += frame_size * (game->assets.ammo.texture.depth - game->gun.ammo - 1);
	pos = (t_vec2){.x = {.i = 0}, .y = {.i = 0}};
	cub_draw_texture(game->frame.display, texture, pos, 1);

	texture = game->assets.health.texture;
	pos = (t_vec2){.x = {.i = 90}, .y = {.i = 0}};
	cub_draw_texture(game->frame.display, texture, pos, 1);

	texture = game->assets.pill.texture;
	pos = (t_vec2){.x = {.i = 180}, .y = {.i = 0}};
	cub_draw_texture(game->frame.display, texture, pos, 1);
}

//static
void	stt_radar_render(t_game *game)
{
	t_mat32 texture;
	t_vec2	pos;

	texture = game->assets.pill.texture;
	pos = (t_vec2){.x = {.i = 0}, .y = {.i = SCREEN_HEIGHT / 4}};
	cub_draw_texture(game->frame.display, texture, pos, 1);
}

void	animate_hud(t_game *game)
{
	stt_hands_render(game);
	stt_cards_render(game);
	stt_radar_render(game);

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
