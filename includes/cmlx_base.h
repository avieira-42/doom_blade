#ifndef CMLX_BASE_H
# define CMLX_BASE_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include <SDL2/SDL_image.h>

// TO REMOVE >>>>
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <X11/extensions/XShm.h>
# include <X11/XKBlib.h>
# include "mlx.h"
// <<<<<< TO REMOVE

# define MLX_MAX_EVENT LASTEvent

typedef struct s_str
{
	char	*ptr;
	size_t	length;
}	t_str;

typedef struct s_mat
{
	void		*ptr;
	uint16_t	width;
	uint16_t	height;
	uint16_t	depth;
	uint8_t		type_size;
	uint8_t		flags;	// MSB -> LSB: float, transposed, 
}	t_mat;

typedef struct s_mat32
{
	uint32_t	*ptr;
	uint16_t	width;
	uint16_t	height;
	uint16_t	depth;
	uint16_t	stride;
}	t_mat32;

typedef union t_32
{
	float		f;
	int32_t		i;
	uint32_t	u;
	struct
	{
		uint8_t	b;	// LSB
		uint8_t	g;
		uint8_t	r;
		uint8_t	a;	// MSB
	};
}	t_32;

typedef struct s_vec2
{
	t_32	x;
	t_32	y;
}	t_vec2;

typedef struct s_vec3
{
	t_32	x;
	t_32	y;
	t_32	z;
}	t_vec3;

typedef struct s_vec4
{
	t_32	x;
	t_32	y;
	t_32	z;
	t_32	w;
}	t_vec4;

// 0xAARRGGBB
typedef union u_argb
{
	uint32_t	i;
	struct
	{
		uint8_t	b;
		uint8_t	g;
		uint8_t	r;
		uint8_t	a;
	};
}	t_argb;

typedef struct s_event_list
{
	int		mask;
	int		(*hook)();
	void	*param;
}	t_event_list;

typedef struct s_win_list
{
	Window				window;
	GC					gc;
	struct s_win_list	*next;
	int					(*mouse_hook)();
	int					(*key_hook)();
	int					(*expose_hook)();
	void				*mouse_param;
	void				*key_param;
	void				*expose_param;
	t_event_list		hooks[MLX_MAX_EVENT];
}	t_win_list;

typedef struct s_img
{
	XImage			*image;
	Pixmap			pix;
	GC				gc;
	int				size_line;
	int				bpp;
	int				width;
	int				height;
	int				type;
	int				format;
	char			*data;
	XShmSegmentInfo	shm;
}	t_img;

typedef struct s_xvar
{
	Display		*display;
	Window		root;
	int			screen;
	int			depth;
	Visual		*visual;
	Colormap	cmap;
	int			private_cmap;
	t_win_list	*win_list;
	int			(*loop_hook)();
	void		*loop_param;
	int			use_xshm;
	int			pshm_format;
	int			do_flush;
	int			decrgb[6];
	Atom		wm_delete_window;
	Atom		wm_protocols;
	int			end_loop;
}	t_xvar;

void	*mlx_int_new_image(t_xvar *xvar, int width, int height, int format);

#endif
