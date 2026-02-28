/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmlx.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:25:37 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/05 12:36:58 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMLX_H
# define CMLX_H

# include "mlx.h"
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <X11/extensions/XShm.h>
# include <X11/XKBlib.h>

# define MLX_MAX_EVENT LASTEvent

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
}				t_xvar;

void	*mlx_int_new_image(t_xvar *xvar, int width, int height, int format);

#endif
