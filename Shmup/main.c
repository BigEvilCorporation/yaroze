#include <libps.h>
#include "Assets.h"
#include "Consts.h"
#include "Cube.h"
#include "File.h"
#include "Gamepad.h"
#include "Maths.h"
#include "Object.h"
#include "Stars.h"
#include "TIM.h"
#include "TMD.h"

//GPU buffers
GsOT DrawTables[NUM_DRAW_BUFFERS];
GsOT_TAG DrawTags[NUM_DRAW_BUFFERS][1<<ORDER_TABLE_LENGTH];
char ScratchBuffers[NUM_DRAW_BUFFERS][GPU_PACKET_SIZE*MAX_GPU_PACKETS];

//The world
GsVIEW2 WorldView;
GsF_LIGHT TestLight;
Object Ship;

main()
{
    int i;
    int j;
    int running = 1;
	int	backbufferIdx = 0;
	int frameCount = 0;
	char* data;
	int size;
	int result;
	u_long padData;
	MATRIX tempMtx;
	
    //Initialise video
    printf("Initialising video\n");
	SetVideoMode(MODE_PAL);
	GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsOFSGPU, 1, 0);
	GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);
	GsInit3D();
	GsSetProjection(PROJECTION_DIST);

    //Load debug font
    printf("Loading font\n");
	FntLoad(960, 256);	
	FntOpen(16, 16, 256, 200, 0, 512);
	
	//Initialise gamepads
	printf("Initialising gamepads\n");
	GamepadInit();
	
	//Build ordering tables
	printf("Building ordering tables\n");
	for(i = 0; i < NUM_DRAW_BUFFERS; i++)
    {
		DrawTables[i].length = ORDER_TABLE_LENGTH;
		DrawTables[i].org = DrawTags[i];
		GsClearOt(0, 0, &DrawTables[i]);
	}
	
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
	
	//Setup world
	printf("Setting up world\n");
	MatrixInit(&MtxIdentity);
	MatrixInit(&WorldView.view);
	WorldView.view.t[0] = CAMERA_INITIAL_X;
	WorldView.view.t[1] = CAMERA_INITIAL_Y;
	WorldView.view.t[2] = CAMERA_INITIAL_Z;
	WorldView.super = WORLD;
	GsSetView2(&WorldView);
	
	//Setup lighting
	printf("Setting up lighting\n");
	TestLight.vx = 0;
	TestLight.vy = 0;
	TestLight.vz = ONE;
	TestLight.r = 255;
	TestLight.g = 255;
	TestLight.b = 255;
	GsSetFlatLight(0, &TestLight);
	GsSetAmbient(ONE/2, ONE/2, ONE/2);
	GsSetLightMode(0);
	
	//Setup objects
	printf("Loading model data\n");
    TMDLoad(ASSET_TMD_OVERTONE);
    TIMLoad(ASSET_TIM_OVERTONE);
    ObjectInit(&Ship);
    ObjectLinkTMD(&Ship, ASSET_TMD_OVERTONE);
    
    //Setup sprites
    //StarfieldInit();

    printf("Starting main loop\n");
	while(running)
    {
		//Get next backbuffer index
		backbufferIdx = GsGetActiveBuff();
		
		//Set the new working buffer address
		GsSetWorkBase(ScratchBuffers[backbufferIdx]);

		//Clear draw data
		GsClearOt(0, 0, &DrawTables[backbufferIdx]);
		
		//Read gamepad
		padData = GamepadRead(1);
		
		if (padData & PADselect)
		    running = 0;
	    if (padData & PADLup)
            WorldView.view.t[2] -= 1;
	    if (padData & PADLdown)
            WorldView.view.t[2] += 1;
        if (padData & PADLleft)
            WorldView.view.t[0] += 1;
	    if (padData & PADLright)
            WorldView.view.t[0] -= 1;
            
        //Run simulation
        //StarfieldUpdate();
		
		//Update world view
		GsSetView2(&WorldView);
		
		//Draw starfield sprites
		//StarfieldDraw(&DrawDataDesc[backbufferIdx]);
        
		//Draw objects
		Ship.rot.vx += 4*(i+1);
		Ship.rot.vy += 8*(i+1);
		ObjectUpdateGsTransform(&Ship); 
		GsGetLw(&Ship.gsTransform, &tempMtx);
		GsSetLightMatrix(&tempMtx);
		GsGetLs(&Ship.gsTransform, &tempMtx);
		GsSetLsMatrix(&tempMtx);

		//Push draw commands
		GsSortObject4(&Ship.gsObj, &DrawTables[backbufferIdx], 14-ORDER_TABLE_LENGTH, getScratchAddr(0));
        
        //Wait for previous frame draw and vsync
		DrawSync(0);
		frameCount = VSync(0);
		
		//Debug draw
		FntPrint("Camera: %d,%d,%d\n", WorldView.view.t[0], WorldView.view.t[1], WorldView.view.t[2]);
		FntFlush(-1);
		
		//Swap back and front buffers
		GsSwapDispBuff();

		//Push clear sceen command
		GsSortClear(CLEAR_COLOUR_R, CLEAR_COLOUR_G, CLEAR_COLOUR_B, &DrawTables[backbufferIdx]);

		//Send the sorted draw data to the GPU
		GsDrawOt(&DrawTables[backbufferIdx]);
	}

    printf("Exiting\n");
	return 0;
}
