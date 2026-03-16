#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cub_structs.h"
#include "cub_utils.h"

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
    sprite = read_xpm(mlx_ptr, filepath, (const char **)&filepath);
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

void	sprites_init(t_game *game)
{
	game->shoot = stt_sprite_sheet_init(game->mlx,
			"assets/sprites/xpm/hud/hands/hands_shooting", 5, ".xpm");
	game->shoot->loops_per_sprite = 2;
	game->shoot->dt = &game->time.delta;
	game->walk = stt_sprite_sheet_init(game->mlx,
			"assets/sprites/xpm/hud/hands/hands_walking", 8, ".xpm");
	game->walk->loops_per_sprite = 4;
	game->walk->dt = &game->time.delta;
	game->reload = stt_sprite_sheet_init(game->mlx,
			"assets/sprites/xpm/hud/hands/hands_reloading", 33, ".xpm");
	game->reload->loops_per_sprite = 3;
	game->reload->dt = &game->time.delta;
	// cards
	game->ammo = stt_sprite_sheet_init(game->mlx,
			"assets/sprites/xpm/hud/hud_ammo/ammo", 10, ".xpm");
	game->ammo->dt = &game->time.delta;
	game->health = stt_sprite_sheet_init(game->mlx,
			"assets/sprites/xpm/hud/hud_health/health", 10, ".xpm");
	game->health->dt = &game->time.delta;
	game->pill = stt_sprite_sheet_init(game->mlx,
			"assets/sprites/xpm/hud/hud_pill/pill", 2, ".xpm");
	game->pill->dt = &game->time.delta;
	// BACKGROUND
	game->city = stt_sprite_sheet_init(game->mlx,
			"assets/sprites/xpm/tiles/city", 4, ".xpm");
	game->city->dt = &game->time.delta;
}
