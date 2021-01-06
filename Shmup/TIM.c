#include "TIM.h"

Texture TIMLoad(u_long* timData)
{
    Texture texture;
    GsIMAGE image;
    
    //Skip header
    timData++;

    //Get image info    
    GsGetTimInfo(timData, &image);
    texture.mode = image.pmode & 0x3;
    texture.flags = image.pmode >> 0x2;

    //Load image data
    texture.imgVram.x = image.px;
    texture.imgVram.y = image.py;
    texture.imgVram.w = image.pw;
    texture.imgVram.h = image.ph;
    LoadImage(&texture.imgVram, image.pixel);

    //Get image tpage
    texture.imgPage = GetTPage(texture.mode, 0, texture.imgVram.x, texture.imgVram.y);
        
    //Load palette
    if(texture.mode == TIM_PMODE_4 || texture.mode == TIM_PMODE_8 || texture.mode == TIM_PMODE_MIX)
    {
        texture.palVram.x = image.cx;
        texture.palVram.y = image.cy;
        texture.palVram.w = image.cw;
        texture.palVram.h = image.ch;
        LoadImage(&texture.palVram, image.clut);
        
        //Get palette tpage
        texture.palPage = GetTPage(TIM_PMODE_16, 0, texture.palVram.x, texture.palVram.y);
    }
    
    return texture;
}
