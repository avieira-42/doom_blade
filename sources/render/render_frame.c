#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


// Blocks contains transposed rows for sequential memory access
// Everything is done in cols by rows, and then tranposed for the rendering
void	raycast(t_view *cam, t_mat8 *map, t_block *blocks, t_mat32 render_frame)
{
	size_t		x;
	t_ray		ray;
	t_ray_hit	hit;
	float		camera_x;

	x = 0;
	while (x < RENDER_WIDTH)
	{
		camera_x = 2.0f * x / (float)RENDER_WIDTH - 1.0f;
		ray = stt_raycast_init(camera_x, cam);
		hit = stt_raycast(&ray, cam, map, blocks);
		x++;
	}
}