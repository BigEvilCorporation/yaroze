#ifndef _INTERPT_H
#define _INTERPT_H

#include <libps.h>

typedef void(*INTHandler)(void);

INTHandler INTInstallHandler(int irq, INTHandler handler);

#endif // _INTERPT_H
