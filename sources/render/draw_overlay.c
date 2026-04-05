#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_structs.h"
#include "cub_utils.h"

void	cub_draw_crosshair(uint32_t *ptr)
{
	size_t	index;

	index = (screen_height / 2 - 1) * screen_width + (screen_width / 2 - 1);
	ptr[index] = 0;
	ptr[index + 1] = 0;
	ptr[index + 2] = 0;
	ptr[index + 3] = 0;
	index += screen_width;
	ptr[index] = 0;
	ptr[index + 1] = 0xFFFFFFFF;
	ptr[index + 2] = 0xFFFFFFFF;
	ptr[index + 3] = 0;
	index += screen_width;
	ptr[index] = 0;
	ptr[index + 1] = 0xFFFFFFFF;
	ptr[index + 2] = 0xFFFFFFFF;
	ptr[index + 3] = 0;
	index += screen_width;
	ptr[index] = 0;
	ptr[index + 1] = 0;
	ptr[index + 2] = 0;
	ptr[index + 3] = 0;
}

// REVIEW: blood isnt animation, why is it sheet?
// TODO: Move radar here
void	cub_draw_blood(t_mat32 frame, t_game *game)
{
	const float	health = (float) game->player.health / PLAYER_HEALTH;
	t_sheet		*sheet;

	sheet = &game->assets.hud_blood;
	if (health > 0.8f)
		return ;
	else if (health >= 0.5f)
	{
		sheet->index = 0;
		cub_draw_sheet(frame, sheet, 0, 0);
	}
	else if (health >= 0.2f)
	{
		sheet->index = 1;
		cub_draw_sheet(frame, sheet, 0, 0);
	}
	else
	{
		sheet->index = 2;
		cub_draw_sheet(frame, sheet, 0, 0);
	}
}
