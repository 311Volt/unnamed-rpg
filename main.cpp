#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>



#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_video.h>
using namespace std;


ALLEGRO_DISPLAY *display = NULL;

#include "src/premade/misc.cpp"
#include "src/premade/medialib.cpp"
#include "src/premade/settingsreader.cpp"

#include "src/cpl/variables.cpp"
#include "src/cpl/medialoader.cpp"
#include "src/cpl/keyboard.cpp"
#include "src/cpl/entity.cpp"
#include "src/cpl/postprocess.cpp"

#include "src/cpl/database/block.cpp"
#include "src/cpl/database/building.cpp"
#include "src/cpl/database/object.cpp"

#include "src/cpl/world/class.cpp"
#include "src/cpl/world/renderer.cpp"
#include "src/cpl/world/editor.cpp"

#include "src/cpl/loop.cpp"

#include "src/cpl/introsequence.cpp"
#include "src/cpl/mainmenu.cpp"


int main(int argc, char **argv)
{
    srand(time(NULL));
    GameSettings.LoadFile("settings.cfg");
    ScreenW = GameSettings.GetValue("ResX");
    ScreenH = GameSettings.GetValue("ResY");

    if(!al_init())
        CriticalError1337("al_init() failed");

    if(!al_init_image_addon())
        CriticalError1337("al_init_image_addon() failed");
    if(!al_init_font_addon())
        CriticalError1337("al_init_font_addon() failed");
    if(!al_init_ttf_addon())
        CriticalError1337("al_init_ttf_addon() failed");
    if(!al_init_primitives_addon())
        CriticalError1337("al_init_primitives_addon() failed");
    if(!al_init_acodec_addon())
        CriticalError1337("al_init_acodec_addon() failed");
    if(!al_install_audio())
        CriticalError1337("al_install_audio() failed");
    if(!al_install_keyboard())
        CriticalError1337("al_install_keyboard() failed");
    if(!al_install_mouse())
        CriticalError1337("al_install_mouse() failed");
    if(GameSettings.GetValue("Fullscreen")) al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    display = al_create_display(ScreenW, ScreenH);

    al_get_keyboard_state(&keyb_lastFrame);
    al_get_keyboard_state(&keyb_currentFrame);
    frameDelta = 0.01;
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    mf = al_load_ttf_font("data/fonts/vcr.ttf", 48, 8);
    mf_small = al_load_ttf_font("data/fonts/vcr.ttf", 16, 8);


    InitPostprocessingEffects();
    LoadMedia(al_load_bitmap("data/loading1.png"), al_load_bitmap("data/loading2.png"),
                &images, &sounds, &textures,
                "data/lib/images.lib", "data/lib/sounds.lib", "data/lib/textures.lib",
                "data/images/", "data/sounds/", "data/textures/");
    LoadBlockDB("data/config/material_proto.cfg", "data/config/block_proto.cfg");
    buildingDB.ReadBuildingProto("data/config/building_proto.cfg");
    objectDB.ReadObjectProto("data/config/object_proto.cfg");
    IntroSequence();
    int MenuReturnValue = TempMainMenu();
    while(MenuReturnValue != 3)
    {
        if(MenuReturnValue == 1)
        {
            MainLoop();
        }
        if(MenuReturnValue == 2)
        {
            WorldEditor();
        }

        MenuReturnValue = TempMainMenu();
    }


    al_destroy_display(display);
    return 0;
}
