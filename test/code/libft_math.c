#include "libft_math.h"

float   ft_abs(float n)
{
        if (n < 0)
                n = n * -1;
        return (n);
}

int     vec_max_coord(t_vecf32 vec)
{
        int     const x = ft_abs(vec.x);
        int     const y = ft_abs(vec.y);

        if (x > y)
                return (x);
        else
                return (y);
}

t_vecf32        vec_sum(t_vecf32 a, t_vecf32 b)
{
        return ((t_vecf32){a.x + b.x, a.y + b.y});
}

t_vecf32        vec_sub(t_vecf32 a, t_vecf32 b)
{
        return ((t_vecf32){b.x - a.x, b.y - a.y});
}

t_vecf32        vec_scalar_mult(t_vecf32 unit, float scalar)
{
        return ((t_vecf32){unit.x * scalar, unit.y * scalar});
}

float   vec_mag_sqd(t_vecf32 vec)
{
        return (vec.x * vec.x + vec.y * vec.y);
}

t_vecf32        vec_prepend_dir(t_vecf32 a)
{
        return (vec_unit((t_vecf32){1, - (a.x / a.y)}));
}

/*float   q_rsqrt(float n)
{
        int32_t         i;
        float           x2;
        float           y;
        const float     three_halfs = 1.5F;

        x2 = n * 0.5f;
        y = n;
        i = * (int32_t *) &y;
        i = 0x5f3759df - (i >> 1);
        y = * (float *) &i;
        y = y * (three_halfs - (x2 * y * y));
        y = y * (three_halfs - (x2 * y * y));
        y = y * (three_halfs - (x2 * y * y));
        return (y);
}*/

float   u_rsqrt(float n)
{
        t_f32           f;
        float           x2;
        const float     three_halfs = 1.5F;

        x2 = n * 0.5f;
        f.float32 = n;
        f.int32 = 0x5f3759df - (f.int32 >> 1);
        return (f.float32 * (three_halfs - (x2 * f.float32 * f.float32)));
}

float   vec_inv_mag(t_vecf32 vec)
{
        return (u_rsqrt(vec_mag_sqd(vec)));
}

t_vecf32        vec_unit(t_vecf32 vec)
{
        float   mag_sqd;
        float   inv_mag;

        mag_sqd = vec_mag_sqd(vec);
        if (mag_sqd < 1e-10f)
                return ((t_vecf32){0, 0});
        inv_mag = u_rsqrt(mag_sqd);
        return ((t_vecf32){vec.x * inv_mag, vec.y * inv_mag});
}

t_vecf32        vec_dir(t_vecf32 a, t_vecf32 b)
{
        return (vec_unit(vec_sub(a, b)));
}

float   vec_dot_product(t_vecf32 a, t_vecf32 b)
{
        return (a.x * b.x + a.y * b.y);
}

float   vec_scalar_projec(t_vecf32 a, t_vecf32 b)
{
        return (vec_dot_product(vec_unit(b), a));
}

t_vecf32        vec_proj(t_vecf32 a, t_vecf32 b)
{
        float const scalar = vec_scalar_projec(a, b);

        return (vec_sum(vec_scalar_mult(a, scalar), b));
}

float   vec_cosin()
{
        return (1);
}

float   vec_sin()
{
        return (1);
}

float   vec_tan()
{
        return (1);
}
