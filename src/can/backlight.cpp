#include <globals.h>
#include <canbus.h>

const uint32_t CAN_BUS_ID = 0x202;

uint8_t backlightFrame[2] = {0xFF, 0x00};

void sendBacklight()
{
   if(      g_game_hour > 17 
        ||  g_game_hour <  7                 // 夜间时间
    ) 
        backlightFrame[1] = 0xFF;               // 全亮
    else {
        backlightFrame[1] = 0x00;               // 关闭
    }   
  
    CAN.sendMsgBuf(CAN_BUS_ID, 2, backlightFrame);
}