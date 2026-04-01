#include "cub_structs.h"
#include "cub_utils.h"

static
void    stt_pixel_swap(t_mat32 frame, int32_t x, int32_t y, uint32_t color)
{
    uint32_t    *dst;

    if (x < 0 || x >= frame.width || y < 0 || y >= frame.height)
        return ;
    dst = frame.ptr + x * frame.stride + y;
    if (color == 0x000000)
    {
        if (*dst == 9906388)
            color = 0xd5213f;
        else if (*dst == 7807488)
            color = 0xd72a55;
    }
    else if (color == 0xFFFFFF)
    {
        if (*dst == 9906388)
            color = 0xe6c84b;
    }
    else if (color == 0x00FF00)
    {
        if (*dst == 9906388)
            color = 0x115533;
    }
    *dst = color;
}

void    stt_quad_draw(t_game *game, t_vec2 pos, t_vec2 size,
            int32_t color, int32_t bound, t_vec2 map_center)
{
    t_vec2  limit;
    int32_t y;
    int32_t x;

    limit.x.i = pos.x.i + size.x.i;
    limit.y.i = pos.y.i + size.y.i;
    y = pos.y.i;
    while (y <= limit.y.i)
    {
        x = pos.x.i;
        while (x <= limit.x.i)
        {
            if (vec2_idist(map_center, (t_vec2){.x.i = x, .y.i = y})
                <= bound * bound)
                stt_pixel_swap(game->frame.render, x, y, color);
            x++;
        }
        y++;
    }
}

static
void    stt_blocks_render(t_game *game, t_vec2 pos,
            int32_t bound, t_vec2 map_center)
{
    t_vec2  size;
    int32_t y;
    int32_t x;
    t_vec2  draw_pos;
    t_vec2  map_pos;

    size.x.i = game->assets.radar_l0.texture.width / 12;
    size.y.i = game->assets.radar_l0.texture.height / 12;
    y = game->player.cam.pos.y.f - 6.f;
    while (y <= game->player.cam.pos.y.f + 6.f)
    {
        x = game->player.cam.pos.x.f - 6.f;
        while (x <= game->player.cam.pos.x.f + 6.f)
        {
            if (x >= 0 && x < (int32_t)game->map.width
                && y >= 0 && y < (int32_t)game->map.height)
            {
                draw_pos.x.i = x - game->player.cam.pos.x.f + 6;
                draw_pos.y.i = y - game->player.cam.pos.y.f + 6;
                map_pos.x.i = pos.x.i + draw_pos.x.i * size.x.i + 8;
                map_pos.y.i = pos.y.i + draw_pos.y.i * size.y.i + 8;
                if (game->map.tex_index[x + game->map.width * y] == 0)
                    stt_quad_draw(game, map_pos, size,
                        0x000000, bound, map_center);
                if (game->map.tex_index[x + game->map.width * y] == 130)
                    stt_quad_draw(game, map_pos, size,
                        0xFFFFFF, bound, map_center);
                if (game->map.tex_index[x + game->map.width * y] == 2)
                    stt_quad_draw(game, map_pos, size,
                        0xFFFFFF, bound, map_center);
            }
            x++;
        }
        y++;
    }
}

static
void    stt_map_render(t_game *game)
{
    t_vec2  pos;
    t_mat32 tex;
    t_vec2  pos4;
    t_vec2  bound;
    t_vec2  map_center;
    t_vec2  p_pos;
    int     radius;
    int     dy;
    int     dx;
    int     px;
    int     py;
    int     tile;

    pos.x.i = 110;
    pos.y.i = 240;

    tex = game->drawbuf.radar_l0.texture;
    tex.ptr += game->drawbuf.radar_l0.index * game->drawbuf.radar_l0.frame_size;
    cub_draw_texture(game->frame.render, tex, pos.x.i, pos.y.i);

    pos4.x.i = pos.x.i + game->assets.radar_l0.texture.width;
    pos4.y.i = pos.y.i + game->assets.radar_l0.texture.height;

    bound.x.i = (pos4.x.i - pos.x.i) / 2;
    bound.y.i = (pos4.y.i - pos.y.i) / 2;

    map_center.x.i = pos.x.i + bound.x.i;
    map_center.y.i = pos.y.i + bound.y.i;

    stt_blocks_render(game, pos, bound.x.i, map_center);

    tile = game->assets.radar_l0.texture.width / 12;

    p_pos.x.i = map_center.x.i - tile / 2 + 4;
    p_pos.y.i = map_center.y.i - tile / 2 + 4;

    radius = 2;
    dy = -radius;
    while (dy <= radius)
    {
        dx = -radius;
        while (dx <= radius)
        {
            if (dx * dx + dy * dy <= radius * radius)
            {
                px = p_pos.x.i + dx;
                py = p_pos.y.i + dy;
                if (vec2_idist(map_center,
                        (t_vec2){.x.i = px, .y.i = py})
                    <= bound.x.i * bound.x.i)
                    stt_pixel_swap(game->frame.render, px, py, 0x00FF00);
            }
            dx++;
        }
        dy++;
    }

    tex = game->drawbuf.radar_l1.texture;
    tex.ptr += game->drawbuf.radar_l1.index * game->drawbuf.radar_l1.frame_size;
    cub_draw_texture(game->frame.render, tex, pos.x.i, pos.y.i);
}

void    animate_hud(t_game *game)
{
    stt_map_render(game);
}
