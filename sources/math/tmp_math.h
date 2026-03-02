#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct s_image
{
	uint32_t	*ptr;
	uint32_t	width;
	uint32_t	height;
}	t_image;