#include <libps.h>

#include "Stars.h"
#include "Maths.h"
#include "Consts.h"
#include "VRAM.h"

typedef struct _starDef
{
    int imgWidth;
    int imgHeight;
    int speedMin;
    int speedMax;
    u_short* imgData;
    u_short texPage;
} StarDef;

typedef struct _star
{
    StarDef* starDef;
    GsSPRITE sprite;
    u_short velY;
} Star;

Star starfield[STAR_COUNT];

u_short star1x1[4] =
{
    0x0000,
    0x0100,
    0x0000,
    0x0000,
};

u_short star2x2[4] =
{
    0x0000,
    0x0110,
    0x0110,
    0x0000,
};

u_short star3x3[4] =
{
    0x1110,
    0x1110,
    0x1110,
    0x0000,
};

u_short star4x4[4] =
{
    0x1111,
    0x1111,
    0x1111,
    0x1111,
};

StarDef starDefs[STAR_NUM_DEFS] =
{
    {
        4, 4,    // Img size
        1, 4,    // Speed min/max
        star1x1, // Img data
        0        // Tex page
    },
    {
        4, 4,    // Img size
        5, 8,    // Speed min/max
        star2x2, // Img data
        0        // Tex page
    },
    {
        4, 4,    // Img size
        9,12,    // Speed min/max
        star3x3, // Img data
        0        // Tex page
    },
    {
        4, 4,    // Img size
        13,20,   // Speed min/max
        star4x4, // Img data
        0        // Tex page
    },
};

u_short starPalette[STAR_PALETTE_SIZE] =
{
    //15-bit colour
    0x0000, // Black
	0xFFFF, // White
};

void StarfieldStarInit(Star* star)
{
    int starDefIdx = rand() % STAR_NUM_DEFS;
    StarDef* starDef = &starDefs[starDefIdx];
    
    star->starDef = starDef;
    star->sprite.attribute = 0;
	star->sprite.x = -(SCREEN_WIDTH/2) + (rand() % SCREEN_WIDTH);
	star->sprite.y = -(SCREEN_HEIGHT/2) + (rand() % SCREEN_HEIGHT);
	star->sprite.w = starDef->imgWidth;
	star->sprite.h = starDef->imgHeight;
	star->sprite.tpage = starDef->texPage;
	star->sprite.u = 0;
	star->sprite.v = 0;
	star->sprite.cx = STAR_PALETTE_X;
	star->sprite.cy = STAR_PALETTE_Y;
	star->sprite.r = 128;
    star->sprite.g = 128;
    star->sprite.b = 128;
	star->sprite.mx = 0;
	star->sprite.my = 0;
	star->sprite.scalex = ONE;
	star->sprite.scaley = ONE;
	star->sprite.rotate = rand();
	star->velY = RandRanged(starDef->speedMin, starDef->speedMax);
}

void StarfieldInit()
{
    int i;
	RECT spriteRect;
	
	//Load sprites into VRAM
	for(i = 0; i < STAR_NUM_DEFS; i++)
	{
        spriteRect.x = STAR_VRAM_X + (TPAGE_WIDTH * i);
        spriteRect.y = STAR_VRAM_Y;
    	spriteRect.w = starDefs[i].imgWidth/4;
        spriteRect.h = starDefs[i].imgHeight;
    	LoadImage(&spriteRect, (u_long*)starDefs[i].imgData);
    	starDefs[i].texPage = GetTPage(0, 0, spriteRect.x, spriteRect.y);
    }
	
	//Load palette into VRAM
	VRAMLoad((u_long*)starPalette, STAR_PALETTE_X, STAR_PALETTE_Y, STAR_PALETTE_SIZE);
	
	//Init stars
    for(i = 0; i < STAR_COUNT; i++)
    {
        StarfieldStarInit(&starfield[i]);
    }
}

void StarfieldUpdate()
{
    int i;
    Star* star;
    StarDef* starDef;
    
    for(i = 0; i < STAR_COUNT; i++)
    {
        star = &starfield[i];
        starDef = star->starDef;
        
		star->sprite.y += star->velY;
		if(star->sprite.y >= (SCREEN_HEIGHT/2))
		{
		    star->sprite.x = -(SCREEN_WIDTH/2) + (rand() % SCREEN_WIDTH);
            star->sprite.y = -(SCREEN_HEIGHT/2) - starDef->imgHeight;
    		star->velY = RandRanged(starDef->speedMin, starDef->speedMax);
        }
    }
}

void StarfieldDraw(GsOT* orderTable)
{
    int i;
    
    for(i = 0; i < STAR_COUNT; i++)
    {
        GsSortFastSprite(&starfield[i].sprite, orderTable, 0);
    }
}
