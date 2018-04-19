#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#define DELETE_PTR(x) if(x != nullptr)	{delete x; x = nullptr;}
#define DELETE_PTR_ARRAY(x) if(x != nullptr) {delete[] x; x = nullptr;}

//The screen scale factor
constexpr float SCREEN_SCALING = 3.0f;

//The width of the screen
constexpr int SCREEN_WIDTH = (int)SCREEN_SCALING * 144;

//The height of the screen
constexpr int SCREEN_HEIGHT = (int)SCREEN_SCALING * 256;

#endif
