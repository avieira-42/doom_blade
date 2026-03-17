#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cub_structs.h"
#include "cub_utils.h"

static char	*populate_array(char *array, int i, int n)
{
	char	c;

	if (n > 9)
	{
		populate_array(array, i - 1, n / 10);
		n = n % 10;
	}
	c = '0' + n;
	array[i] = c;
	return (array);
}

static
void	stt_path_cat(char *path, t_anim *anim, int32_t sprite_number)
{
	size_t		i;
	size_t		j;
	const char	*number = ft_itoa(sprite_number + 1);

	i = 0;
	j = 0;
	while (anim->base_path[i])
		path[j++] = anim->base_path[i++];
	i = 0;
	while(number[i])
		path[j++] = number[i++];
	i = 0;
	while (anim->file_type[i])
		path[j++] = anim->file_type[i++];
	path[j] = '\0';
}

static
void    stt_load_sprite(void *mlx_ptr, t_img *sprite, char *filepath, int32_t *endian)
{
    sprite->image = mlx_xpm_file_to_image(mlx_ptr, filepath, &sprite->width, &sprite->height);
    if (!sprite->image)
    {
        sprite->data = NULL;
        return ;
    }
    sprite->data = mlx_get_data_addr(sprite->image,
            &sprite->bpp, &sprite->size_line, endian);
}

static
void    stt_load_animation(void *mlx_ptr, t_anim *animation)
{
    int32_t     i;
    char		path[200];

	animation->sheet = malloc(sizeof(t_img) * animation->size);
    i = 0;
    while (i < animation->size)
    {
		stt_path_cat(path, animation, i);
		stt_load_sprite(mlx_ptr, &(animation->sheet[i]), path, &animation->endian);
        i++;
    }
}

static
t_anim	*stt_sprite_sheet_init(t_xvar *mlx, char *base_path,
		int32_t size, char *file_type)
{
	t_anim	*anim;

	anim = malloc(sizeof(t_anim));
	anim->base_path = ft_strdup(base_path);
	anim->size = size;
	anim->file_type = ft_strdup(file_type);
	anim->iterator = 0;
	anim->end = false;
	stt_load_animation(mlx, anim);
	return (anim);
}

static
void	stt_sprites_init(t_game *game)
{
	game->hud.shoot = stt_sprite_sheet_init(game->mlx,
			"assets/sprites/xpm/hud/hands/hands_shooting", 5, ".xpm");
	game->hud.shoot->loops_per_sprite = 2;
	game->hud.shoot->dt = &game->time.delta;
	game->hud.walk = stt_sprite_sheet_init(game->mlx,
			"assets/sprites/xpm/hud/hands/hands_walking", 8, ".xpm");
	game->hud.walk->loops_per_sprite = 4;
	game->hud.walk->dt = &game->time.delta;
	game->hud.reload = stt_sprite_sheet_init(game->mlx,
			"assets/sprites/xpm/hud/hands/hands_reloading", 33, ".xpm");
	game->hud.reload->loops_per_sprite = 3;
	game->hud.reload->dt = &game->time.delta;
	// cards
	game->hud.ammo = stt_sprite_sheet_init(game->mlx,
			"assets/sprites/xpm/hud/hud_ammo/ammo", 10, ".xpm");
	game->hud.ammo->dt = &game->time.delta;
	game->hud.health = stt_sprite_sheet_init(game->mlx,
			"assets/sprites/xpm/hud/hud_health/health", 10, ".xpm");
	game->hud.health->dt = &game->time.delta;
	game->hud.pill = stt_sprite_sheet_init(game->mlx,
			"assets/sprites/xpm/hud/hud_pill/pill", 2, ".xpm");
	game->hud.pill->dt = &game->time.delta;
	// BACKGROUND
	game->hud.city = stt_sprite_sheet_init(game->mlx,
			"assets/sprites/xpm/tiles/city", 4, ".xpm");
	game->hud.city->dt = &game->time.delta;
}

void	hud_init(t_game *game)
{
	stt_sprites_init(game);
	game->hud.hands_reload = false;
	game->hud.hands_shoot = false;
	game->hud.shoot_sound = false;
}
