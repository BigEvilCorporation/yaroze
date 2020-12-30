#ifndef _STARS_H
#define _STARS_H

#define STAR_COUNT           64
#define STAR_NUM_DEFS        4
#define STAR_VRAM_X          640
#define STAR_VRAM_Y          0
#define STAR_PALETTE_X       0
#define STAR_PALETTE_Y       480
#define STAR_PALETTE_SIZE    2

void StarfieldInit();
void StarfieldUpdate();
void StarfieldDraw(GsOT* orderTable);

#endif //_STARS_H
