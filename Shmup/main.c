#include <libps.h>
#include "Assets.h"
#include "Consts.h"
#include "Cube.h"
#include "Gamepad.h"
#include "Maths.h"
#include "Object.h"
#include "Stars.h"
#include "TIM.h"
#include "TMD.h"

//GPU buffers
GsOT DrawTables[NUM_DRAW_BUFFERS];
GsOT_TAG DrawTags[NUM_DRAW_BUFFERS][1<<ORDER_TABLE_LENGTH];
PACKET ScratchBuffers[NUM_DRAW_BUFFERS][MAX_GPU_PACKETS];

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

		//Build draw data and commands
		GsSortObject4(&Ship.gsObj, &DrawTables[backbufferIdx], 0, getScratchAddr(0));
        
        //Vsync and swap previous backbuffer
		DrawSync(0);
		frameCount = VSync(0);
		GsSwapDispBuff();

		//Push clear sceen command
		GsSortClear(0, 0, 0, &DrawTables[backbufferIdx]);

		//Send the sorted draw data to the GPU
		GsDrawOt(&DrawTables[backbufferIdx]);

        //Debug draw
		FntPrint("What's up nerds\n\n");
		FntPrint("Camera: %d,%d,%d\n", WorldView.view.t[0], WorldView.view.t[1], WorldView.view.t[2]);
		FntPrint("UP    : Camera forward\n");
		FntPrint("DOWN  : Camera back\n");
		FntPrint("LEFT  : Camera left\n");
		FntPrint("RIGHT : Camera right\n");
		FntPrint("SELECT: Quit\n");
		FntFlush(-1);
	}

    printf("Exiting\n");
	return 0;
}
