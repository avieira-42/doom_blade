/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 20:52:02 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/18 17:09:12 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTCORE_HPP
# define FTCORE_HPP

#include <stddef.h>
#include <stdint.h>

#define MIN(x, y)			((x) < (y) ? (x) : (y))
#define MIN3(x, y, z)		(MIN(x, MIN(y, z)))
#define MIN4(x, y, z, w)	(MIN(MIN(x, y), MIN(z, w)))
#define MAX(x, y)			((x) > (y) ? (x) : (y))
#define MAX3(x, y, z)		(MAX(x, MAX(y, z)))
#define MAX4(x, y, z, w)	(MAX(MAX(x, y), MAX(z, w)))
#define ABS(x)				((x) > 0 ? (x) : -(x))
#define ABSMAX(x, y)		(MAX(ABS(x), ABS(y)))
#define ABSMIN(x, y)		(MIN(ABS(x), ABS(y)))
#define ABSDIFF(x, y)		(MAX(x, y) - MIN(x, y))
#define CLAMP(x, low, high)	(MAX(low, MIN(x, high)))
#define LOG2(x)				(63u - __builtin_clzll(x))

typedef int8_t		i8;
typedef uint8_t		u8;
typedef int16_t		i16;
typedef uint16_t	u16;
typedef int32_t		i32;
typedef uint32_t	u32;
typedef int64_t		i64;
typedef uint64_t	u64;
typedef float 		f32;
typedef double 		f64;
typedef size_t		usize;
typedef ptrdiff_t	isize;

typedef struct s_float3
{
	float	xf, yf, zf;
}	float3;

// typedef union t32
// {
// 	uint8_t		u8[4];
// 	uint16_t	u16[2];
// 	uint32_t	u32;
// 	int8_t		i8[4];
// 	int16_t		i16[2];
// 	int32_t		i32;
// 	float		f32;
// }	t32;

#endif