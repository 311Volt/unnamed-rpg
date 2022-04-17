entity player;
void MainLoop()
{
    ALLEGRO_BITMAP* playertex = images.GetBitmapResource("Character");
    player.SetTexture(playertex);
    player.SetSpeedLimit(500);
    while(1)
    {
        //CZYNNOŒCI INICJUJ¥CE
        frameBegin = al_get_time();
        al_get_keyboard_state(&keyb_currentFrame);
        al_get_mouse_state(&mouse_currentFrame);
        
        al_clear_to_color(al_map_rgb(140,225,240));
        
        //PRZETWARZANIE WEJŒCIA
        if(isKeyDown(ALLEGRO_KEY_LEFT)) player.SetForceX(-1);
        if(isKeyDown(ALLEGRO_KEY_RIGHT)) player.SetForceX(1);
        if(isKeyDown(ALLEGRO_KEY_UP)) player.SetForceY(-1);
        if(isKeyDown(ALLEGRO_KEY_DOWN)) player.SetForceY(1);
        
        if(isKeyPressed(ALLEGRO_KEY_V)) mode_postprocess = !mode_postprocess;
        
        if(isKeyPressed(ALLEGRO_KEY_ESCAPE))
        {
            al_get_keyboard_state(&keyb_lastFrame);
            return;
        }
        
        //FIZYKA
        player.Update(frameDelta, 30.0);
        
        //RENDER
        al_draw_bitmap(player.GetTexture(), int(player.GetX()), int(player.GetY()), 0);
        
        //FULL PR0 EFEKTY POSTPROCESOWE
        if(mode_postprocess) {PPEffect_Vignette(255);}
        //CZYNNOŒCI KOÑCZ¥CE
        al_flip_display();
        keyb_lastFrame = keyb_currentFrame;
        mouse_lastFrame = mouse_currentFrame;
        frameEnd = al_get_time();
        
        if(int(frameEnd)>int(frameBegin))
        {
            cout<<fps<<"fps\n";
            fps = 0;
        }
        else
            fps++;
        
        frameDelta = frameEnd-frameBegin;
    }
}
