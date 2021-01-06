#include "CDPrompt.h"
#include "Consts.h"

void CDPromptShow()
{
#if YAROZE_USE_CD
	printf("Stopping CDROM motor\n");
	CDROMStopMotor();
	printf("Stopped.\n");
	
	while(1)
	{
        backbufferIdx = GsGetActiveBuff();
		GsSetWorkBase(ScratchBuffers[backbufferIdx]);
		GsClearOt(0, 0, &DrawTables[backbufferIdx]);
		
		padData = GamepadRead(1);
		
		if (padData & PADstart)
		{
            break;
        }
        
        DrawSync(0);
		frameCount = VSync(0);
		
		//Debug draw
		FntPrint("Insert game disc\nthen press START");
		FntFlush(-1);
		
		GsSwapDispBuff();
		GsSortClear(100, 0, 0, &DrawTables[backbufferIdx]);
		GsDrawOt(&DrawTables[backbufferIdx]);
    }
    
    printf("Attempting to unlock CDROM\n");
    CDROMUnlock();
    
	size = FileGetSize("\\TEST.TXT;1");
	
	if(size > 0)
	   printf("Success!\n");
    else
       printf("Unlock failed\n");
	
	printf("TEST.TXT: %d bytes\n", size);
	
	data = (char*)malloc(size+1);
	result = FileReadSync("\\TEST.TXT;1", size, (u_long*)data);
	if(result == FILE_READ_SUCCESS)
	{
        data[size] = 0;
        printf("Read 16 bytes from file: %s\n", data);
    }
    else
    {
        printf("Failed to read data from file\n");
    }
#endif // YAROZE_USE_CD
}
