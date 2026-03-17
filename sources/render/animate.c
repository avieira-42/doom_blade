#include "cub_types.h"
#include "cub_structs.h"
#include "cmlx.h"
#include "cub_utils.h"

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

void	window_clear(t_img *frame, int32_t color)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < SCREEN_HEIGHT)
	{
		j = 0;
		while (j < SCREEN_WIDTH)
		{
			stt_frame_pixel_put(frame, j, i, color);
			j++;
		}
		i++;
	}
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

void	sprite_sheet_animate(t_img *frame, t_anim_old *animation, t_vec2 pos, float scale)
{
	int32_t	i;

	i = animation->iterator;
	animation->counter += *animation->dt * 30;
	if (animation->counter >= animation->loops_per_sprite)
	{
		animation->counter = 0;
		animation->iterator++;
		if (animation->iterator== animation->size)
		{
			animation->iterator= 0;
			animation->end = true;
		}
	}
	draw_texture(frame, &animation->sheet[i], pos, scale);
}

static
void	stt_reload_handler(t_game *game)
{
	if (game->hud.gun.first_iterator == -1)
	{
		game->hud.reload->iterator = game->hud.gun.ammo * 4;
		game->hud.gun.first_iterator = game->hud.reload->iterator;
	}
	sprite_sheet_animate(&game->frame, game->hud.reload,
			(t_vec2){(t_32){ .i = SCREEN_WIDTH / 5.3f },
			(t_32){ .i = SCREEN_HEIGHT / 3.0f }}, 1.6);
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

	static
void	stt_shooting_handler(t_game *game)
{
	game->hud.walk->iterator = 0;
	game->hud.gun.first_iterator = -1;
	if (game->hud.shoot_sound == true)
	{
		Mix_PlayChannel(1, game->audio.gun_shot, 0);
		game->hud.shoot_sound = false;
	}
	sprite_sheet_animate(&game->frame, game->hud.shoot,
			(t_vec2){
			(t_32){ .i = SCREEN_WIDTH / 5.3f },
			(t_32){ .i = SCREEN_HEIGHT / 3.0f }
			}, 1.6);
	if (game->hud.shoot->end == true)
	{
		game->hud.shoot_sound = false;
		game->hud.shoot->end = false;
		game->hud.shoot->iterator = 0;
	}
}

	static
void	stt_walking_handler(t_game *game)
{
	if (game->w || game->a || game->s || game->d)
	{
		sprite_sheet_animate(&game->frame, game->hud.walk,
				(t_vec2){
				(t_32){ .i = SCREEN_WIDTH / 5.3f },
				(t_32){ .i = SCREEN_HEIGHT / 3.0f }
				}, 1.6);
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


	static
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

	static
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

	static
void	stt_hud_animate(t_game *game)
{
	stt_hands_render(game);
	stt_cards_render(game);
}

void	animate(t_game *game)
{
	time_delta_get(game);
	stt_hud_animate(game);
}
