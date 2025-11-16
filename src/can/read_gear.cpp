#include <globals.h>
#include <canbus.h>

uint8_t currentCounter = 0;

uint8_t previous_Lever_position = 0x00;
uint8_t now_Lever_position = 0x0e;
uint8_t Park_button = 0x00;

void readGear()
{
    unsigned long rxId;
    byte len;
    byte rxBuf[8];

    if (CAN.readMsgBuf(&rxId, &len, rxBuf) == CAN_OK 
        && rxId == 0x197
    )
    {
        byte readerCounter = rxBuf[1];
        if (readerCounter != currentCounter)
        {
            now_Lever_position = rxBuf[2];
            Park_button =   rxBuf[3];
            currentCounter = readerCounter;
        }
    }
}