#include "TIM.h"

void TIMLoad(u_long* timData)
{
    RECT rect;
    GsIMAGE image;
    
    //Skip header
    timData++;

    //Get image info    
    GsGetTimInfo(timData, &image);

    //Load image data
    rect.x = image.px;
    rect.y = image.py;
    rect.w = image.pw;
    rect.h = image.ph;
    LoadImage(&rect, image.pixel);
    
    //Load palette
    if(image.pmode == TIM_PMODE_4 || image.pmode == TIM_PMODE_8 || image.pmode == TIM_PMODE_MIX)
    {
        rect.x = image.cx;
        rect.y = image.cy;
        rect.w = image.cw;
        rect.h = image.ch;
        LoadImage(&rect, image.clut);
    }
}
