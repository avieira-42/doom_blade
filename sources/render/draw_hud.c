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

void	cub_draw_hud(t_mat32 frame, t_drawbuf *drawbuf,
		int32_t p_health, t_sheet blood)
{
	t_mat32	texture;
	size_t	x_tmp;
	size_t	y_tmp;

	texture = drawbuf->hands.texture;
	texture.ptr += drawbuf->hands.index * drawbuf->hands.frame_size;
	x_tmp = (RENDER_WIDTH - texture.width) / 2;
	y_tmp = RENDER_HEIGHT - texture.height;
	cub_draw_texture(frame, texture, x_tmp, y_tmp);

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
}
