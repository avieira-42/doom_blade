#include "cub_types.h"
#include "cub_structs.h"
#include "cub_utils.h"

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

// REVIEW: design bug makes it so rendering mutates animation state. 
// Animation state should occur regardless of rendering, because things that aren't visible aren't rendered but still loop
void cub_sprite_sheet_animate(t_mat32 frame, t_sheet *sheet, t_vec2 pos)
{
	size_t  frame_size;
	t_mat32 texture;

	frame_size = sheet->texture.width * sheet->texture.height;
	texture = sheet->texture;
	texture.ptr += sheet->iterator * frame_size;
	cub_sprite_sheet_update(sheet);
	cub_draw_texture(frame, texture, pos);
}
