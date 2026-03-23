#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cub_utils.h"
#include "cmlx.h"

static
t_img   *stt_load_img(t_xvar *mlx, t_str path, size_t count)
{
    t_img               *img;
    static const char   append_str[5] = ".xpm";
    int                 tmp[2];
    size_t              number_length;
    char                *append_ptr;

    append_ptr = path.ptr + path.length;
    number_length = ft_itoa_r((int64_t)count, append_ptr);
    append_ptr += number_length;
    ft_memcpy(append_ptr, append_str, sizeof(append_str));
    img = mlx_xpm_file_to_image(mlx, path.ptr, tmp, tmp + 1);
    return (img);
}

static
int stt_load_sheet(t_xvar *mlx, t_mat32 sprite, t_str path, size_t count)
{
    size_t          i;
    t_img           *img;
    const size_t    stride = sprite.rows * sprite.cols;

    i = 1;  // TODO: Review image indexing
    sprite.ptr += stride;
    while (i < count)
    {
        img = stt_load_img(mlx, path, i);
        if (img == NULL)
            return (-1);
        if (img->width != sprite.cols || img->height != sprite.rows)
            return (mlx_destroy_image(mlx, img), -1);

        ft_memcpy(sprite.ptr, img->data, stride * sizeof(uint32_t));

		// transpose after loading
        //ft_transpose(&(t_mat32){ sprite.ptr, sprite.rows, sprite.cols, 1, 0 });

        mlx_destroy_image(mlx, img);
        sprite.ptr += stride;
        i++;
    }
    return (0);
}

#define SENTINEL_VALUE 0xFF000000

void    stt_clean_texture(t_mat32 texture)
{
    size_t          i;
    const size_t    length = texture.rows * texture.cols * texture.depth;

    i = 0;
    while (i < length)
    {
        if (texture.ptr[i] == SENTINEL_VALUE)
            texture.ptr[i] = 0;
        i++;
    }
}

t_sheet cub_read_spritesheet(t_xvar *mlx, const char *base_path, size_t count)
{
    t_sheet     sprite;
    t_mat32     texture;
    t_img       *img;
    char        buffer[256];
    const t_str path = {buffer, ft_strlen(base_path)};

    ft_memset(&sprite, 0, sizeof(sprite));
    if (path.length + 32 > sizeof(buffer))
        return (sprite);
    ft_memcpy(buffer, base_path, path.length + 1);

    img = stt_load_img(mlx, path, 1);
    if (img == NULL)
        return (sprite);

    texture = (t_mat32){0, img->height, img->width, count, 0};
    texture.ptr = malloc((size_t)(img->height * img->width) * count * sizeof(uint32_t));
    if (texture.ptr == NULL)
        return (mlx_destroy_image(mlx, img), sprite);

    ft_memcpy(texture.ptr, img->data, texture.cols * texture.rows * sizeof(uint32_t));

	//transpose after loading
    //ft_transpose(&(t_mat32){ texture.ptr, texture.rows, texture.cols, 1, 0 });

    mlx_destroy_image(mlx, img);

    if (stt_load_sheet(mlx, texture, path, count) == -1)
        return (free(texture.ptr), sprite);

    sprite.texture = texture;
    return (sprite);
}

void    sprites_init(t_game *game)
{
    game->assets.shoot = cub_read_spritesheet(game->mlx, "assets/sprites/xpm/hud/hands/hands_shooting", 6);
    game->assets.shoot.end = false;
	game->assets.shoot.loops_per_sprite = 5;
	game->assets.shoot.iterator = 0;
	game->assets.shoot.counter = 0;
    if (game->assets.shoot.texture.ptr == NULL)
        return ;
    game->assets.walk = cub_read_spritesheet(game->mlx, "assets/sprites/xpm/hud/hands/hands_walking", 8);
    game->assets.walk.end = false;
	game->assets.walk.loops_per_sprite = 5;
    game->assets.reload = cub_read_spritesheet(game->mlx, "assets/sprites/xpm/hud/hands/hands_reloading", 34);
    game->assets.reload.end = false;
	game->assets.reload.loops_per_sprite = 5;
    game->assets.ammo = cub_read_spritesheet(game->mlx, "assets/sprites/xpm/hud/hud_ammo/ammo", 10);
    game->assets.ammo.end = false;
	game->assets.ammo.loops_per_sprite = 0;
    game->assets.health = cub_read_spritesheet(game->mlx, "assets/sprites/xpm/hud/hud_health/health", 10);
    game->assets.health.end = true;
	game->assets.health.loops_per_sprite = 0;
    game->assets.pill = cub_read_spritesheet(game->mlx, "assets/sprites/xpm/hud/hud_pill/pill", 2);
    game->assets.pill.end = true;
	game->assets.pill.loops_per_sprite = 0;
    game->assets.city = cub_read_spritesheet(game->mlx, "assets/sprites/xpm/tiles/city", 4);
    game->assets.city.end = true;
	game->assets.city.loops_per_sprite = 0;
}

