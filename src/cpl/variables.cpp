//rozdzielczoœæ
int ScreenW;
int ScreenH;

//do obliczania czasu wykonania pojedynczej klatki
double frameBegin;
double frameEnd;
double frameDelta;

//przechowywanie stanu klawiatury z aktualnej i poprzedniej klatki
ALLEGRO_KEYBOARD_STATE keyb_lastFrame;
ALLEGRO_KEYBOARD_STATE keyb_currentFrame;
ALLEGRO_MOUSE_STATE mouse_lastFrame;
ALLEGRO_MOUSE_STATE mouse_currentFrame;

//fonty
ALLEGRO_FONT* mf;
ALLEGRO_FONT* mf_small;

//przechowywanie obrazów i dŸwiêków
medialib images;
medialib sounds;
medialib textures;

//tryby
bool mode_postprocess = true;
bool mode_debug = false;

//temp

//pozosta³e
int fps = 0;
const double rads_90deg = 1.5707963268;
ConfigFile GameSettings;
//vector<entity> entities;
