#ifndef _CONSTS_H
#define _CONSTS_H

#define YAROZE_USE_CD        0

#define SCREEN_WIDTH         320
#define SCREEN_HEIGHT        240
#define FRAMEBUFFER_WIDTH    1024
#define FRAMEBUFFER_HEIGHT   512
#define PROJECTION_DIST      1024
#define ORDER_TABLE_LENGTH	 14      // 1<<X tags, max 14 (16384 tags)
#define NUM_DRAW_BUFFERS     2
#define MAX_GPU_PACKETS	     1024*10
#define GPU_PACKET_SIZE      24      // 6 words
#define TPAGE_WIDTH          64
#define TPAGE_HEIGHT         256

#define CLEAR_COLOUR_R       128
#define CLEAR_COLOUR_G       64
#define CLEAR_COLOUR_B       64

#define CAMERA_INITIAL_X     0
#define CAMERA_INITIAL_Y     0
#define CAMERA_INITIAL_Z     1000

#define DEBUG_FONT_VRAM_X    960
#define DEBUG_FONT_VRAM_Y    0

#define REG_ISTAT (*(volatile u_long*)(0x1F801070))
#define REG_IMASK (*(volatile u_long*)(0x1F801074))

#endif // _CONSTS_H
