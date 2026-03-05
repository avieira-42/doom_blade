/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:18:57 by avieira-          #+#    #+#             */
/*   Updated: 2025/06/12 11:39:07 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_printarg(char f, va_list var)
{
	if (f == 'c')
		return (ft_printchar(va_arg(var, int)));
	else if (f == 's')
		return (ft_printstr(va_arg(var, char *)));
	else if (f == 'p')
		return (ft_printadd(va_arg(var, void *)));
	else if (f == 'd' || f == 'i')
		return (ft_printnbr(va_arg(var, int)));
	else if (f == 'u')
		return (ft_printunsign_nbr(va_arg(var, unsigned int)));
	else if (f == 'x' || f == 'X')
		return (ft_printhex(va_arg(var, unsigned int), f));
	else if (f == '%')
		return (ft_printchar(f));
	else
		return (ft_printchar('%') + (ft_printchar('f')));
}

int	ft_printf(const char *format, ...)
{
	int		size;
	va_list	var;

	if (!format)
		return (-1);
	size = 0;
	va_start(var, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			size += ft_printarg(*format, var);
		}
		else
			size += ft_printchar(*format);
		format++;
	}
	return (size);
}
/*
#include <stdio.h>
int	main(int argc, char **argv)
{
	int				i;
	int				a;
	int				*i_ptr;
	unsigned int	b;
	char			c;
	char			answer;
	char			*s_ptr;
	char			choice;

	i = 0;
	(void) argc;
	printf("type '8' when prompting the asked data type to break any loop\n");
	while (1)
	{
		a = 0;
		b = 0;
		c = 'c';
		i_ptr = &a;
		s_ptr = &c;
		answer = 'y';
		printf("choose a data type (c) (s) (p) (d) (i) (u) (x) (X) :\n");
		scanf(" %c", &choice);
		while (choice == 'c' && c != '8')
		{
			printf("char:\n");
			scanf(" %c", &c);
			printf(", %i\n", printf("%c", c));
			ft_printf(", %i\n", ft_printf("%c", c));
		}
		while (choice == 's' && answer == 'y')
		{
			if (i >= argc)
				i = 0;
			printf(", %i\n", printf("%s", argv[i]));
			ft_printf(", %i\n", ft_printf("%s", argv[i]));
			i++;
			printf("wanna check another word? (y) (n)\n");
			scanf(" %c", &answer);
		}
		while (choice == 'p' && c != '8' && a != 8)
		{
			printf("int, char:\n");
			scanf(" %i %c", &a, &c);
			printf(", %i\n", printf("%p, %p", i_ptr, s_ptr));
			ft_printf(", %i\n", ft_printf("%p, %p", i_ptr, s_ptr));
		}
		while ((choice == 'i' || choice == 'd') && a != 8)
		{
			printf("int:\n");
			scanf(" %i", &a);
			printf(", %i\n", printf("%i", a));
			ft_printf(", %i\n", ft_printf("%i", a));
		}
		while (choice == 'u' && b != 8)
		{
			printf("unsigned int:\n");
			scanf(" %u", &b);
			printf(", %i\n", printf("%u", b));
			ft_printf(", %i\n", ft_printf("%u", b));
		}
		while (choice == 'x' && a != 8)
		{
			printf("int:\n");
			scanf(" %i", &a);
			printf(", %i\n", printf("%x", a));
			ft_printf(", %i\n", ft_printf("%x", a));
		}
		while (choice == 'X' && a != 8)
		{
			printf("int:\n");
			scanf(" %i", &a);
			printf(", %i\n", printf("%X", a));
			ft_printf(", %i\n", ft_printf("%X", a));
		}
	}
}*/
