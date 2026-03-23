#include "cub_types.h"
#include "cub_structs.h"
#include "cub_utils.h"
#include "cmlx.h"

static
void    stt_frame_pixel_put(t_mat32 frame, int32_t x, int32_t y, uint32_t color)
{
    uint32_t    *dst;
    if (x < 0 || x >= frame.cols || y < 0 || y >= frame.rows)
        return ;
    dst = frame.ptr + x + frame.cols * y;
    *(unsigned int *)dst = color;
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
void    cub_draw_texture(t_mat32 frame, t_mat32 image, t_vec2 pos, float scale)
{
    (void)scale;

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
                stt_frame_pixel_put(frame, pos.x.i + x, pos.y.i + y, color);
            x++;
        }
        y++;
    }
}

void cub_sprite_sheet_update(t_sheet *sheet)
{
    sheet->counter++;
    printf("loops:%i\n", sheet->loops_per_sprite);
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
    if (game->hud.gun.first_iterator == -1)
    {
        game->assets.reload.iterator = game->hud.gun.ammo * 4;
        game->hud.gun.first_iterator = game->assets.reload.iterator;
    }
    cub_sprite_sheet_animate(game->display_frame, &game->assets.reload,
            (t_vec2){.x = { .i = 0 }, .y = {.i = 0}});
    if (game->assets.reload.iterator - game->hud.gun.first_iterator == 4
            || game->assets.reload.iterator >= game->assets.reload.texture.depth - 1)
    {
        Mix_PlayChannel(1, game->audio.gun_reload, 0);
        game->hud.gun.ammo++;
        game->hud.gun.first_iterator = -1;
    }
    if (game->hud.gun.ammo == game->hud.gun.max_ammo)
    {
        game->hud.hands_reload = false;
        game->hud.gun.first_iterator = -1;
        game->assets.reload.iterator = 0;
    }
}

// static
void    stt_shooting_handler(t_game *game)
{
	game->hud.hands_reload = false;
    game->assets.walk.iterator = 0;
    game->hud.gun.first_iterator = -1;
    if (game->hud.shoot_sound == true)
    {
        Mix_PlayChannel(1, game->audio.gun_shot, 0);
        game->hud.shoot_sound = false;
    }
    cub_sprite_sheet_animate(game->display_frame, &game->assets.shoot,
            (t_vec2){.x = { .i = 0}, .y = { .i = 0}});
    if (game->assets.shoot.end == true)
    {
        game->hud.shoot_sound = false;
        game->assets.shoot.end = false;
        game->assets.shoot.iterator = 0;
        game->hud.hands_shoot = false;
		game->assets.shoot.end = false;
    }
}

// static
void    stt_walking_handler(t_game *game)
{
    if (game->w || game->a || game->s || game->d)
    {
        Mix_PlayChannel(-1, game->audio.current_step, 0);
        cub_sprite_sheet_animate(game->display_frame, &game->assets.walk,
                (t_vec2){.x = { .i = 0}, .y = {.i = 0}});
    }
    else
    {
        game->assets.walk.iterator = 0;
        cub_draw_texture(game->display_frame, game->assets.walk.texture,
                (t_vec2){.x =  {.i = 0}, .y = {.i = 0}}, 1.6);
    }
}


// static
void    stt_hands_render(t_game *game)
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
/*void  stt_cards_render(t_game *game)
{
    cub_draw_texture(game->display_frame,
            &game->hud.ammo->sheet[game->hud.ammo->size - game->hud.gun.ammo - 2],
            (t_vec2){.x = {.i = 50}, .y = {.i = SCREEN_HEIGHT / 1.25}}, 2);
    cub_draw_texture(&game->frame, &game->hud.health->sheet[0],
            (t_vec2){.x = {.i = 215}, .y = {.i = SCREEN_HEIGHT / 1.25}}, 2);
    cub_draw_texture(&game->frame, &game->hud.pill->sheet[0],
            (t_vec2){.x = {.i = 380}, .y = {.i = SCREEN_HEIGHT / 1.25}}, 2);
}*/

//static
void    stt_hud_animate(t_game *game)
{
    stt_hands_render(game);
    //stt_cards_render(game);
}

void    animate_hud(t_game *game)
{
    (void)game;
    //time_delta_get(game);
    input_handler(game);
    stt_hud_animate(game);

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
