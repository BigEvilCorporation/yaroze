#include "VRAM.h"

void VRAMLoad(u_long* data, u_long addrX, u_long addrY, u_short sizeShort)
{
    RECT rect;
    rect.x = addrX;
    rect.y = addrY;
	rect.w = sizeShort;
    rect.h = 1;
	LoadImage(&rect, data);
}
