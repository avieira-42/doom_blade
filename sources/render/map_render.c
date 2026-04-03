#include "cub_structs.h"
#include "cub_utils.h"

// to refactor
/*there was a necessity to create an input_handler for the middle layer
* of the input pipeline */
void	animate_hud(t_game *game)
{
	t_mat32	texture;

	input_handler(game);
	if (game->player.map & st_checking)
	{
		game->drawbuf.radar.index = 0;
		texture = game->drawbuf.radar.texture;
		texture.ptr = game->drawbuf.radar.texture.ptr
			+ game->drawbuf.radar.frame_size * (game->drawbuf.radar.count -1);
		cub_draw_texture(game->frame.render, texture, 0, 195);
		map_render(game);
	}
	else if (game->player.map & st_raising)
	{
		texture = game->drawbuf.radar.texture;
		texture.ptr += game->drawbuf.radar.index
			* game->drawbuf.radar.frame_size;
		cub_draw_texture(game->frame.render, texture, 0, 195);
		if (game->drawbuf.radar.index == game->drawbuf.radar.count - 2)
			game->player.map |= st_checking;
	}
}
