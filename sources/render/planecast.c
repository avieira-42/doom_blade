#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_utils.h"

void	planecast(t_frame frame, t_mat32 floor_mat,
		t_mat32 ceil_mat, t_view cam)
{
	floorcast(frame, floor_mat, cam);
	ceilingcast(frame, ceil_mat, cam);
}
