void RenderBlocks(world* w, int zoom, double x_center, double y_center);
void RenderObjects(world* w, int zoom, double x_center, double y_center);
void RenderBuildings(world* w, int zoom_original, double x_center, double y_center, double zoom_multiplier);
void RenderHighBlocks(world* w, int zoom_original, double x_center, double y_center, double zoom_multiplier);
void RenderHighBlocksOld(world* w, int zoom_original, double x_center, double y_center, double zoom_multiplier);


void RenderWorld(world* w, int zoom, double x_center, double y_center)
{
    RenderBlocks(w, zoom, x_center, y_center);
    RenderObjects(w, zoom, x_center, y_center);
    RenderHighBlocks(w, zoom, x_center, y_center, 1.5);
    RenderBuildings(w, zoom, x_center, y_center, 1.5);

}


void RenderBlocks(world* w, int zoom, double x_center, double y_center)
{
    double x_offset = x_center-(double)ScreenW/(double)zoom/2.0;
    double y_offset = y_center-(double)ScreenH/(double)zoom/2.0;

    double starting_x = (double)x_center - (double)ScreenW/2.0/double(zoom);
    double starting_y = (double)y_center - (double)ScreenH/2.0/double(zoom);
    double ending_x = x_center+double(x_center-starting_x);
    double ending_y = y_center+double(y_center-starting_y);

    int x_begin = (int)max(0.0, floor(starting_x));
    int y_begin = (int)max(0.0, floor(starting_y));
    int x_end = (int)min((double)w->GetWidth(), ceil(ending_x));
    int y_end = (int)min((double)w->GetHeight(), ceil(ending_y));


    for(int y=y_begin; y<y_end; y++)
    {
        for(int x=x_begin; x<x_end; x++)
        {
            int xp = int((double)x*(double)zoom-x_offset*(double)zoom);
            int yp = int((double)y*(double)zoom-y_offset*(double)zoom);

            //
            if(w->CheckValidPos(x,y))
            {
                ALLEGRO_BITMAP* tex_pointer = blockDB.GetBlockByID(w->GetBlock(x,y))->GetTextureFromPos(x, y);
                int rot = w->GetRotation(x,y);
                int tex_size = al_get_bitmap_width(tex_pointer);
                al_draw_scaled_rotated_bitmap(tex_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*rot, 0);
                bool edges[4] = {0,0,0,0};
                if(blockDB.GetBlockByID(w->GetBlock(x,y))->edge_effect)
                {
                    if(   (w->CheckValidPos(x,y-1) && w->GetBlock(x,y-1) != w->GetBlock(x,y)) || !w->CheckValidPos(x,y-1)   )
                        edges[0] = true;
                    if(   (w->CheckValidPos(x+1,y) && w->GetBlock(x+1,y) != w->GetBlock(x,y)) || !w->CheckValidPos(x+1,y)   )
                        edges[1] = true;
                    if(   (w->CheckValidPos(x,y+1) && w->GetBlock(x,y+1) != w->GetBlock(x,y)) || !w->CheckValidPos(x,y+1)   )
                        edges[2] = true;
                    if(   (w->CheckValidPos(x-1,y) && w->GetBlock(x-1,y) != w->GetBlock(x,y)) || !w->CheckValidPos(x-1,y)   )
                        edges[3] = true;

                    ALLEGRO_BITMAP* edge_pointer = blockDB.GetBlockByID(w->GetBlock(x,y))->edge_texture_side;
                    ALLEGRO_BITMAP* corner_pointer = blockDB.GetBlockByID(w->GetBlock(x,y))->edge_texture_corner;

                    if(edges[0])
                        al_draw_scaled_rotated_bitmap(edge_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*1, 0);
                    if(edges[1])
                        al_draw_scaled_rotated_bitmap(edge_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*2, 0);
                    if(edges[2])
                        al_draw_scaled_rotated_bitmap(edge_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*3, 0);
                    if(edges[3])
                        al_draw_scaled_rotated_bitmap(edge_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*4, 0);

                    if(edges[3] && edges[0])
                        al_draw_scaled_rotated_bitmap(corner_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*0, 0);
                    if(edges[0] && edges[1])
                        al_draw_scaled_rotated_bitmap(corner_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*1, 0);
                    if(edges[1] && edges[2])
                        al_draw_scaled_rotated_bitmap(corner_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*2, 0);
                    if(edges[2] && edges[3])
                        al_draw_scaled_rotated_bitmap(corner_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*3, 0);
                }
            }
        }
    }


}

void RenderObjects(world* w, int zoom, double x_center, double y_center)
{
    double x_offset = x_center-(double)ScreenW/(double)zoom/2.0;
    double y_offset = y_center-(double)ScreenH/(double)zoom/2.0;

    double starting_x = (double)x_center - (double)ScreenW/2.0/double(zoom);
    double starting_y = (double)y_center - (double)ScreenH/2.0/double(zoom);
    double ending_x = x_center+double(x_center-starting_x);
    double ending_y = y_center+double(y_center-starting_y);

    int x_begin = (int)max(0.0, floor(starting_x));
    int y_begin = (int)max(0.0, floor(starting_y));
    int x_end = (int)min((double)w->GetWidth(), ceil(ending_x));
    int y_end = (int)min((double)w->GetHeight(), ceil(ending_y));

    for(int i=0; i<w->object.size(); i++)
    {
        int obj_w = objectDB.GetObjectByID(w->object[i].id)->w;
        int obj_h = objectDB.GetObjectByID(w->object[i].id)->h;

        if(w->object[i].x+obj_w < x_begin ||
           w->object[i].y+obj_h < y_begin ||
           w->object[i].x > x_end ||
           w->object[i].y > y_end)
            continue;


        int xp = int((double)w->object[i].x*(double)zoom-x_offset*(double)zoom);
        int yp = int((double)w->object[i].y*(double)zoom-y_offset*(double)zoom);

        ALLEGRO_BITMAP* tex_pointer = objectDB.GetObjectByID(w->object[i].id)->texture;
        int rot = w->object[i].rot;
        int tex_size_x = al_get_bitmap_width(tex_pointer);
        int tex_size_y = al_get_bitmap_height(tex_pointer);

        al_draw_scaled_rotated_bitmap(tex_pointer, tex_size_x/2, tex_size_y/2, xp+(zoom/2)*obj_w, yp+(zoom/2)*obj_h, (float)zoom/(float)tex_size_x*obj_w, (float)zoom/(float)tex_size_y*obj_w, rads_90deg*rot, 0);
    }



}


void RenderBuildings(world* w, int zoom_original, double x_center, double y_center, double zoom_multiplier)
{
    double v_near = 1;

    ALLEGRO_VERTEX hbv[20];
    ALLEGRO_BITMAP* tex_pointer[5];

    ALLEGRO_TRANSFORM t;
    al_identity_transform(&t);
    al_perspective_transform(&t, -ScreenW/2, -ScreenH/2, v_near, ScreenW/2, ScreenH/2, 10000);
    al_use_projection_transform(&t);

    double zoom = (double)zoom_original;
    zoom_multiplier = (zoom_multiplier-1) * (double(zoom_original)/10000.0) + 1;
    double x_offset = x_center-(double)ScreenW/(double)zoom/2.0;
    double y_offset = y_center-(double)ScreenH/(double)zoom/2.0;

    double starting_x = (double)x_center - (double)ScreenW/2.0/double(zoom);
    double starting_y = (double)y_center - (double)ScreenH/2.0/double(zoom);
    double ending_x = x_center+double(x_center-starting_x);
    double ending_y = y_center+double(y_center-starting_y);

    int x_begin = (int)max(0.0, floor(starting_x));
    int y_begin = (int)max(0.0, floor(starting_y));
    int x_end = (int)min((double)w->GetWidth(), ceil(ending_x));
    int y_end = (int)min((double)w->GetHeight(), ceil(ending_y));

    for(int i=0; i<w->building.size(); i++)
    {
        if(w->building[i].x+buildingDB.GetBuildingByID(w->building[i].id)->w < x_begin ||
           w->building[i].y+buildingDB.GetBuildingByID(w->building[i].id)->h < y_begin ||
           w->building[i].x > x_end ||
           w->building[i].y > y_end)
            continue;

        for(int j=0; j<5; j++)
            tex_pointer[j] = buildingDB.GetBuildingByID(w->building[i].id)->texture[1];

        tex_pointer[w->building[i].rot] = buildingDB.GetBuildingByID(w->building[i].id)->texture[0];
        tex_pointer[4] = buildingDB.GetBuildingByID(w->building[i].id)->texture[2];

        int tex_size_x = al_get_bitmap_width(buildingDB.GetBuildingByID(w->building[i].id)->texture[0]);
        int tex_size_y = al_get_bitmap_height(buildingDB.GetBuildingByID(w->building[i].id)->texture[0]);


        int xp = int((double)-ScreenW/2 + (double)w->building[i].x*(double)zoom-x_offset*(double)zoom)*16;
        int yp = int((double)-ScreenH/2 + (double)w->building[i].y*(double)zoom-y_offset*(double)zoom)*16;

        int xp2 = int((double)-ScreenW/2 + (double)(w->building[i].x+buildingDB.GetBuildingByID(w->building[i].id)->w)*(double)zoom-x_offset*(double)zoom)*16;
        int yp2 = int((double)-ScreenH/2 + (double)(w->building[i].y+buildingDB.GetBuildingByID(w->building[i].id)->h)*(double)zoom-y_offset*(double)zoom)*16;

        int xp_norm = int((double)w->building[i].x*(double)zoom-x_offset*(double)zoom);
        int yp_norm = int((double)w->building[i].y*(double)zoom-y_offset*(double)zoom);

        int xp_norm2 = int((double)(w->building[i].x+buildingDB.GetBuildingByID(w->building[i].id)->w)*(double)zoom-x_offset*(double)zoom);
        int yp_norm2 = int((double)(w->building[i].y+buildingDB.GetBuildingByID(w->building[i].id)->h)*(double)zoom-y_offset*(double)zoom);

        double b_height = buildingDB.GetBuildingByID(w->building[i].id)->height;

        hbv[0].x = (float)xp;
        hbv[0].y = (float)yp;
        hbv[0].z = -v_near*(16-min(b_height, 15.0)*(zoom/100));
        hbv[0].u = 0;
        hbv[0].v = tex_size_y;
        hbv[0].color = al_map_rgb(255,255,255);
        hbv[1].x = (float)xp2;
        hbv[1].y = (float)yp;
        hbv[1].z = -v_near*(16-min(b_height, 15.0)*(zoom/100));
        hbv[1].u = tex_size_x;
        hbv[1].v = tex_size_y;
        hbv[1].color = al_map_rgb(255,255,255);
        hbv[2].x = (float)xp2;
        hbv[2].y = (float)yp2;
        hbv[2].z = -v_near*(16.0-min(b_height, 15.0)*(zoom/100));
        hbv[2].u = tex_size_x;
        hbv[2].v = 0;
        hbv[2].color = al_map_rgb(255,255,255);
        hbv[3].x = (float)xp;
        hbv[3].y = (float)yp2;
        hbv[3].z = -v_near*(16.0-min(b_height, 15.0)*(zoom/100));
        hbv[3].u = 0;
        hbv[3].v = 0;
        hbv[3].color = al_map_rgb(255,255,255);



        hbv[4].x = (float)xp;
        hbv[4].y = (float)yp;
        hbv[4].z = -v_near*(16-min(b_height, 15.0)*(zoom/100));
        hbv[4].u = 0;
        hbv[4].v = 0;
        hbv[4].color = al_map_rgb(255,255,255);
        hbv[5].x = (float)xp2;
        hbv[5].y = (float)yp;
        hbv[5].z = -v_near*(16-min(b_height, 15.0)*(zoom/100));
        hbv[5].u = tex_size_x;
        hbv[5].v = 0;
        hbv[5].color = al_map_rgb(255,255,255);
        hbv[6].x = (float)xp2;
        hbv[6].y = (float)yp;
        hbv[6].z = -v_near*16;
        hbv[6].u = tex_size_x;
        hbv[6].v = tex_size_y;
        hbv[6].color = al_map_rgb(255,255,255);
        hbv[7].x = (float)xp;
        hbv[7].y = (float)yp;
        hbv[7].z = -v_near*16;
        hbv[7].u = 0;
        hbv[7].v = tex_size_y;
        hbv[7].color = al_map_rgb(255,255,255);

        hbv[8].x = (float)xp2;
        hbv[8].y = (float)yp;
        hbv[8].z = -v_near*(16-min(b_height, 15.0)*(zoom/100));
        hbv[8].u = 0;
        hbv[8].v = 0;
        hbv[8].color = al_map_rgb(255,255,255);
        hbv[9].x = (float)xp2;
        hbv[9].y = (float)yp2;
        hbv[9].z = -v_near*(16.0-min(b_height, 15.0)*(zoom/100));
        hbv[9].u = tex_size_x;
        hbv[9].v = 0;
        hbv[9].color = al_map_rgb(255,255,255);
        hbv[10].x = (float)xp2;
        hbv[10].y = (float)yp2;
        hbv[10].z = -v_near*16;
        hbv[10].u = tex_size_x;
        hbv[10].v = tex_size_y;
        hbv[10].color = al_map_rgb(255,255,255);
        hbv[11].x = (float)xp2;
        hbv[11].y = (float)yp;
        hbv[11].z = -v_near*16;
        hbv[11].u = 0;
        hbv[11].v = tex_size_y;
        hbv[11].color = al_map_rgb(255,255,255);

        hbv[12].x = (float)xp;
        hbv[12].y = (float)yp2;
        hbv[12].z = -v_near*(16.0-min(b_height, 15.0)*(zoom/100));
        hbv[12].u = 0;
        hbv[12].v = 0;
        hbv[12].color = al_map_rgb(255,255,255);
        hbv[13].x = (float)xp2;
        hbv[13].y = (float)yp2;
        hbv[13].z = -v_near*(16.0-min(b_height, 15.0)*(zoom/100));
        hbv[13].u = tex_size_x;
        hbv[13].v = 0;
        hbv[13].color = al_map_rgb(255,255,255);
        hbv[14].x = (float)xp2;
        hbv[14].y = (float)yp2;
        hbv[14].z = -v_near*16;
        hbv[14].u = tex_size_x;
        hbv[14].v = tex_size_y;
        hbv[14].color = al_map_rgb(255,255,255);
        hbv[15].x = (float)xp;
        hbv[15].y = (float)yp2;
        hbv[15].z = -v_near*16;
        hbv[15].u = 0;
        hbv[15].v = tex_size_y;
        hbv[15].color = al_map_rgb(255,255,255);

        hbv[16].x = (float)xp;
        hbv[16].y = (float)yp;
        hbv[16].z = -v_near*(16-min(b_height, 15.0)*(zoom/100));
        hbv[16].u = 0;
        hbv[16].v = 0;
        hbv[16].color = al_map_rgb(255,255,255);
        hbv[17].x = (float)xp;
        hbv[17].y = (float)yp2;
        hbv[17].z = -v_near*(16.0-min(b_height, 15.0)*(zoom/100));
        hbv[17].u = tex_size_x;
        hbv[17].v = 0;
        hbv[17].color = al_map_rgb(255,255,255);
        hbv[18].x = (float)xp;
        hbv[18].y = (float)yp2;
        hbv[18].z = -v_near*16;
        hbv[18].u = tex_size_x;
        hbv[18].v = tex_size_y;
        hbv[18].color = al_map_rgb(255,255,255);
        hbv[19].x = (float)xp;
        hbv[19].y = (float)yp;
        hbv[19].z = -v_near*16;
        hbv[19].u = 0;
        hbv[19].v = tex_size_y;
        hbv[19].color = al_map_rgb(255,255,255);

        if(yp_norm > ScreenH/2) al_draw_prim(hbv+4, NULL, tex_pointer[0], 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);
        if(xp_norm2 < ScreenW/2) al_draw_prim(hbv+8, NULL, tex_pointer[1], 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);
        if(yp_norm2 < ScreenH/2) al_draw_prim(hbv+12, NULL, tex_pointer[2], 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);
        if(xp_norm > ScreenW/2) al_draw_prim(hbv+16, NULL, tex_pointer[3], 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);

        al_draw_prim(hbv, NULL, tex_pointer[4], 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);
    }

    al_identity_transform(&t);
    al_orthographic_transform(&t, 0, 0, -1, ScreenW, ScreenH, 1);
    al_use_projection_transform(&t);
}

void RenderHighBlocks(world* w, int zoom_original, double x_center, double y_center, double zoom_multiplier)
{
    if(zoom_original <= 20) return;
    double v_near = 1;
    ALLEGRO_TRANSFORM t;
    al_identity_transform(&t);
    al_perspective_transform(&t, -ScreenW/2, -ScreenH/2, v_near, ScreenW/2, ScreenH/2, 10000);
    al_use_projection_transform(&t);


    int vertex_tracker = 4;

    double zoom = (double)zoom_original;
    zoom_multiplier = (zoom_multiplier-1) * (double(zoom_original)/10000.0) + 1;
    double x_offset = x_center-(double)ScreenW/(double)zoom/2.0;
    double y_offset = y_center-(double)ScreenH/(double)zoom/2.0;

    double starting_x = (double)x_center - (double)ScreenW/2.0/double(zoom);
    double starting_y = (double)y_center - (double)ScreenH/2.0/double(zoom);
    double ending_x = x_center+double(x_center-starting_x);
    double ending_y = y_center+double(y_center-starting_y);

    int x_begin = (int)max(0.0, floor(starting_x));
    int y_begin = (int)max(0.0, floor(starting_y));
    int x_end = (int)min((double)w->GetWidth(), ceil(ending_x));
    int y_end = (int)min((double)w->GetHeight(), ceil(ending_y));
    ALLEGRO_VERTEX hbv[20];
    for(int y=y_begin; y<y_end; y++)
    {
        for(int x=x_begin; x<x_end; x++)
        {
            int xp = int((double)-ScreenW/2 + (double)x*(double)zoom-x_offset*(double)zoom)*16;
            int yp = int((double)-ScreenH/2 + (double)y*(double)zoom-y_offset*(double)zoom)*16;

            int xp_norm = int((double)x*(double)zoom-x_offset*(double)zoom);
            int yp_norm = int((double)y*(double)zoom-y_offset*(double)zoom);

            double b_height = blockDB.GetBlockByID(w->GetBlock(x,y))->height;
            //
            if(w->CheckValidPos(x,y) && blockDB.GetBlockByID(w->GetBlock(x,y))->depth_effect)
            {
                ALLEGRO_BITMAP* tex_pointer = blockDB.GetBlockByID(w->GetBlock(x,y))->GetTextureFromPos(x, y);
                int rot = w->GetRotation(x,y);
                int tex_size = al_get_bitmap_width(tex_pointer);
                hbv[0].x = (float)xp;
                hbv[0].y = (float)yp;
                hbv[0].z = -v_near*(16-min(b_height, 15.0)*(zoom/100));
                hbv[0].u = 0;
                hbv[0].v = 0;
                hbv[0].color = al_map_rgb(255,255,255);
                hbv[1].x = (float)xp+zoom*16;
                hbv[1].y = (float)yp;
                hbv[1].z = -v_near*(16-min(b_height, 15.0)*(zoom/100));
                hbv[1].u = tex_size;
                hbv[1].v = 0;
                hbv[1].color = al_map_rgb(255,255,255);
                hbv[2].x = (float)xp+zoom*16;
                hbv[2].y = (float)yp+zoom*16;
                hbv[2].z = -v_near*(16.0-min(b_height, 15.0)*(zoom/100));
                hbv[2].u = tex_size;
                hbv[2].v = tex_size;
                hbv[2].color = al_map_rgb(255,255,255);
                hbv[3].x = (float)xp;
                hbv[3].y = (float)yp+zoom*16;
                hbv[3].z = -v_near*(16.0-min(b_height, 15.0)*(zoom/100));
                hbv[3].u = 0;
                hbv[3].v = tex_size;
                hbv[3].color = al_map_rgb(255,255,255);



                hbv[4].x = (float)xp;
                hbv[4].y = (float)yp;
                hbv[4].z = -v_near*(16-min(b_height, 15.0)*(zoom/100));
                hbv[4].u = tex_size;
                hbv[4].v = tex_size;
                hbv[4].color = al_map_rgb(255,255,255);
                hbv[5].x = (float)xp+zoom*16;
                hbv[5].y = (float)yp;
                hbv[5].z = -v_near*(16-min(b_height, 15.0)*(zoom/100));
                hbv[5].u = 0;
                hbv[5].v = tex_size;
                hbv[5].color = al_map_rgb(255,255,255);
                hbv[6].x = (float)xp+zoom*16;
                hbv[6].y = (float)yp;
                hbv[6].z = -v_near*16;
                hbv[6].u = 0;
                hbv[6].v = 0;
                hbv[6].color = al_map_rgb(255,255,255);
                hbv[7].x = (float)xp;
                hbv[7].y = (float)yp;
                hbv[7].z = -v_near*16;
                hbv[7].u = tex_size;
                hbv[7].v = 0;
                hbv[7].color = al_map_rgb(255,255,255);

                hbv[8].x = (float)xp+zoom*16;
                hbv[8].y = (float)yp;
                hbv[8].z = -v_near*(16-min(b_height, 15.0)*(zoom/100));
                hbv[8].u = 0;
                hbv[8].v = 0;
                hbv[8].color = al_map_rgb(255,255,255);
                hbv[9].x = (float)xp+zoom*16;
                hbv[9].y = (float)yp+zoom*16;
                hbv[9].z = -v_near*(16.0-min(b_height, 15.0)*(zoom/100));
                hbv[9].u = tex_size;
                hbv[9].v = 0;
                hbv[9].color = al_map_rgb(255,255,255);
                hbv[10].x = (float)xp+zoom*16;
                hbv[10].y = (float)yp+zoom*16;
                hbv[10].z = -v_near*16;
                hbv[10].u = tex_size;
                hbv[10].v = tex_size;
                hbv[10].color = al_map_rgb(255,255,255);
                hbv[11].x = (float)xp+zoom*16;
                hbv[11].y = (float)yp;
                hbv[11].z = -v_near*16;
                hbv[11].u = 0;
                hbv[11].v = tex_size;
                hbv[11].color = al_map_rgb(255,255,255);

                hbv[12].x = (float)xp;
                hbv[12].y = (float)yp+zoom*16;
                hbv[12].z = -v_near*(16.0-min(b_height, 15.0)*(zoom/100));
                hbv[12].u = 0;
                hbv[12].v = 0;
                hbv[12].color = al_map_rgb(255,255,255);
                hbv[13].x = (float)xp+zoom*16;
                hbv[13].y = (float)yp+zoom*16;
                hbv[13].z = -v_near*(16.0-min(b_height, 15.0)*(zoom/100));
                hbv[13].u = tex_size;
                hbv[13].v = 0;
                hbv[13].color = al_map_rgb(255,255,255);
                hbv[14].x = (float)xp+zoom*16;
                hbv[14].y = (float)yp+zoom*16;
                hbv[14].z = -v_near*16;
                hbv[14].u = tex_size;
                hbv[14].v = tex_size;
                hbv[14].color = al_map_rgb(255,255,255);
                hbv[15].x = (float)xp;
                hbv[15].y = (float)yp+zoom*16;
                hbv[15].z = -v_near*16;
                hbv[15].u = 0;
                hbv[15].v = tex_size;
                hbv[15].color = al_map_rgb(255,255,255);

                hbv[16].x = (float)xp;
                hbv[16].y = (float)yp;
                hbv[16].z = -v_near*(16-min(b_height, 15.0)*(zoom/100));
                hbv[16].u = 0;
                hbv[16].v = 0;
                hbv[16].color = al_map_rgb(255,255,255);
                hbv[17].x = (float)xp;
                hbv[17].y = (float)yp+zoom*16;
                hbv[17].z = -v_near*(16.0-min(b_height, 15.0)*(zoom/100));
                hbv[17].u = tex_size;
                hbv[17].v = 0;
                hbv[17].color = al_map_rgb(255,255,255);
                hbv[18].x = (float)xp;
                hbv[18].y = (float)yp+zoom*16;
                hbv[18].z = -v_near*16;
                hbv[18].u = tex_size;
                hbv[18].v = tex_size;
                hbv[18].color = al_map_rgb(255,255,255);
                hbv[19].x = (float)xp;
                hbv[19].y = (float)yp;
                hbv[19].z = -v_near*16;
                hbv[19].u = 0;
                hbv[19].v = tex_size;
                hbv[19].color = al_map_rgb(255,255,255);

                bool edges[4] = {0,0,0,0};
                if((w->CheckValidPos(x,y-1) && w->GetBlock(x,y-1) != w->GetBlock(x,y)) || !w->CheckValidPos(x,y-1))
                    edges[0] = true;
                if((w->CheckValidPos(x+1,y) && w->GetBlock(x+1,y) != w->GetBlock(x,y)) || !w->CheckValidPos(x+1,y))
                    edges[1] = true;
                if((w->CheckValidPos(x,y+1) && w->GetBlock(x,y+1) != w->GetBlock(x,y)) || !w->CheckValidPos(x,y+1))
                    edges[2] = true;
                if((w->CheckValidPos(x-1,y) && w->GetBlock(x-1,y) != w->GetBlock(x,y)) || !w->CheckValidPos(x-1,y))
                    edges[3] = true;

                if(edges[0] && yp_norm > ScreenH/2) al_draw_prim(hbv+4, NULL, tex_pointer, 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);
                if(edges[1] && xp_norm+zoom < ScreenW/2) al_draw_prim(hbv+8, NULL, tex_pointer, 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);
                if(edges[2] && yp_norm+zoom < ScreenH/2) al_draw_prim(hbv+12, NULL, tex_pointer, 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);
                if(edges[3] && xp_norm > ScreenW/2) al_draw_prim(hbv+16, NULL, tex_pointer, 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);

                al_draw_prim(hbv, NULL, tex_pointer, 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);

                /*if(blockDB.GetBlockByID(w->GetBlock(x,y))->edge_effect)
                {

                    ALLEGRO_BITMAP* edge_pointer = blockDB.GetBlockByID(w->GetBlock(x,y))->edge_texture_side;
                    ALLEGRO_BITMAP* corner_pointer = blockDB.GetBlockByID(w->GetBlock(x,y))->edge_texture_corner;

                    if(edges[0])
                        al_draw_scaled_rotated_bitmap(edge_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*1, 0);
                    if(edges[1])
                        al_draw_scaled_rotated_bitmap(edge_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*2, 0);
                    if(edges[2])
                        al_draw_scaled_rotated_bitmap(edge_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*3, 0);
                    if(edges[3])
                        al_draw_scaled_rotated_bitmap(edge_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*4, 0);

                    if(edges[3] && edges[0])
                        al_draw_scaled_rotated_bitmap(corner_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*0, 0);
                    if(edges[0] && edges[1])
                        al_draw_scaled_rotated_bitmap(corner_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*1, 0);
                    if(edges[1] && edges[2])
                        al_draw_scaled_rotated_bitmap(corner_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*2, 0);
                    if(edges[2] && edges[3])
                        al_draw_scaled_rotated_bitmap(corner_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*3, 0);
                }*/
            }
        }
    }
    al_identity_transform(&t);
    al_orthographic_transform(&t, 0, 0, -1, ScreenW, ScreenH, 1);
    al_use_projection_transform(&t);
}


void RenderHighBlocksOld(world* w, int zoom_original, double x_center, double y_center, double zoom_multiplier)
{
    double zoom = (double)zoom_original;
    zoom_multiplier = (zoom_multiplier-1) * (double(zoom_original)/10000.0) + 1;

    if(zoom_original <= 20) return;

    for(int i=0; i<8; i++)
    {
        zoom = pow(zoom, zoom_multiplier);
        double x_offset = x_center-(double)ScreenW/(double)zoom/2.0;
        double y_offset = y_center-(double)ScreenH/(double)zoom/2.0;

        double starting_x = (double)x_center - (double)ScreenW/2.0/double(zoom);
        double starting_y = (double)y_center - (double)ScreenH/2.0/double(zoom);
        double ending_x = x_center+double(x_center-starting_x);
        double ending_y = y_center+double(y_center-starting_y);

        int x_begin = (int)max(0.0, floor(starting_x));
        int y_begin = (int)max(0.0, floor(starting_y));
        int x_end = (int)min((double)w->GetWidth(), ceil(ending_x));
        int y_end = (int)min((double)w->GetHeight(), ceil(ending_y));


        for(int y=y_begin; y<y_end; y++)
        {
            for(int x=x_begin; x<x_end; x++)
            {
                int xp = int((double)x*(double)zoom-x_offset*(double)zoom);
                int yp = int((double)y*(double)zoom-y_offset*(double)zoom);

                //
                if(w->CheckValidPos(x,y) && blockDB.GetBlockByID(w->GetBlock(x,y))->depth_effect && blockDB.GetBlockByID(w->GetBlock(x,y))->height > i)
                {
                    ALLEGRO_BITMAP* tex_pointer = blockDB.GetBlockByID(w->GetBlock(x,y))->GetTextureFromPos(x, y);
                    int rot = w->GetRotation(x,y);
                    int tex_size = al_get_bitmap_width(tex_pointer);
                    al_draw_scaled_rotated_bitmap(tex_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*(i+1+rot), 0);
                    bool edges[4] = {0,0,0,0};
                    if(blockDB.GetBlockByID(w->GetBlock(x,y))->edge_effect)
                    {
                        if(   (w->CheckValidPos(x,y-1) && w->GetBlock(x,y-1) != w->GetBlock(x,y)) || !w->CheckValidPos(x,y-1)   )
                            edges[0] = true;
                        if(   (w->CheckValidPos(x+1,y) && w->GetBlock(x+1,y) != w->GetBlock(x,y)) || !w->CheckValidPos(x+1,y)   )
                            edges[1] = true;
                        if(   (w->CheckValidPos(x,y+1) && w->GetBlock(x,y+1) != w->GetBlock(x,y)) || !w->CheckValidPos(x,y+1)   )
                            edges[2] = true;
                        if(   (w->CheckValidPos(x-1,y) && w->GetBlock(x-1,y) != w->GetBlock(x,y)) || !w->CheckValidPos(x-1,y)   )
                            edges[3] = true;

                        ALLEGRO_BITMAP* edge_pointer = blockDB.GetBlockByID(w->GetBlock(x,y))->edge_texture_side;
                        ALLEGRO_BITMAP* corner_pointer = blockDB.GetBlockByID(w->GetBlock(x,y))->edge_texture_corner;

                        if(edges[0])
                            al_draw_scaled_rotated_bitmap(edge_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*1, 0);
                        if(edges[1])
                            al_draw_scaled_rotated_bitmap(edge_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*2, 0);
                        if(edges[2])
                            al_draw_scaled_rotated_bitmap(edge_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*3, 0);
                        if(edges[3])
                            al_draw_scaled_rotated_bitmap(edge_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*4, 0);

                        if(edges[3] && edges[0])
                            al_draw_scaled_rotated_bitmap(corner_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*0, 0);
                        if(edges[0] && edges[1])
                            al_draw_scaled_rotated_bitmap(corner_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*1, 0);
                        if(edges[1] && edges[2])
                            al_draw_scaled_rotated_bitmap(corner_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*2, 0);
                        if(edges[2] && edges[3])
                            al_draw_scaled_rotated_bitmap(corner_pointer, tex_size/2, tex_size/2, xp+zoom/2, yp+zoom/2, (float)zoom/(float)tex_size, (float)zoom/(float)tex_size, rads_90deg*3, 0);
                    }
                }
            }
        }
    }

}
