#include "Interpt.h"

void _install_irq_handler(int irq, void(*func)(void));

INTHandler INTInstallHandler(int irq, INTHandler handler)
{
    //_install_irq_handler(irq, handler);
    return NULL;
}
