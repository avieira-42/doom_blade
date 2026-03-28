void    render_textured_floor(t_game *game)
{
    t_mat32     frame;
    t_mat32     floor_tex;
    t_mat32     ceil_tex;
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

    frame = game->frame.render;
    floor_tex = game->blocks[0].south;
    ceil_tex = game->blocks[0].north;

    y = frame.height / 2;
    while (y < frame.height)
    {
        rayDirX0 = game->player.cam.dir.x.f - game->player.cam.plane.x.f;
        rayDirY0 = game->player.cam.dir.y.f - game->player.cam.plane.y.f;
        rayDirX1 = game->player.cam.dir.x.f + game->player.cam.plane.x.f;
        rayDirY1 = game->player.cam.dir.y.f + game->player.cam.plane.y.f;

        p = y - frame.height / 2;
        posZ = 0.5f * frame.height;
        rowDist = posZ / p;

        stepX = rowDist * (rayDirX1 - rayDirX0) / frame.width;
        stepY = rowDist * (rayDirY1 - rayDirY0) / frame.width;

        floorX = game->player.cam.pos.x.f + rowDist * rayDirX0;
        floorY = game->player.cam.pos.y.f + rowDist * rayDirY0;

        x = 0;
        while (x < frame.width)
        {
            tx = (int)(floor_tex.width * (floorX - (int)floorX));
            ty = (int)(floor_tex.height * (floorY - (int)floorY));

            tx = tx & (floor_tex.width - 1);
            ty = ty & (floor_tex.height - 1);

            floor_color = floor_tex.ptr[ty * floor_tex.width + tx];
            ceil_color = ceil_tex.ptr[ty * ceil_tex.width + tx];

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
