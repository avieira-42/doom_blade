#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_structs.h"
#include "cub_utils.h"

void		planes_cast(t_mat32 frame, t_mat32 floor, t_mat32 ceiling, t_view cam)
{
    int         y;
    int         x;
    float       rayDirX0;
    float       rayDirY0;
    float       rayDirX1;
    float       rayDirY1;
    int         p;
    float       posZ;
    float       rowDist;
    float       stepX;
    float       stepY;
    float       floorX;
    float       floorY;
    int         tx;
    int         ty;
    uint32_t    floor_color;
    uint32_t    ceil_color;
    uint32_t    *col_ptr;
    int         ceil_y;


    y = frame.height / 2;
    while (y < frame.height)
    {
        rayDirX0 = cam.dir.x.f - cam.plane.x.f;
        rayDirY0 = cam.dir.y.f - cam.plane.y.f;
        rayDirX1 = cam.dir.x.f + cam.plane.x.f;
        rayDirY1 = cam.dir.y.f + cam.plane.y.f;

        p = y - frame.height / 2;
        posZ = 0.5f * frame.height;
        rowDist = posZ / p;

        stepX = rowDist * (rayDirX1 - rayDirX0) / frame.width;
        stepY = rowDist * (rayDirY1 - rayDirY0) / frame.width;

        floorX = cam.pos.x.f + rowDist * rayDirX0;
        floorY = cam.pos.y.f + rowDist * rayDirY0;

        x = 0;
        while (x < frame.width)
        {
            tx = (int)(floor.width * (floorX - (int)floorX));
            ty = (int)(floor.height * (floorY - (int)floorY));

            tx = tx & (floor.width - 1);
            ty = ty & (floor.height - 1);

            floor_color = floor.ptr[ty * floor.width + tx];
            ceil_color = ceiling.ptr[ty * ceiling.width + tx];

            col_ptr = frame.ptr + (x * frame.stride);

            /* FLOOR */
            col_ptr[y] = floor_color;

            /* CEILING */
            ceil_y = frame.height - y - 1;
            col_ptr[ceil_y] = ceil_color;

            floorX += stepX;
            floorY += stepY;
            x++;
        }
        y++;
    }
}
