#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_structs.h"
#include "cub_utils.h"

void	cub_draw_damage(t_mat32 frame, t_game *game, long dt)
{
	/*blood health signlaling in the edges is working, but enemies
		* shoot player outside of sight  so NEED REVIEW, in case we have time*/

	const int32_t	p_health = game->player.health;
	t_mat32			texture;

	if (p_health > 100)
		return ;
	texture = game->assets.hud_blood.texture;
	if (p_health <= 100)
		texture.ptr = game->assets.hud_blood.texture.ptr;
	if (p_health <= 50 && p_health >= 20)
		texture.ptr = game->assets.hud_blood.texture.ptr
			+ game->assets.hud_blood.frame_size;
	if (p_health <= 20)
		texture.ptr = game->assets.hud_blood.texture.ptr
			+ game->assets.hud_blood.frame_size * 2;
	cub_draw_texture(frame, texture, 0, 0);
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

/*cub_advance_animation(&e->running, dt);
  if (is_enemy_shooting())
  {
  e->state &= ~(size_t)e_running;
  e->state |= e_shooting;
  e->shooting.index = 0;
  e->shooting.frame_dt = 0;
  player->health -= 20;
  }*/
