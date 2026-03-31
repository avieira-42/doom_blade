#include "cub_structs.h"
#include "cub_utils.h"

//void	stt_quad_draw(t_game *game, t_vec2 pos, t_vec2 size, int32_t color, int32_t bound, t_vec2 map_center)
//{
//	const t_vec2	limit = (t_vec2){.x.i = pos.x.i + size.x.i,
//									.y.i = pos.y.i + size.y.i};
//	int32_t			x;
//	int32_t			y;
//
//	y = pos.y.i;
//	while (y <= limit.y.i)	// REVIEW: off by one limits
//	{
//		x = pos.x.i;
//		while (x <= limit.x.i)	// REVIEW: off by one limits
//		{
//			if (/*(x == pos.x.i || x == limit.x.i
//						|| y == pos.y.i || y == limit.y.i)
//					&& */ vec2_idist(map_center, (t_vec2){.x.i = x, .y.i = y})
//						<= bound * bound)
//				frame_pixel_put(game->frame.display, x, y, color);
//			x++;
//		}
//		y++;
//	}
//}
//
//static
//void	stt_blocks_render(t_game *game, t_vec2 pos, int32_t bound, t_vec2 map_center)
//{
//	const t_vec2	size = (t_vec2){
//		.x.i = game->assets.radar_l0.texture.width / 12,	// REVIEW: Magic numbers
//		.y.i = game->assets.radar_l0.texture.height / 12};
//	t_vec2			map_pos;
//	int32_t			x;
//	int32_t			y;
//	t_vec2			draw_pos;
//
//	y = game->player.cam.pos.y.f - 6.f;			// REVIEW: Magic numbers
//	while (y <= game->player.cam.pos.y.f + 6.f)
//	{
//		x = game->player.cam.pos.x.f - 6.f;
//		while (x <= game->player.cam.pos.x.f + 6.f)
//		{
//			if (y >= 0 && y < game->map.height
//					&& x >= 0 && x < game->map.width)
//			{
//				draw_pos = (t_vec2){.x.i = x - game->player.cam.pos.x.f + 6,
//					.y.i = y - game->player.cam.pos.y.f + 6};
//				map_pos = (t_vec2){.x.i = pos.x.i + draw_pos.x.i * size.x.i,
//					.y.i = pos.y.i + draw_pos.y.i * size.y.i};
//				if (game->map.ptr[x + game->map.width * y] == 0)
//				stt_quad_draw(game, map_pos, size, 0x440044, bound, map_center);	// REVIEW: magic numbers for colors despite having enums for RGBS
//			}
//			x++;
//		}
//		y++;
//	}
//}
//
//static
//void	stt_map_render(t_game *game)
//{
//	const t_vec2	pos = (t_vec2){.x.i = SCREEN_WIDTH - 255, .y.i = 5};	// REVIEW: magic numbers for pos
//	const t_vec2	pos4 = (t_vec2){.x.i = pos.x.i
//		+ game->assets.radar_l0.texture.width, .y.i = pos.y.i
//			+ game->assets.radar_l0.texture.height};
//	const t_vec2	bound = (t_vec2){.x.i = (pos4.x.i - pos.x.i) / 2,
//		.y.i = (pos4.y.i - pos.y.i) / 2};
//	const t_vec2	map_center = (t_vec2){.x.i = pos.x.i + bound.x.i,
//		.y.i = pos.y.i + bound.y.i};
//	const t_vec2	p_pos = (t_vec2){.x.i = pos.x.i + bound.x.i - 20,
//		.y.i = pos.y.i + bound.y.i - 15};
//
//	//cub_sprite_sheet_animate(game->frame.display, &game->assets.radar_l0, pos);
//	stt_blocks_render(game, pos, bound.x.i, map_center);
//	stt_quad_draw(game, p_pos, (t_vec2){.x.i = 10, .y.i = 10},
//			0x003300, bound.x.i, map_center);
//	//cub_sprite_sheet_animate(game->frame.display, &game->assets.radar_l1, pos);
//}
//
//
//void	animate_hud(t_game *game)	// REVIEW: all functions take the game god struct, no encapsulation and separation of concerns is even attempted
//{
//	stt_map_render(game);
//}


// ASKED AI TO GENERATE THIS OFF OF THE OLD PERSONAL IMPLEMENTATION

void stt_quad_draw(t_game *game, t_vec2 pos, t_vec2 size, int32_t color, int32_t bound, t_vec2 map_center)
{
    const t_vec2 limit = (t_vec2){
        .x.i = pos.x.i + size.x.i,
        .y.i = pos.y.i + size.y.i
    };

    for (int32_t y = pos.y.i; y <= limit.y.i; y++)
    {
        for (int32_t x = pos.x.i; x <= limit.x.i; x++)
        {
            if (vec2_idist(map_center, (t_vec2){ .x.i = x, .y.i = y }) <= bound * bound)
                frame_pixel_put(game->frame.render, x, y, color);
        }
    }
}

static
void stt_blocks_render(t_game *game, t_vec2 pos, int32_t bound, t_vec2 map_center)
{
    const t_vec2 size = (t_vec2){
        .x.i = game->assets.radar_l0.texture.width / 12,
        .y.i = game->assets.radar_l0.texture.height / 12
    };

    for (int32_t y = game->player.cam.pos.y.f - 6.f; y <= game->player.cam.pos.y.f + 6.f; y++)
    {
        for (int32_t x = game->player.cam.pos.x.f - 6.f; x <= game->player.cam.pos.x.f + 6.f; x++)
        {
            if (x >= 0 && x < game->map.width && y >= 0 && y < game->map.height)
            {
                t_vec2 draw_pos = (t_vec2){
                    .x.i = x - game->player.cam.pos.x.f + 6,
                    .y.i = y - game->player.cam.pos.y.f + 6
                };

                t_vec2 map_pos = (t_vec2){
                    .x.i = pos.x.i + draw_pos.x.i * size.x.i,
                    .y.i = pos.y.i + draw_pos.y.i * size.y.i
                };

                if (game->map.ptr[x + game->map.width * y] == 0)
                    stt_quad_draw(game, map_pos, size, 0x440044, bound, map_center);
            }
        }
    }
}

static
void stt_map_render(t_game *game)
{
    // NEW RADAR POSITION
    const t_vec2 pos = (t_vec2){ .x.i = 110, .y.i = 240 };

    // --- Draw Radar Background (Layer 0) ---
    {
        t_mat32 tex = game->drawbuf.radar_l0.texture;
        tex.ptr += game->drawbuf.radar_l0.index * game->drawbuf.radar_l0.frame_size;
        cub_draw_texture(game->frame.render, tex, pos.x.i, pos.y.i);
    }

    // Compute radar bounds
    const t_vec2 pos4 = (t_vec2){
        .x.i = pos.x.i + game->assets.radar_l0.texture.width,
        .y.i = pos.y.i + game->assets.radar_l0.texture.height
    };

    const t_vec2 bound = (t_vec2){
        .x.i = (pos4.x.i - pos.x.i) / 2,
        .y.i = (pos4.y.i - pos.y.i) / 2
    };

    const t_vec2 map_center = (t_vec2){
        .x.i = pos.x.i + bound.x.i,
        .y.i = pos.y.i + bound.y.i
    };

    // --- Draw Blocks Inside Radar ---
    stt_blocks_render(game, pos, bound.x.i, map_center);

    // --- Draw Player Dot ---
    const t_vec2 p_pos = (t_vec2){
        .x.i = map_center.x.i - 8,
        .y.i = map_center.y.i - 8
    };

    stt_quad_draw(game, p_pos, (t_vec2){ .x.i = 5, .y.i = 5 }, 0x003300, bound.x.i, map_center);

    // --- Draw Radar Sweep (Layer 1) ---
    {
        t_mat32 tex = game->drawbuf.radar_l1.texture;
        tex.ptr += game->drawbuf.radar_l1.index * game->drawbuf.radar_l1.frame_size;
        cub_draw_texture(game->frame.render, tex, pos.x.i, pos.y.i);
    }
}

void animate_hud(t_game *game)
{
    stt_map_render(game);
}
