#define EDITING_MODE_BLOCK 0
#define EDITING_MODE_BUILDING 1
#define EDITING_MODE_OBJECT 2
#define EDITING_MODE_DELETE 3


int BlockSelectionTool(double* fbegin);
int BuildingSelectionTool(double* fbegin);
int ObjectSelectionTool(double* fbegin);
int WorldResizeTool(double* fbegin, world* w);


void WorldEditor()
{
    world* city = CreateEmptyWorld(250, 250, 0);
    int EditingMode = 0;
    double xp = 0;
    double yp = 0;
    double starting_x;
    double starting_y;
    double ending_x;
    double ending_y;
    int MouseHoverX;
    int MouseHoverY;
    int MouseHoverXPixel;
    int MouseHoverYPixel;
    int TargetID = 1;
    int TargetRotation = 0;
    int GUISelection = 0;
    int tex_size;
    bool NoTargetBlockChange = 1;

    double zoom = 64;
    double speed = 5;
    int fps_last = fps;
    while(1)
    {
        //CZYNNOŒCI INICJUJACE
        frameBegin = al_get_time();
        al_get_keyboard_state(&keyb_currentFrame);
        al_get_mouse_state(&mouse_currentFrame);
        al_clear_to_color(al_map_rgb(0,0,0));
        if(isLMBPressed()) NoTargetBlockChange = 1;
        //OBLICZENIA
        starting_x = (double)xp - (double)ScreenW/2.0/floor(zoom);
        starting_y = (double)yp - (double)ScreenH/2.0/floor(zoom);
        ending_x = xp+(xp-starting_x);
        ending_y = yp+(yp-starting_y);
        MouseHoverX = int(starting_x+(double)GetMouseX()/floor(zoom));
        MouseHoverY = int(starting_y+(double)GetMouseY()/floor(zoom));
        MouseHoverXPixel = MouseHoverX*int(zoom) - int(starting_x*floor(zoom));
        MouseHoverYPixel = MouseHoverY*int(zoom) - int(starting_y*floor(zoom));
        tex_size = al_get_bitmap_width(blockDB.GetBlockByID(TargetID)->texture[0]);
        GUISelection = 0;

        //PRZETWARZANIE WEJSCIA
        if(isKeyDown(ALLEGRO_KEY_LEFT)) xp -= frameDelta*speed;
        if(isKeyDown(ALLEGRO_KEY_RIGHT)) xp += frameDelta*speed;
        if(isKeyDown(ALLEGRO_KEY_UP)) yp -= frameDelta*speed;
        if(isKeyDown(ALLEGRO_KEY_DOWN)) yp += frameDelta*speed;

        if(isKeyPressed(ALLEGRO_KEY_V)) mode_postprocess = !mode_postprocess;

        if(isKeyPressed(ALLEGRO_KEY_A)) zoom *= 1.25;
        if(isKeyPressed(ALLEGRO_KEY_Z)) zoom = max(1.0, zoom/1.25);

        if(isKeyPressed(ALLEGRO_KEY_ENTER)) TargetRotation = (TargetRotation+1)%4;

        if(isRMBDown())
        {
            xp -= GetMouseXMovement()/floor(zoom);
            yp -= GetMouseYMovement()/floor(zoom);
        }
        if(IsMouseInRectangle(20, 20, 220, 60))
            GUISelection = 1;
        if(IsMouseInRectangle(20, 80, 220, 120))
            GUISelection = 2;
        if(IsMouseInRectangle(20, 140, 220, 180))
            GUISelection = 3;
        if(IsMouseInRectangle(20, 200, 60, 240))
            GUISelection = 4;
        if(IsMouseInRectangle(80, 200, 120, 248))
            GUISelection = 5;
        if(IsMouseInRectangle(140, 200, 204, 248))
            GUISelection = 6;
        if(IsMouseInRectangle(200, 200, 264, 248))
            GUISelection = 7;


        if(isLMBDown() && NoTargetBlockChange && GUISelection == 0)
        {
                if(EditingMode == EDITING_MODE_BLOCK)
                    city->SetBlock(MouseHoverX, MouseHoverY, TargetRotation, TargetID);
                if(EditingMode == EDITING_MODE_BUILDING && city->SafeToPlaceBuilding(MouseHoverX, MouseHoverY, TargetID))
                    city->InsertBuilding(MouseHoverX, MouseHoverY, TargetRotation, TargetID);
                if(EditingMode == EDITING_MODE_OBJECT && city->SafeToPlaceObject(MouseHoverX, MouseHoverY, TargetID))
                    city->InsertObject(MouseHoverX, MouseHoverY, TargetRotation, TargetID);
                if(EditingMode == EDITING_MODE_DELETE)
                {
                    city->DeleteBuildingAtPosition(MouseHoverX, MouseHoverY);
                    city->DeleteObjectAtPosition(MouseHoverX, MouseHoverY);
                }
        }
        else
        {
            if(mouse_currentFrame.z > 0)
                zoom *= pow(1.125, mouse_currentFrame.z);
            if(mouse_currentFrame.z < 0  && !(((float)ScreenW/floor(zoom/pow(1.125, -mouse_currentFrame.z)) > city->GetWidth())||((float)ScreenH/floor(zoom/pow(1.125, -mouse_currentFrame.z)) > city->GetHeight())))
                zoom = max(1.0, zoom/pow(1.125, -mouse_currentFrame.z));
        }


        if(isKeyPressed(ALLEGRO_KEY_S)) speed *= 1.25;
        if(isKeyPressed(ALLEGRO_KEY_X)) speed /= 1.25;



        if(isKeyPressed(ALLEGRO_KEY_ESCAPE))
        {
            al_get_keyboard_state(&keyb_lastFrame);
            return;
        }


        if(ScreenW/zoom < city->GetWidth() && ScreenH/zoom < city->GetHeight())
        {
            if(starting_x <= 1)
            {
                xp = max(xp, (double)ScreenW/2.0/floor(zoom));
            }
            if(starting_y <= 1)
            {
                yp = max(yp, (double)ScreenH/2.0/floor(zoom));
            }
            if(ending_x >= city->GetWidth()-1)
            {
                xp = min(city->GetWidth()-(double)ScreenW/2.0/floor(zoom), xp);
            }
            if(ending_y >= city->GetHeight()-1)
            {
                yp = min(city->GetHeight()-(double)ScreenH/2.0/floor(zoom), yp);
            }
        }
        starting_x = (double)xp - (double)ScreenW/2.0/floor(zoom);
        starting_y = (double)yp - (double)ScreenH/2.0/floor(zoom);
        ending_x = xp+(xp-starting_x);
        ending_y = yp+(yp-starting_y);


        //RENDER
        RenderWorld(city, int(zoom), xp, yp);

        if(GUISelection>0 && isLMBPressed())
        {
            switch(GUISelection)
            {
                case 1: TargetID = BlockSelectionTool(&frameBegin); EditingMode = 0; break;
                case 2: TargetID = BuildingSelectionTool(&frameBegin); EditingMode = 1; break;
                case 3: TargetID = ObjectSelectionTool(&frameBegin); EditingMode = 2; break;
                case 4: EditingMode = 3; break;
                case 5: WorldResizeTool(&frameBegin, city); break;
                case 6:
                    al_draw_text(mf, al_map_rgb(255,255,255), ScreenW/2, ScreenH/2, ALLEGRO_ALIGN_CENTER, "Zapisywanie...");
                    al_flip_display();
                    al_rest(0.2);
                    city->SaveToFile("saves/default.sav");
                    break;
                case 7:
                    al_draw_text(mf, al_map_rgb(255,255,255), ScreenW/2, ScreenH/2, ALLEGRO_ALIGN_CENTER, "Wczytywanie...");
                    al_flip_display();
                    al_rest(0.2);
                    delete city;
                    city = CreateWorldFromFile("saves/default.sav");
                    break;
            }
            NoTargetBlockChange = false;
        }

        //debug info
        al_draw_textf(mf_small, al_map_rgb(255,255,255), ScreenW-20, 30, ALLEGRO_ALIGN_RIGHT, "%d fps", fps_last);
        al_draw_textf(mf_small, al_map_rgb(255,255,255), ScreenW-20, 50, ALLEGRO_ALIGN_RIGHT, "X - %.3f (%.3f/%.3f), myszka - %d", float(xp), float(starting_x), float(ending_x), MouseHoverX);
        al_draw_textf(mf_small, al_map_rgb(255,255,255), ScreenW-20, 70, ALLEGRO_ALIGN_RIGHT, "Y - %.3f (%.3f/%.3f), myszka - %d", float(yp), float(starting_y), float(ending_y), MouseHoverY);
        al_draw_textf(mf_small, al_map_rgb(255,255,255), ScreenW-20, 90, ALLEGRO_ALIGN_RIGHT, "Predkosc: %d blokow na sekunde (S/X by zmienic)", int(speed));
        al_draw_textf(mf_small, al_map_rgb(255,255,255), ScreenW-20, 110, ALLEGRO_ALIGN_RIGHT, "Zoom: %d px/blok, zmiana A/Z lub scroll", int(zoom));

        //podglad
        if(EditingMode == EDITING_MODE_BLOCK)
        {
            al_draw_tinted_scaled_rotated_bitmap(blockDB.GetBlockByID(TargetID)->texture[0], al_map_rgba(100,100,100,100),
                tex_size/2, tex_size/2, MouseHoverXPixel+floor(zoom)/2, MouseHoverYPixel+floor(zoom)/2,
                floor(zoom)/(float)tex_size, floor(zoom)/(float)tex_size, rads_90deg*TargetRotation*!blockDB.GetBlockByID(TargetID)->random_rotation, 0);
            if(!blockDB.GetBlockByID(TargetID)->random_rotation) al_draw_scaled_rotated_bitmap(images.GetBitmapResource("YellowArrow"),
                tex_size/2, tex_size/2, MouseHoverXPixel+floor(zoom)/2, MouseHoverYPixel+floor(zoom)/2,
                floor(zoom)/(float)tex_size, floor(zoom)/(float)tex_size, rads_90deg*TargetRotation, 0);
        }
        if(EditingMode == EDITING_MODE_BUILDING)
        {
            al_draw_filled_rectangle(MouseHoverXPixel, MouseHoverYPixel, MouseHoverXPixel+buildingDB.GetBuildingByID(TargetID)->w*floor(zoom),
                                     MouseHoverYPixel+buildingDB.GetBuildingByID(TargetID)->h*floor(zoom), al_map_rgba(127,127,127,127));
        }
        if(EditingMode == EDITING_MODE_OBJECT)
        {
            al_draw_filled_rectangle(MouseHoverXPixel, MouseHoverYPixel, MouseHoverXPixel+objectDB.GetObjectByID(TargetID)->w*floor(zoom),
                                     MouseHoverYPixel+objectDB.GetObjectByID(TargetID)->h*floor(zoom), al_map_rgba(127,127,127,127));
        }
        if(EditingMode == EDITING_MODE_DELETE)
        {
            al_draw_bitmap(images.GetBitmapResource("GUI_EditorDelete"), GetMouseX()-20, GetMouseY()-20, 0);
        }

        //GUI
        for(int i=0; i<bool(GUISelection==1)+1; i++) al_draw_bitmap(images.GetBitmapResource("GUI_EditorBlock"), 20, 20, 0);
        for(int i=0; i<bool(GUISelection==2)+1; i++) al_draw_bitmap(images.GetBitmapResource("GUI_EditorBuilding"), 20, 80, 0);
        for(int i=0; i<bool(GUISelection==3)+1; i++) al_draw_bitmap(images.GetBitmapResource("GUI_EditorObject"), 20, 140, 0);
        for(int i=0; i<bool(GUISelection==4)+1; i++) al_draw_bitmap(images.GetBitmapResource("GUI_EditorDelete"), 20, 200, 0);
        for(int i=0; i<bool(GUISelection==5)+1; i++) al_draw_bitmap(images.GetBitmapResource("GUI_EditorResize"), 80, 200, 0);
        for(int i=0; i<bool(GUISelection==6)+1; i++) al_draw_tinted_bitmap(images.GetBitmapResource("GUI_EditorSave"), al_map_rgba(127,127,127,127), 140, 200, 0);
        for(int i=0; i<bool(GUISelection==7)+1; i++) al_draw_tinted_bitmap(images.GetBitmapResource("GUI_EditorLoad"), al_map_rgba(127,127,127,127), 200, 200, 0);



        al_draw_filled_rectangle(ScreenW-city->GetWidth(), ScreenH-city->GetHeight(), ScreenW, ScreenH, al_map_rgba(50,50,50,100));
        al_draw_rectangle(ScreenW-city->GetWidth()+starting_x, ScreenH-city->GetHeight()+starting_y, ScreenW-city->GetWidth()+ending_x, ScreenH-city->GetHeight()+ending_y, al_map_rgba(255,255,255,255), 1);
        //FULL PR0 EFEKTY POSTPROCESOWE
        if(mode_postprocess) {PPEffect_Vignette(255);}


        //CZYNNOSCI KONCZACE
        al_flip_display();
        keyb_lastFrame = keyb_currentFrame;
        mouse_lastFrame = mouse_currentFrame;
        frameEnd = al_get_time();
        if(int(frameEnd)>int(frameBegin))
        {
            fps_last = fps;
            cout<<fps<<"fps\n";
            fps = 0;
        }
        else
            fps++;
        if(mouse_currentFrame.z) al_set_mouse_z(0);
        frameDelta = frameEnd-frameBegin;
    }
}


int BlockSelectionTool(double* fbegin)
{
    double FuncBegin = al_get_time();
    ALLEGRO_BITMAP* bg = al_create_bitmap(ScreenW, ScreenH);
    al_set_target_bitmap(bg);
    al_draw_tinted_bitmap(al_get_backbuffer(display), al_map_rgba(127,127,127,127), 0, 0, 0);
    al_set_target_bitmap(al_get_backbuffer(display));
    int mouse_hover = -1;
    while(1)
    {
        //CZYNNOŒCI INICJUJ¥CE
        frameBegin = al_get_time();
        al_get_keyboard_state(&keyb_currentFrame);
        al_get_mouse_state(&mouse_currentFrame);
        al_clear_to_color(al_map_rgb(0,0,0));
        mouse_hover = -1;
        //PRZETWARZANIE WEJŒCIA
        if(isKeyPressed(ALLEGRO_KEY_V)) mode_postprocess = !mode_postprocess;
        al_draw_bitmap(bg, 0, 0, 0);
        al_draw_textf(mf, al_map_rgb(255,255,255), ScreenW/2, 10, ALLEGRO_ALIGN_CENTRE, "Wybierz blok");
        for(int i=0; i<blockDB.blockProto.size(); i++)
        {
            int tex_size = al_get_bitmap_width(blockDB.blockProto[i].texture[0]);
            int x_begin = 70*(i % (ScreenW/70)) + (ScreenW-70*(ScreenW/70));
            int y_begin = 60 + (i/ScreenW/70)*70;

            if(GetMouseX() >= x_begin && GetMouseX() <= x_begin+64 && GetMouseY() >= y_begin && GetMouseY() <= y_begin+64)
            {
                mouse_hover = i;
                al_draw_scaled_bitmap(blockDB.blockProto[i].texture[0], 0, 0, tex_size, tex_size, x_begin, y_begin, 64, 64, 0);
            }
            else
                al_draw_tinted_scaled_bitmap(blockDB.blockProto[i].texture[0], al_map_rgba_f(0.5,0.5,0.5,0.5), 0, 0, tex_size, tex_size, x_begin, y_begin, 64, 64, 0);
        }
        if(mouse_hover >= 0)
        {
            al_draw_textf(mf_small, al_map_rgb(0,0,0), GetMouseX()+17, GetMouseY()+2, ALLEGRO_ALIGN_LEFT, blockDB.blockProto[mouse_hover].name.c_str());
            al_draw_textf(mf_small, al_map_rgb(255,255,255), GetMouseX()+15, GetMouseY(), ALLEGRO_ALIGN_LEFT, blockDB.blockProto[mouse_hover].name.c_str());

            if(isLMBPressed())
            {
                *fbegin += al_get_time()-FuncBegin;
                return mouse_hover;
            }
        }
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

    al_destroy_bitmap(bg);
}

int BuildingSelectionTool(double* fbegin)
{
    double FuncBegin = al_get_time();
    ALLEGRO_BITMAP* bg = al_create_bitmap(ScreenW, ScreenH);
    al_set_target_bitmap(bg);
    al_draw_tinted_bitmap(al_get_backbuffer(display), al_map_rgba(127,127,127,127), 0, 0, 0);
    al_set_target_bitmap(al_get_backbuffer(display));
    int mouse_hover = -1;
    while(1)
    {
        //CZYNNOŒCI INICJUJ¥CE
        frameBegin = al_get_time();
        al_get_keyboard_state(&keyb_currentFrame);
        al_get_mouse_state(&mouse_currentFrame);
        al_clear_to_color(al_map_rgb(0,0,0));
        mouse_hover = -1;
        //PRZETWARZANIE WEJŒCIA
        if(isKeyPressed(ALLEGRO_KEY_V)) mode_postprocess = !mode_postprocess;
        al_draw_bitmap(bg, 0, 0, 0);
        al_draw_textf(mf, al_map_rgb(255,255,255), ScreenW/2, 10, ALLEGRO_ALIGN_CENTRE, "Wybierz budynek");
        for(int i=0; i<buildingDB.buildingProto.size(); i++)
        {
            int tex_size_x = al_get_bitmap_width(buildingDB.buildingProto[i].texture[0]);
            int tex_size_y = al_get_bitmap_height(buildingDB.buildingProto[i].texture[0]);

            int x_begin = 70*(i % (ScreenW/70)) + (ScreenW-70*(ScreenW/70));
            int y_begin = 60 + (i/ScreenW/70)*70;

            if(GetMouseX() >= x_begin && GetMouseX() <= x_begin+64 && GetMouseY() >= y_begin && GetMouseY() <= y_begin+64)
            {
                mouse_hover = i;
                al_draw_scaled_bitmap(buildingDB.buildingProto[i].texture[0], 0, 0, tex_size_x, tex_size_y, x_begin, y_begin, 64, 64, 0);
            }
            else
                al_draw_tinted_scaled_bitmap(buildingDB.buildingProto[i].texture[0], al_map_rgba_f(0.5,0.5,0.5,0.5), 0, 0, tex_size_x, tex_size_y, x_begin, y_begin, 64, 64, 0);
        }
        if(mouse_hover >= 0)
        {
            al_draw_textf(mf_small, al_map_rgb(0,0,0), GetMouseX()+17, GetMouseY()+2, ALLEGRO_ALIGN_LEFT, buildingDB.buildingProto[mouse_hover].name.c_str());
            al_draw_textf(mf_small, al_map_rgb(255,255,255), GetMouseX()+15, GetMouseY(), ALLEGRO_ALIGN_LEFT, buildingDB.buildingProto[mouse_hover].name.c_str());
            if(isLMBPressed())
            {
                *fbegin += al_get_time()-FuncBegin;
                return mouse_hover;
            }
        }
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

    al_destroy_bitmap(bg);
}

int ObjectSelectionTool(double* fbegin)
{
    double FuncBegin = al_get_time();
    ALLEGRO_BITMAP* bg = al_create_bitmap(ScreenW, ScreenH);
    al_set_target_bitmap(bg);
    al_draw_tinted_bitmap(al_get_backbuffer(display), al_map_rgba(127,127,127,127), 0, 0, 0);
    al_set_target_bitmap(al_get_backbuffer(display));
    int mouse_hover = -1;
    while(1)
    {
        //CZYNNOŒCI INICJUJ¥CE
        frameBegin = al_get_time();
        al_get_keyboard_state(&keyb_currentFrame);
        al_get_mouse_state(&mouse_currentFrame);
        al_clear_to_color(al_map_rgb(0,0,0));
        mouse_hover = -1;
        //PRZETWARZANIE WEJŒCIA
        if(isKeyPressed(ALLEGRO_KEY_V)) mode_postprocess = !mode_postprocess;
        al_draw_bitmap(bg, 0, 0, 0);
        al_draw_textf(mf, al_map_rgb(255,255,255), ScreenW/2, 10, ALLEGRO_ALIGN_CENTRE, "Wybierz obiekt");
        for(int i=0; i<objectDB.objectProto.size(); i++)
        {
            int tex_size_x = al_get_bitmap_width(objectDB.objectProto[i].texture);
            int tex_size_y = al_get_bitmap_height(objectDB.objectProto[i].texture);

            int x_begin = 70*(i % (ScreenW/70)) + (ScreenW-70*(ScreenW/70));
            int y_begin = 60 + (i/ScreenW/70)*70;

            if(GetMouseX() >= x_begin && GetMouseX() <= x_begin+64 && GetMouseY() >= y_begin && GetMouseY() <= y_begin+64)
            {
                mouse_hover = i;
                al_draw_scaled_bitmap(objectDB.objectProto[i].texture, 0, 0, tex_size_x, tex_size_y, x_begin, y_begin, 64, 64, 0);
            }
            else
                al_draw_tinted_scaled_bitmap(objectDB.objectProto[i].texture, al_map_rgba_f(0.5,0.5,0.5,0.5), 0, 0, tex_size_x, tex_size_y, x_begin, y_begin, 64, 64, 0);
        }
        if(mouse_hover >= 0)
        {
            al_draw_textf(mf_small, al_map_rgb(0,0,0), GetMouseX()+17, GetMouseY()+2, ALLEGRO_ALIGN_LEFT, objectDB.objectProto[mouse_hover].name.c_str());
            al_draw_textf(mf_small, al_map_rgb(255,255,255), GetMouseX()+15, GetMouseY(), ALLEGRO_ALIGN_LEFT, objectDB.objectProto[mouse_hover].name.c_str());
            if(isLMBPressed())
            {
                *fbegin += al_get_time()-FuncBegin;
                return mouse_hover;
            }
        }
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

    al_destroy_bitmap(bg);
}
int WorldResizeTool(double* fbegin, world* w)
{
    double FuncBegin = al_get_time();
    ALLEGRO_BITMAP* bg = al_create_bitmap(ScreenW, ScreenH);
    al_set_target_bitmap(bg);
    al_draw_tinted_bitmap(al_get_backbuffer(display), al_map_rgba(127,127,127,127), 0, 0, 0);
    al_set_target_bitmap(al_get_backbuffer(display));
    int mouse_hover = -1;
    int msize = w->GetWidth();
    while(1)
    {
        //CZYNNOŒCI INICJUJ¥CE
        frameBegin = al_get_time();
        al_get_keyboard_state(&keyb_currentFrame);
        al_get_mouse_state(&mouse_currentFrame);
        al_clear_to_color(al_map_rgb(0,0,0));
        mouse_hover = -1;
        //PRZETWARZANIE WEJŒCIA
        msize = max(10, msize+mouse_currentFrame.z*10);
        msize = min(msize, 500);
        if(isKeyPressed(ALLEGRO_KEY_V)) mode_postprocess = !mode_postprocess;
        al_draw_bitmap(bg, 0, 0, 0);
        al_draw_textf(mf, al_map_rgb(255,255,255), ScreenW/2, 10, ALLEGRO_ALIGN_CENTRE, "Zmien rozmiar mapy (scroll)");
        al_draw_textf(mf_small, al_map_rgb(255,255,255), ScreenW/2, 50, ALLEGRO_ALIGN_CENTRE, "ENTER by potwierdzic");


        al_draw_textf(mf, al_map_rgb(255,255,255), ScreenW/2, ScreenH/2, ALLEGRO_ALIGN_CENTRE, "%d x %d", msize, msize);

        if(isKeyPressed(ALLEGRO_KEY_ENTER))
        {
            w->Resize(msize, msize);
            return 0;
        }


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
        if(mouse_currentFrame.z) al_set_mouse_z(0);
        frameDelta = frameEnd-frameBegin;
    }

    al_destroy_bitmap(bg);
}
