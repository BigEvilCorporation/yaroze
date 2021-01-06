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

//GPU
GsOT DrawTables[NUM_DRAW_BUFFERS];
GsOT_TAG* DrawTags[NUM_DRAW_BUFFERS];
char* ScratchBuffers[NUM_DRAW_BUFFERS];

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
	char* data;
	int size;
	int result;
	u_long padData;
	MATRIX mtxLocal;
	MATRIX mtxWorld;
	Texture texture;
	
	//Allocate GPU buffers
	for(i = 0; i < NUM_DRAW_BUFFERS; i++)
    {
        DrawTags[i] = (GsOT_TAG*)malloc(sizeof(GsOT_TAG)*(1<<ORDER_TABLE_LENGTH));
        ScratchBuffers[i] = (char*)malloc(GPU_PACKET_SIZE*MAX_GPU_PACKETS);
    }
	
    //Initialise video
    printf("Initialising video\n");
	SetVideoMode(MODE_PAL);
	ResetGraph(0);
	GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsOFSGPU|GsNONINTER, 1, 0);
	
	//Set backbuffer positions in vram
	GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);
	
	//Init 3D coordinate system
	GsInit3D();
	GsSetProjection(PROJECTION_DIST);	

    //Load debug font
    printf("Loading font\n");
	FntLoad(DEBUG_FONT_VRAM_X, DEBUG_FONT_VRAM_Y);	
	FntOpen(-(SCREEN_WIDTH/2), -(SCREEN_HEIGHT/2)+16, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 512);
	
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
	
#if YAROZE_USE_CD
	CDPromptShow();
#endif
	
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
    texture = TIMLoad(ASSET_TIM_OVERTONE);
    printf("Texture mode %d loaded at vram %d,%d-%d,%d, page %d\n", texture.mode, texture.imgVram.x, texture.imgVram.y, texture.imgVram.w, texture.imgVram.h, texture.imgPage);
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
		
		//Update world view
		GsSetView2(&WorldView);
		
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
		
		//Draw starfield sprites
		//StarfieldDraw(&DrawDataDesc[backbufferIdx]);
        
		//Draw objects
		//Ship.rot.vx += 4*(i+1);
		Ship.rot.vy += 8*(i+1);
		ObjectUpdateGsTransform(&Ship); 
		GsGetLws(&Ship.gsTransform, &mtxWorld, &mtxLocal);
		GsSetLightMatrix(&mtxWorld);
		GsSetLsMatrix(&mtxLocal);

		//Push draw commands
		GsSortObject4(&Ship.gsObj, &DrawTables[backbufferIdx], 14-ORDER_TABLE_LENGTH, getScratchAddr(0));
        
        //Wait for previous frame draw and vsync
		DrawSync(0);
		VSync(0);
		
		//Swap back and front buffers
		GsSwapDispBuff();

		//Push clear sceen command
		GsSortClear(CLEAR_COLOUR_R, CLEAR_COLOUR_G, CLEAR_COLOUR_B, &DrawTables[backbufferIdx]);

		//Send the sorted draw data to the GPU
		GsDrawOt(&DrawTables[backbufferIdx]);
		
		//Debug draw
		FntPrint("Camera: %d,%d,%d\n", WorldView.view.t[0], WorldView.view.t[1], WorldView.view.t[2]);
		FntFlush(-1);
	}

    printf("Exiting\n");
    ResetGraph(0);
	return 0;
}
