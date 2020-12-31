#include "Gamepad.h"

volatile u_char* PadData0;
volatile u_char* PadData1;

void GamepadInit()
{
    GetPadBuf(&PadData0, &PadData1);
}

u_long GamepadRead(long id)
{
	return(~(*(PadData0+3) | *(PadData0+2) << 8 | *(PadData1+3) << 16 | *(PadData1+2) << 24));
}
