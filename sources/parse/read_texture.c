#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "game_types.h"
#include "game_prototypes.h"
#include "game_defines.h"
#include <SDL3/SDL.h>

static inline
void	stt_clean_texture(uint32_t *ptr, size_t length)
{
	size_t	i;

	i = 0;
	while (i < length)
	{
		if (ptr[i] == ALPHA_SENTINEL1 || ptr[i] == ALPHA_SENTINEL2)
			ptr[i] = 0;
		i++;
	}
}

static
SDL_Surface *cub_load_surface(const char *path)
{
	SDL_Surface	*loaded;
	SDL_Surface	*converted;

	loaded = SDL_LoadSurface(path);
	if (loaded == NULL)
		return NULL;
	converted = SDL_ConvertSurface(loaded, SDL_PIXELFORMAT_RGBA8888);
	SDL_DestroySurface(loaded);
	return converted;
}

static
SDL_Surface	*stt_read_texture(const char *filename, const char **filename_ptr)
{
	SDL_Surface	*im;
	SDL_Surface	*im32;
	char		buffer[256];
	char		*path;
	const char	*path_end = buffer + sizeof(buffer) - 1;

	path = buffer;
	while (isspace(*filename))
		filename++;
	while (*filename != 0 && !isspace(*filename))
	{
		if (path >= path_end)
			return (NULL);	// Path too long, TODO: print
		*path++ = *filename++;
	}
	*path = 0;
	if (filename_ptr != NULL)
		*filename_ptr = filename;
	im = cub_load_surface(buffer);
	if (im == NULL)
		return (NULL);
	im32 = SDL_ConvertSurface(im, SDL_PIXELFORMAT_ARGB8888);
	SDL_DestroySurface(im);
	return (im32);
}

int	cub_read_texture(t_game *game, t_mat32 *dst, const char *filename, const char **filename_ptr)
{
	SDL_Surface	*img;
	t_mat32		src;
	size_t		size;

	img = stt_read_texture(filename, filename_ptr);
	if (img == NULL)
		exit_log(2, "Failed to load image", 1);
	if (dst->ptr == NULL)	// DST needs to have depth defined
	{
		size = (size_t)(img->h * img->w) * sizeof(uint32_t) * dst->depth;
		*dst = (t_mat32){malloc(size), img->w, img->h, dst->depth, img->h};
		if (dst->ptr == NULL)
		{
			SDL_DestroySurface(img);
			exit_log(2, "Error_leading dst data buffer", 1);
		}
	}
	src = (t_mat32){(uint32_t *)img->pixels, img->w, img->h, 1, img->w};
	ft_near_scale_t(*dst, src);
	SDL_DestroySurface(img);
	stt_clean_texture(dst->ptr, dst->height * dst->width);
	if (src.width != dst->width || src.height != dst->height)
		exit_log(2, "Sheet frame dimensions do not match", 1);
	return (0);
}
