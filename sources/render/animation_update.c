#include "cub_types.h"
#include "cub_structs.h"
#include "cub_utils.h"
#include "cmlx.h"

static
void	stt_frame_pixel_put(t_img *frame, int32_t x, int32_t y, int32_t color)
{
	char	*dst;
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
		return ;
	dst = frame->data + (y * frame->size_line + x * frame->bpp / 8);
	*(unsigned int *)dst = color;
}

static
int32_t	stt_get_color(t_img *image, int32_t x, int32_t y)
{
	int32_t	*color;

	if (x < 0 || x >= image->width || y < 0 || y > image->height)
		return (0);
	color = (int32_t *) (image->data + x * (image->bpp / 8) + y * image->size_line);
	return (*color);
}

void	draw_texture(t_img *frame, t_img *image, t_vec2 pos, float scale)
{
	int32_t	x;
	int32_t	y;
	int32_t	color;	

	x = 0;
	while (x < image->width * scale)
	{
		y = 0;
		while (y < image->height * scale)
		{
			color = stt_get_color(image, x / scale, y / scale);
			if (color != 2228223 && color != 1441791)
				stt_frame_pixel_put(frame, pos.x.i + x, pos.y.i + y, color);
			y++;
		}
		x++;
	}
}

/*void	cub_sprite_sheet_update(t_sheet *sheet)
{
	sheet->counter += 30;
	if (sheet->counter >= sheet->loops_per_sprite)
	{
		sheet->counter = 0;
		sheet->iterator++;
		sheet->texture.ptr += sheet->texture.cols *sheet->texture.rows;
		if (sheet->iterator == sheet->texture.depth)
		{
			sheet->texture.ptr = sheet->first;
			sheet->end = true;
		}
	}
}

// TODO: Add Scale to cub_draw_image
void	cub_sprite_sheet_animate(t_mat32 frame, t_sheet *sheet, t_vec2 pos)
{
	cub_sprite_sheet_update(sheet);
	cub_draw_image(sheet->texture, frame, pos.x.i, pos.y.i);
}

static
void	stt_reload_handler(t_game *game)
{
	if (game->hud.gun.first_iterator == -1)
	{
		game->hud.reload->iterator = game->hud.gun.ammo * 4;
		game->hud.gun.first_iterator = game->hud.reload->iterator;
	}
	cub_sprite_sheet_animate(game->render_frame, &game->assets.reload,
			(t_vec2){(t_32){ .i = SCREEN_WIDTH / 5.3f },
			(t_32){ .i = SCREEN_HEIGHT / 3.0f }});
	if (game->hud.reload->iterator - game->hud.gun.first_iterator == 4
			|| game->hud.reload->iterator >= game->hud.reload->size - 1)
	{
		Mix_PlayChannel(1, game->audio.gun_reload, 0);
		game->hud.gun.ammo++;
		game->hud.gun.first_iterator = -1;
	}
	if (game->hud.gun.ammo == game->hud.gun.max_ammo)
	{
		game->hud.hands_reload = false;
		game->hud.gun.first_iterator = -1;
		game->hud.reload->iterator = 0;
	}
}

// static
void	stt_shooting_handler(t_game *game)
{
	game->hud.walk->iterator = 0;
	game->hud.gun.first_iterator = -1;
	if (game->hud.shoot_sound == true)
	{
		Mix_PlayChannel(1, game->audio.gun_shot, 0);
		game->hud.shoot_sound = false;
	}
	cub_sprite_sheet_animate(game->render_frame, &game->assets.shoot,
			(t_vec2){
			(t_32){ .i = SCREEN_WIDTH / 5.3f },
			(t_32){ .i = SCREEN_HEIGHT / 3.0f }});
	if (game->hud.shoot->end == true)
	{
		game->hud.shoot_sound = false;
		game->hud.shoot->end = false;
		game->hud.shoot->iterator = 0;
		game->hud.hands_shoot = false;
	}
}

// static
void	stt_walking_handler(t_game *game)
{
	if (game->w || game->a || game->s || game->d)
	{
		Mix_PlayChannel(-1, game->audio.current_step, 0);
		cub_sprite_sheet_animate(game->render_frame, &game->assets.walk,
				(t_vec2){
				(t_32){ .i = SCREEN_WIDTH / 5.3f },
				(t_32){ .i = SCREEN_HEIGHT / 3.0f }});
	}
	else
	{
		game->hud.walk->iterator = 0;
		draw_texture(&game->frame, &game->hud.walk->sheet[0],
				(t_vec2){
				(t_32){ .i = SCREEN_WIDTH / 5.3f },
				(t_32){ .i = SCREEN_HEIGHT / 3.0f }
				}, 1.6);
	}
}


// static
void	stt_hands_render(t_game *game)
{
	if (game->hud.hands_reload == true && game->hud.hands_shoot == false)
		stt_reload_handler(game);
	else
	{
		if (game->hud.hands_shoot == true)
			stt_shooting_handler(game);
		else
			stt_walking_handler(game);
	}
}

// static
void	stt_cards_render(t_game *game)
{
	draw_texture(&game->frame,
			&game->hud.ammo->sheet[game->hud.ammo->size - game->hud.gun.ammo - 2],
			(t_vec2){.x = {.i = 50}, .y = {.i = SCREEN_HEIGHT / 1.25}}, 2);
	draw_texture(&game->frame, &game->hud.health->sheet[0],
			(t_vec2){.x = {.i = 215}, .y = {.i = SCREEN_HEIGHT / 1.25}}, 2);
	draw_texture(&game->frame, &game->hud.pill->sheet[0],
			(t_vec2){.x = {.i = 380}, .y = {.i = SCREEN_HEIGHT / 1.25}}, 2);
}

//static
void	stt_hud_animate(t_game *game)
{
	stt_hands_render(game);
	stt_cards_render(game);
}*/

void	animate_hud(t_game *game)
{
	(void)game;
	/*time_delta_get(game);
	input_handler(game);
	stt_hud_animate(game);*/

	// debug >>>>>>
	cub_draw_image(game->assets.walk.texture, game->display_frame, 100, 100);
	//sprite_sheet_animate(game->assets->)
	// <<<<<<< debug
}
