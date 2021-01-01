#include "CDROM.h"

#define CD_CMD_STOP  0x08
#define CD_CMD_UNLOCK_1  0x50
#define CD_CMD_UNLOCK_2  0x51
#define CD_CMD_UNLOCK_3  0x52
#define CD_CMD_UNLOCK_4  0x53
#define CD_CMD_UNLOCK_5  0x54
#define CD_CMD_UNLOCK_6  0x55
#define CD_CMD_UNLOCK_7  0x56
#define CD_CMD_UNLOCK_8  0x57

//ASM
void _96_init();
void _96_remove();
void _96_CdStop();
void _InitCd();
void _cd_control(char command, char* params, int paramlen);
void _cd_wait_ack();
extern volatile char _cd_ack_wait;

static int CDROMCommand(int command, char* parameters)
{
    int paramlen = 0;
    
    //If not ready, bail
    //if(_cd_ack_wait)
    //    return 0;
        
    //Commands with params
    if(    command == CD_CMD_UNLOCK_2
        || command == CD_CMD_UNLOCK_3
        || command == CD_CMD_UNLOCK_4
        || command == CD_CMD_UNLOCK_5
        || command == CD_CMD_UNLOCK_6)
    {
        paramlen = 1;
    }
    
    _cd_control(command, parameters, paramlen);
        
    //_cd_wait_ack();
    return 1;
}

static void Wait()
{
    int i;
    for(i = 0; i < 120; i++)
    {
        VSync(0);
    }
}

void CDROMStopMotor()
{
    CDROMCommand(CD_CMD_STOP, NULL);
    Wait();
}

void CDROMUnlock()
{
    CDROMCommand(CD_CMD_UNLOCK_1, NULL);
    Wait();
    CDROMCommand(CD_CMD_UNLOCK_2, "Licensed by");
    Wait();
    CDROMCommand(CD_CMD_UNLOCK_3, "Sony");
    Wait();
    CDROMCommand(CD_CMD_UNLOCK_4, "Computer");
    Wait();
    CDROMCommand(CD_CMD_UNLOCK_5, "Entertainment");
    Wait();
    CDROMCommand(CD_CMD_UNLOCK_6, "World Wide");
    Wait();
    CDROMCommand(CD_CMD_UNLOCK_7, NULL);
    Wait();
    CDROMCommand(CD_CMD_UNLOCK_8, NULL);
    Wait();
}
