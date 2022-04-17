void FadeSequence(ALLEGRO_BITMAP* bm)
{
    double begin = al_get_time();
    
    while(al_get_time()-begin <= 0.5)
    {
        int a = int((al_get_time()-begin)*510.0);
        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_tinted_bitmap(bm,
            al_map_rgba(a,a,a,a),
            ScreenW/2-al_get_bitmap_width(bm)/2, 
            ScreenH/2-al_get_bitmap_height(bm)/2, 0);
        al_flip_display();
    }
    while(al_get_time()-begin <= 1.5)
    {
        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_bitmap(bm,
            ScreenW/2-al_get_bitmap_width(bm)/2, 
            ScreenH/2-al_get_bitmap_height(bm)/2, 0);
        al_flip_display();
    }
    while(al_get_time()-begin <= 2.0)
    {
        int a = int(1.0-(al_get_time()-begin-1.5)*510.0);
        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_tinted_bitmap(bm,
            al_map_rgba(a,a,a,a),
            ScreenW/2-al_get_bitmap_width(bm)/2, 
            ScreenH/2-al_get_bitmap_height(bm)/2, 0);
        al_flip_display();
    }
}

void IntroSequence()
{
    FadeSequence(images.GetBitmapResource("CPLLogo"));
}
