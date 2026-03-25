#include "cub_types.h"
#include "cub_structs.h"
#include "cub_utils.h"
#include "cmlx.h"

void    frame_pixel_put(t_mat32 frame, int32_t x, int32_t y, uint32_t color)
{
    uint32_t    *dst;
    if (x < 0 || x >= frame.cols || y < 0 || y >= frame.rows)
        return ;
    dst = frame.ptr + x + frame.cols * y;
    *(unsigned int *)dst = color;
}

void    cub_draw_texture(t_mat32 frame, t_mat32 image, t_vec2 pos, float scale)
{
    size_t      x;
    size_t      y;
    uint32_t    color;  

    y = 0;
    while (y < image.rows)
    {
        x = 0;
        while (x < image.cols)
        {
            color = image.ptr[x + image.cols * y];
            if (color != 2228223 && color != 1441791)
                frame_pixel_put(frame, pos.x.i + x, pos.y.i + y, color);
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

// TODO: Add Scale to cub_draw_texture
void cub_sprite_sheet_animate(t_mat32 frame, t_sheet *sheet, t_vec2 pos)
{
    size_t  frame_size;
    t_mat32 texture;

    frame_size = sheet->texture.cols * sheet->texture.rows;
    texture = sheet->texture;
    texture.ptr += sheet->iterator * frame_size;
    cub_sprite_sheet_update(sheet);
    cub_draw_texture(frame, texture, pos, 1.0f);
}

static
void    stt_reload_handler(t_game *game)
{
    if (game->gun.first_iterator == -1)
    {
        game->assets.reload.iterator = game->gun.ammo * 4;
        game->gun.first_iterator = game->assets.reload.iterator;
    }
    cub_sprite_sheet_animate(game->display_frame, &game->assets.reload,
            (t_vec2){.x = { .i = SCREEN_WIDTH / 5 }, .y = {.i = SCREEN_HEIGHT / 2.5}});
    if (game->assets.reload.iterator - game->gun.first_iterator == 4
            || game->assets.reload.iterator >= game->assets.reload.texture.depth - 1)
    {
        Mix_PlayChannel(1, game->audio.gun_reload, 0);
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
void    stt_shooting_handler(t_game *game)
{
	game->assets.reload.start = false;
    game->assets.walk.iterator = 0;
    game->gun.first_iterator = -1;
    if (game->assets.shoot.sound == true)
    {
        Mix_PlayChannel(1, game->audio.gun_shot, 0);
        game->assets.shoot.sound = false;
    }
    cub_sprite_sheet_animate(game->display_frame, &game->assets.shoot,
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
void    stt_walking_handler(t_game *game)
{
    if (game->key & key_w || game->key & key_a
			|| game->key & key_s || game->key & key_d)
    {
        Mix_PlayChannel(-1, game->audio.current_step, 0);
        cub_sprite_sheet_animate(game->display_frame, &game->assets.walk,
            (t_vec2){.x = { .i = SCREEN_WIDTH / 5 }, .y = {.i = SCREEN_HEIGHT / 2.5}});
    }
    else
    {
        game->assets.walk.iterator = 0;
        cub_draw_texture(game->display_frame, game->assets.walk.texture,
            (t_vec2){.x = { .i = SCREEN_WIDTH / 5 }, .y = {.i = SCREEN_HEIGHT / 2.5}}, 1);
    }
}


// static
void    stt_hands_render(t_game *game)
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

    frame_size = game->assets.ammo.texture.cols * game->assets.ammo.texture.rows;
    texture = game->assets.ammo.texture;
    texture.ptr += frame_size * (game->assets.ammo.texture.depth - game->gun.ammo - 1);
	pos = (t_vec2){.x = {.i = 15}, .y = {.i = 15}};
    cub_draw_texture(game->display_frame, texture, pos, 1);

    texture = game->assets.health.texture;
	pos = (t_vec2){.x = {.i = 90 + 15}, .y = {.i = 15}};
    cub_draw_texture(game->display_frame, texture, pos, 1);

    texture = game->assets.pill.texture;
	pos = (t_vec2){.x = {.i = 180 + 15}, .y = {.i = 15}};
    cub_draw_texture(game->display_frame, texture, pos, 1);
}


        /*y = 0;
        while (y < map.rows)
        {
                x = 0;
                while(x < map.cols)
                {
                        block = map.ptr[x + map.cols * y];
                        if ((x == 0 || x == map.cols - 1 || y == 0 || y == map.rows - 1)
                                        && block != '1')
                                return (-1);
                        x++;
                }
                y++;
        }*/

//static
void	stt_quad_draw(t_game *game, t_vec2 pos, t_vec2 size, int32_t color, int32_t bound, t_vec2 map_center)
{
	const t_vec2	limit = (t_vec2){.x = {.i = pos.x.i + size.x.i},
									.y = {.i =pos.y.i + size.y.i}};
	int32_t			x;
	int32_t			y;

	y = pos.y.i;
	while (y <= limit.y.i)
	{
		x = pos.x.i;
		while (x <= limit.x.i)
		{
			if ((x == pos.x.i || x == limit.x.i
					|| y == pos.y.i || y == limit.y.i)
					&& vec2_dist(map_center, (t_vec2){.x = {.i = x}, .y = {.i = y}}) <= bound * bound)
				frame_pixel_put(game->display_frame, x, y, color);
			x++;
		}
		y++;
	}
}

//static
void	stt_blocks_render(t_game *game, t_vec2 pos, int32_t bound, t_vec2 map_center)
{
	const t_vec2	size = (t_vec2){.x = {.i = game->assets.radar_l0.texture.cols / 12},
									.y = {.i = game->assets.radar_l0.texture.rows / 12}};
	t_vec2			map_pos;
	int32_t			x;
	int32_t			y;
	t_vec2			draw_pos;

	y = game->player.cam.pos.y.f - 6;
	while (y <= game->player.cam.pos.y.f + 6)
	{
		x = game->player.cam.pos.x.f - 6;
		while (x <= game->player.cam.pos.x.f + 6)
		{
			if (y >= 0 && y < game->map.rows
					&& x >= 0 && x < game->map.cols)
			{
				draw_pos = (t_vec2){.x = {.i = x - game->player.cam.pos.x.f + 6}, .y = {.i = y - game->player.cam.pos.y.f + 6}};
				map_pos = (t_vec2){.x = {.i = pos.x.i + draw_pos.x.i * size.x.i}, .y = {.i = pos.y.i + draw_pos.y.i * size.y.i}};
				if (game->map.ptr[x + game->map.cols * y] == 1)
				stt_quad_draw(game, map_pos, size, 0x440044, bound, map_center);
			}
			x++;
		}
		y++;
	}
}

//static
void	stt_map_render(t_game *game)
{
	const t_vec2	pos = (t_vec2){.x = {.i = SCREEN_WIDTH - 255}, .y = {.i = 5}};
	const t_vec2	pos4 = (t_vec2){.x = {.i = pos.x.i + game->assets.radar_l0.texture.cols}, .y = {.i = pos.y.i + game->assets.radar_l0.texture.rows}};
	const t_vec2	bound = (t_vec2){.x = {.i = (pos4.x.i - pos.x.i) / 2}, .y = {.i = (pos4.y.i - pos.y.i) / 2}};
	const t_vec2	map_center = (t_vec2){.x = {.i = pos.x.i + bound.x.i}, .y = {.i = pos.y.i + bound.y.i}};
	const t_vec2	p_pos = (t_vec2){.x = {.i = pos.x.i + bound.x.i - 10}, .y = {.i = pos.y.i + bound.y.i - 10}};

	/* render radar (spritesheet to be fragmented into 2 seperate layers
	that are rendered one befor the map and the other after)*/
	cub_sprite_sheet_animate(game->display_frame, &game->assets.radar_l0, pos);

	/* render blocks */
	stt_blocks_render(game, pos, bound.x.i, map_center);
	stt_quad_draw(game, p_pos, (t_vec2){.x = {.i = 10}, .y = {.i = 10}}, 0x003300, bound.x.i, map_center);

	/* render second layer */
	cub_sprite_sheet_animate(game->display_frame, &game->assets.radar_l1, pos);
}

//static
void    stt_animate(t_game *game)
{
    stt_hands_render(game);
    stt_cards_render(game);
	stt_map_render(game);
}

void    animate_hud(t_game *game)
{
    stt_animate(game);

    // debug >>>>>>
    //cub_draw_texture(game->display_frame, game->assets.walk.texture, (t_vec2){.x = {.i = 0}, .y = {.i = 0}}, 1.3);
    //cub_draw_texture(game->display_frame, game->assets.shoot.texture, (t_vec2){.x = {.i = 0}, .y = {.i = 0}}, 1.3);
    //cub_draw_texture(game->display_frame, game->assets.reload.texture, (t_vec2){.x = {.i = 0}, .y = {.i = 0}}, 1.3);
    /*size_t    frame_size = game->assets.walk.texture.cols * game->assets.walk.texture.rows;
    t_mat32 texture = game->assets.walk.texture;
    texture.ptr += frame_size;
    cub_draw_texture(game->display_frame, texture, (t_vec2){.x = {.i = 0}, .y = {.i = 0}}, 1.3);*/
    // <<<<<<< debug
}
