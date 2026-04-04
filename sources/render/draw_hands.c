#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_structs.h"
#include "cub_utils.h"

void	cub_draw_hands(t_mat32 frame, t_game *game, long dt)
{
	t_sheet	*sheet;
	t_mat32	texture;
	size_t	x_tmp;
	size_t	y_tmp;

	sheet = cub_actions(&game->player, &game->map, dt);
	texture = sheet->texture;
	texture.ptr += sheet->index * sheet->frame_size;
	x_tmp = (RENDER_WIDTH - texture.width) / 2;
	y_tmp = RENDER_HEIGHT - texture.height;
	cub_draw_texture(frame, texture, x_tmp, y_tmp);
	cub_draw_radar(game, frame, &game->player.hands, dt);
}

/* blood health signlaling in the edges is working, but enemies
 * shoot player outside of sight  so NEED REVIEW, in case we have time
texture = blood.texture;
if (p_health <= 100)
	texture.ptr = blood.texture.ptr;
if (p_health <= 50 && p_health >= 20)
	texture.ptr = blood.texture.ptr += blood.texture.stride;
if (p_health <= 20)
	texture.ptr = blood.texture.ptr += blood.frame_size;
cub_draw_texture(frame, texture, 0, 0);*/
