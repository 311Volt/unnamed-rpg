
void LoadMedia(ALLEGRO_BITMAP* loadimg1, ALLEGRO_BITMAP* loadimg2, medialib* img, medialib* snd, medialib* tex, string fn1, string fn2, string fn3, string wp1, string wp2, string wp3)
{
    img->LoadLib(fn1, wp1, LIB_IMAGES);
    snd->LoadLib(fn2, wp2, LIB_SOUNDS);
    tex->LoadLib(fn3, wp3, LIB_IMAGES);
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    while(1)
	{
	    al_clear_to_color(al_map_rgb(0,0,0));
	    al_draw_bitmap(loadimg1, ScreenW/2-al_get_bitmap_width(loadimg1)/2, ScreenH/2-al_get_bitmap_height(loadimg1)/2, 0);
        if(img->NextResource() == LIB_EOF)
            if(tex->NextResource() == LIB_EOF)
                if(snd->NextResource() == LIB_EOF)
                    break;
        int p = int( al_get_bitmap_width(loadimg2) * float(img->progress + tex->progress + snd->progress) / float(img->libsize + tex->libsize + snd->libsize) );
        al_draw_bitmap_region(loadimg2, 0, 0, p+(ScreenW/2-al_get_bitmap_width(loadimg2)/2), al_get_bitmap_height(loadimg2), ScreenW/2-al_get_bitmap_width(loadimg2)/2, ScreenH/2-al_get_bitmap_height(loadimg2)/2, 0);
        al_flip_display();
        //al_rest(0.2);
    }
}
