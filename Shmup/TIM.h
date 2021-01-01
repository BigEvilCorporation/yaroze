#ifndef _TIM_H
#define _TIM_H

#include <libps.h>

#define TIM_PMODE_4   0
#define TIM_PMODE_8   1
#define TIM_PMODE_16  2
#define TIM_PMODE_24  3
#define TIM_PMODE_MIX 4

void TIMLoad(u_long* timData);

#endif // _TIM_H
