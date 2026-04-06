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
	x_tmp = ((render_width - texture.width) / 2) + 5;	// TODO: defines
	y_tmp = render_height - texture.height;
	cub_draw_texture(frame, texture, x_tmp, y_tmp);
	cub_draw_radar(game, frame, &game->player.hands, dt);
}
