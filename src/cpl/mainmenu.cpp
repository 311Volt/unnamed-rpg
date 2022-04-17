int TempMainMenu() //full prowizorka
{
    //ALLEGRO_VIDEO* mm = al_open_video("gimber.ogv");
    //if(!mm) CriticalError1337("ASDSDFAFSDASDFSDFSDF");
    //al_start_video(mm, al_get_default_mixer());
    while(1)
    {
        al_get_keyboard_state(&keyb_currentFrame);
        al_clear_to_color(al_map_rgb(0,0,0));
        //if(al_get_video_frame(mm)) al_draw_bitmap(al_get_video_frame(mm), 0, 0, 0);
        al_draw_textf(mf, al_map_rgb(255,255,255), ScreenW/2, 50, ALLEGRO_ALIGN_CENTRE, "*FULL PRO MAIN MENU*");
        al_draw_textf(mf, al_map_rgb(255,255,255), ScreenW/2, 150, ALLEGRO_ALIGN_CENTRE, "1 - gra");
        al_draw_textf(mf, al_map_rgb(255,255,255), ScreenW/2, 200, ALLEGRO_ALIGN_CENTRE, "2 - edytor map");
        al_draw_textf(mf, al_map_rgb(255,255,255), ScreenW/2, 250, ALLEGRO_ALIGN_CENTRE, "3/ESC - wyjscie");
        
        
        if(isKeyDown(ALLEGRO_KEY_1))
        {
            return 1;
        }
        if(isKeyDown(ALLEGRO_KEY_2))
        {
            return 2;
        }
        if(isKeyDown(ALLEGRO_KEY_3) || isKeyPressed(ALLEGRO_KEY_ESCAPE))
        {
            return 3;
        }
        al_get_keyboard_state(&keyb_lastFrame);
        al_flip_display();
    }
}
