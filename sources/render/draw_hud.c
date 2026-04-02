#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_structs.h"
#include "cub_utils.h"

void	cub_draw_crosshair(uint32_t *ptr)
{
	size_t	index;

	index = (SCREEN_HEIGHT / 2 - 1) * SCREEN_WIDTH + (SCREEN_WIDTH / 2 - 1);
	ptr[index] = 0;
	ptr[index + 1] = 0;
	ptr[index + 2] = 0;
	ptr[index + 3] = 0;
	index += SCREEN_WIDTH;
	ptr[index] = 0;
	ptr[index + 1] = 0xFFFFFFFF;
	ptr[index + 2] = 0xFFFFFFFF;
	ptr[index + 3] = 0;
	index += SCREEN_WIDTH;
	ptr[index] = 0;
	ptr[index + 1] = 0xFFFFFFFF;
	ptr[index + 2] = 0xFFFFFFFF;
	ptr[index + 3] = 0;
	index += SCREEN_WIDTH;
	ptr[index] = 0;
	ptr[index + 1] = 0;
	ptr[index + 2] = 0;
	ptr[index + 3] = 0;
}

void	cub_draw_hud(t_mat32 frame, t_drawbuf *drawbuf)
{
	t_mat32	texture;

	texture = drawbuf->hands.texture;
	texture.ptr += drawbuf->hands.index * drawbuf->hands.frame_size;		// Hands
	size_t	x_tmp = (RENDER_WIDTH - texture.width) / 2;
	size_t	y_tmp = RENDER_HEIGHT - texture.height;
	cub_draw_texture(frame, texture, x_tmp, y_tmp);
	// .. Radar
	// TMP RADAR >>>>
	/*texture = drawbuf->radar_l0.texture;
	texture.ptr += drawbuf->radar.index * drawbuf->radar_l0.frame_size;
	cub_draw_texture(frame, texture, 110, 240);
	texture = drawbuf->radar_l1.texture;
	texture.ptr += drawbuf->radar.index * drawbuf->radar_l1.frame_size;
	cub_draw_texture(frame, texture, 110, 240);*/
	// <<<< TMP RADAR
}
