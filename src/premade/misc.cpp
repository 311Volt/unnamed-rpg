string IntToString(int input)
{
    stringstream ss;
    ss<<input;
    return ss.str();
}

void CriticalError_File404(string err, string input)
{
    al_show_native_message_box(display, "TERRIBLE HORRIBLE NO GOOD VERY BAD ERROR" ,"ERROR", string(err+input).c_str(), NULL, ALLEGRO_MESSAGEBOX_ERROR);
    exit(0);
}
void CriticalError1337(string err)
{
    al_show_native_message_box(display, "TERRIBLE HORRIBLE NO GOOD VERY BAD ERROR" ,"ERROR", err.c_str(), NULL, ALLEGRO_MESSAGEBOX_ERROR);
    exit(0);
}
void CriticalErrorNoClose(string err)
{
    al_show_native_message_box(display, "TERRIBLE HORRIBLE NO GOOD VERY BAD ERROR" ,"ERROR", err.c_str(), NULL, ALLEGRO_MESSAGEBOX_ERROR);
}
