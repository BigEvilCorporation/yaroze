#include <libps.h>
#include "pad.h"
#include "TMD.h"
#include "Cube.h"
#include "Stars.h"
#include "Maths.h"
#include "Consts.h"

//Object struct and funcs
typedef struct _object
{
    GsDOBJ2 gsObj;
    GsCOORDINATE2 gsTransform;
    VECTOR pos; 
    SVECTOR rot;
} Object;

static void ObjectInit(Object* object);
static void ObjectLinkTMD(Object* object, u_long* tmdData);
static void ObjectUpdateGsTransform(Object* object);

//Maths funcs
static void MatrixInit(MATRIX* mtx);
MATRIX MtxIdentity;

//Gamepad
static u_long PadRead(long id);
volatile u_char* PadData0;
volatile u_char* PadData1;

//GPU buffers
GsOT DrawDataDesc[NUM_DRAW_BUFFERS];
GsOT_TAG DrawData[NUM_DRAW_BUFFERS][1<<ORDER_TABLE_LENGTH];
PACKET ScratchBuffer[NUM_DRAW_BUFFERS][MAX_GPU_PACKETS];

//The world
GsVIEW2 WorldView;
GsF_LIGHT TestLight;
Object TestObjs[4];

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
	GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER|GsOFSGPU, 1, 0);
	GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);
	GsInit3D();
	GsSetProjection(PROJECTION_DIST);

    //Load debug font
    printf("Loading font\n");
	FntLoad(960, 256);	
	FntOpen(16, 16, 256, 200, 0, 512);
	
	//Initialise gamepads
	printf("Initialising gamepads\n");
	GetPadBuf(&PadData0, &PadData1);
	
	//Build ordering tables
	printf("Building ordering tables\n");
	for(i = 0; i < NUM_DRAW_BUFFERS; i++)
    {
		DrawDataDesc[i].length = ORDER_TABLE_LENGTH;
		DrawDataDesc[i].org = DrawData[i];
		GsClearOt(0, 0, &DrawDataDesc[i]);
	}
	
	//Setup world
	printf("Setting up world\n");
	MatrixInit(&MtxIdentity);
	MatrixInit(&WorldView.view);
	WorldView.view.t[2] = 1000;
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
	
	//Setup test objects
	printf("Loading model data\n");
	TMDLoad((u_long*)&geomCube);

	for(i = 0; i < 4; i++)
	{
        ObjectInit(&TestObjs[i]);
        ObjectLinkTMD(&TestObjs[i], (u_long*)&geomCube);
    }
    
    TestObjs[0].pos.vx = -30;
    TestObjs[0].pos.vy = -30;
    TestObjs[1].pos.vx =  30;
    TestObjs[1].pos.vy =  30;
    TestObjs[2].pos.vx = -30;
    TestObjs[2].pos.vy =  30;
    TestObjs[3].pos.vx =  30;
    TestObjs[3].pos.vy = -30;
    
    //Setup sprites
    StarfieldInit();

    printf("Starting main loop\n");
	while(running)
    {
		//Get next backbuffer index
		backbufferIdx = GsGetActiveBuff();
		
		//Set the new working buffer address
		GsSetWorkBase(ScratchBuffer[backbufferIdx]);

		//Clear draw data
		GsClearOt(0, 0, &DrawDataDesc[backbufferIdx]);
		
		//Read gamepad
		padData = PadRead(1);
		
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
        StarfieldUpdate();
		
		//Update world view
		GsSetView2(&WorldView);
		
		//Draw starfield sprites
		StarfieldDraw(&DrawDataDesc[backbufferIdx]);
        
		//Draw objects
		for(i = 0; i < 4; i++)
		{
    		TestObjs[i].rot.vx += 4*(i+1);
    		TestObjs[i].rot.vy += 8*(i+1);
    		ObjectUpdateGsTransform(&TestObjs[i]);
    		GsGetLw(&TestObjs[i].gsTransform, &tempMtx);
    		GsSetLightMatrix(&tempMtx);
    		GsGetLs(&TestObjs[i].gsTransform, &tempMtx);
    		GsSetLsMatrix(&tempMtx);
    
    		//Build draw data and commands
    		GsSortObject4(&TestObjs[i].gsObj, &DrawDataDesc[backbufferIdx], 3, getScratchAddr(0));
        }
        
        //Vsync and swap previous backbuffer
		DrawSync(0);
		frameCount = VSync(0);
		GsSwapDispBuff();

		//Push clear sceen command
		GsSortClear(0, 0, 0, &DrawDataDesc[backbufferIdx]);

		//Send the sorted draw data to the GPU
		GsDrawOt(&DrawDataDesc[backbufferIdx]);

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

static u_long PadRead(long id)
{
	return(~(*(PadData0+3) | *(PadData0+2) << 8 | *(PadData1+3) << 16 | *(PadData1+2) << 24));
}

static void ObjectInit(Object* object)
{
    //Init transform
	object->pos.vx = 0;
	object->pos.vy = 0;
	object->pos.vz = 0;
	object->rot.vx = 0;
	object->rot.vy = 0;
	object->rot.vz = 0;
    GsInitCoordinate2(WORLD, &object->gsTransform);
	object->gsObj.coord2 = &object->gsTransform;
	ObjectUpdateGsTransform(object);
}

static void ObjectLinkTMD(Object* object, u_long* tmdData)
{
	//Link pre-mapped TMD data
	tmdData += 3;    //Skip TMD header and metadata
	GsLinkObject4((u_long)tmdData, &object->gsObj, 0);
}

static void ObjectUpdateGsTransform(Object* object)
{
    //Build pos/rot matrix
    MATRIX tempMtx;   
    tempMtx.t[0] = object->pos.vx;   
    tempMtx.t[1] = object->pos.vy;   
    tempMtx.t[2] = object->pos.vz;   
    RotMatrix(&object->rot, &tempMtx); 
    object->gsTransform.coord = tempMtx;
    object->gsTransform.flg = 0;
}

static void MatrixInit(MATRIX* mtx)
{
    mtx->t[0] = 0;
    mtx->t[1] = 0;
    mtx->t[2] = 0;
    mtx->m[0][0] = ONE;
    mtx->m[0][1] = 0;
    mtx->m[0][2] = 0;
    mtx->m[1][0] = 0;
    mtx->m[1][1] = ONE;
    mtx->m[1][2] = 0;
    mtx->m[2][0] = 0;
    mtx->m[2][1] = 0;
    mtx->m[2][2] = ONE;
}
