#ifndef _CONSTS_H
#define _CONSTS_H

#define SCREEN_WIDTH         320
#define SCREEN_HEIGHT        240
#define PROJECTION_DIST      1000
#define ORDER_TABLE_LENGTH	 12      // 1<<X tags, max 14 (16384 tags)
#define NUM_DRAW_BUFFERS     2
#define MAX_GPU_PACKETS	     10000
#define GPU_PACKET_SIZE      24      // 6 words
#define TPAGE_WIDTH          64
#define TPAGE_HEIGHT         256

#define CLEAR_COLOUR_R       64
#define CLEAR_COLOUR_G       64
#define CLEAR_COLOUR_B       64

#define CAMERA_INITIAL_X     0
#define CAMERA_INITIAL_Y     0
#define CAMERA_INITIAL_Z     600

#define REG_ISTAT (*(volatile u_long*)(0x1F801070))
#define REG_IMASK (*(volatile u_long*)(0x1F801074))

#endif // _CONSTS_H
