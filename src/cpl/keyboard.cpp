bool isKeyDown(int keyID)
{
    if(al_key_down(&keyb_currentFrame, keyID))
        return true;
    return false;
}
bool wasKeyDown(int keyID)
{
    if(al_key_down(&keyb_lastFrame, keyID))
        return true;
    return false;
}
bool isKeyPressed(int keyID)
{
    if(al_key_down(&keyb_lastFrame, keyID) == 0 && al_key_down(&keyb_currentFrame, keyID))
        return true;
    return false;
}
bool isKeyReleased(int keyID)
{
    if(al_key_down(&keyb_lastFrame, keyID) && al_key_down(&keyb_currentFrame, keyID) == 0)
        return true;
    return false;
}
bool isKeyStillDown(int keyID)
{
    if(al_key_down(&keyb_lastFrame, keyID) && al_key_down(&keyb_currentFrame, keyID))
        return true;
    return false;
}
bool isKeyStillUp(int keyID)
{
    if(al_key_down(&keyb_lastFrame, keyID) == 0 && al_key_down(&keyb_currentFrame, keyID) == 0)
        return true;
    return false;
}



//////////////////////////////////////////////////////////////////////////////////////////////





bool isLMBDown()
{
    if(al_mouse_button_down(&mouse_currentFrame, 1))
        return true;
    return false;
}
bool isLMBUp()
{
    if(al_mouse_button_down(&mouse_currentFrame, 1))
        return true;
    return false;
}
bool isLMBPressed()
{
    if(al_mouse_button_down(&mouse_currentFrame, 1) > al_mouse_button_down(&mouse_lastFrame, 1))
        return true;
    return false;
}
bool isLMBReleased()
{
    if(al_mouse_button_down(&mouse_currentFrame, 1) < al_mouse_button_down(&mouse_lastFrame, 1))
        return true;
    return false;
}





bool isRMBDown()
{
    if(al_mouse_button_down(&mouse_currentFrame, 2))
        return true;
    return false;
}
bool isRMBUp()
{
    if(al_mouse_button_down(&mouse_currentFrame, 2))
        return true;
    return false;
}
bool isRMBPressed()
{
    if(al_mouse_button_down(&mouse_currentFrame, 2) > al_mouse_button_down(&mouse_lastFrame, 2))
        return true;
    return false;
}
bool isRMBReleased()
{
    if(al_mouse_button_down(&mouse_currentFrame, 2) < al_mouse_button_down(&mouse_lastFrame, 2))
        return true;
    return false;
}





bool isMMBDown()
{
    if(al_mouse_button_down(&mouse_currentFrame, 3))
        return true;
    return false;
}
bool isMMBUp()
{
    if(al_mouse_button_down(&mouse_currentFrame, 3))
        return true;
    return false;
}
bool isMMBPressed()
{
    if(al_mouse_button_down(&mouse_currentFrame, 3) > al_mouse_button_down(&mouse_lastFrame, 3))
        return true;
    return false;
}
bool isMMBReleased()
{
    if(al_mouse_button_down(&mouse_currentFrame, 3) < al_mouse_button_down(&mouse_lastFrame, 3))
        return true;
    return false;
}


int GetMouseX()
{
    return mouse_currentFrame.x;
}
int GetMouseY()
{
    return mouse_currentFrame.y;
}

int GetMouseXMovement()
{
    return mouse_currentFrame.x-mouse_lastFrame.x;
}
int GetMouseYMovement()
{
    return mouse_currentFrame.y-mouse_lastFrame.y;
}

bool IsMouseInRectangle(int x1, int y1, int x2, int y2)
{
    if(GetMouseX() >= x1 && GetMouseX() <= x2 && GetMouseY() >= y1 && GetMouseY() <= y2)
        return true;
    return false;
}
