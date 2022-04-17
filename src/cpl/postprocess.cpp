bool pp_ready = false;
ALLEGRO_BITMAP* pp_temp;
ALLEGRO_BITMAP* pp_temp2;

void InitPostprocessingEffects()
{
    pp_ready = true;
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    pp_temp = al_create_bitmap(ScreenW, ScreenH);
    pp_temp2 = al_create_bitmap(ScreenW, ScreenH);
}

void PPEffect_Vignette(int strength)
{
    al_draw_tinted_scaled_bitmap(images.GetBitmapResource("VignetteFilter"), al_map_rgba(0, 0, 0, strength), 0, 0, 640, 360, 0, 0, ScreenW, ScreenH, 0);
}
