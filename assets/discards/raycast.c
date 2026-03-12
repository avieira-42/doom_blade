t_ray_hit	raycast2(float camera_x, t_view *cam, t_game *game)
{
	t_ray		ray;
	t_ray_hit	hit;
	uint8_t		block_index;

	ray = stt_raycast_init(camera_x, cam);
	if (stt_dda(&ray, &game->map, &block_index) == 0)
	{
		hit.perp_dist = (ray.map_pos.x.i - cam->pos.x.f + (1 - ray.step.x.i) / 2) / ray.ray_dir.x.f;
		if (ray.step.x.i > 0)
			hit.texture = game->blocks[block_index].west;
		else
			hit.texture = game->blocks[block_index].east;
		hit.x_pos_texture = cam->pos.y.f + hit.perp_dist * ray.ray_dir.y.f;
	}
	else
	{
		hit.perp_dist = (ray.map_pos.y.i - cam->pos.y.f + (1 - ray.step.y.i) / 2) / ray.ray_dir.y.f;
		if (ray.step.y.i > 0)
			hit.texture = game->blocks[block_index].north;
		else
			hit.texture = game->blocks[block_index].south;
		hit.x_pos_texture = cam->pos.x.f + hit.perp_dist * ray.ray_dir.x.f;
	}
	hit.x_pos_texture -= floorf(hit.x_pos_texture);
	return (hit);
}