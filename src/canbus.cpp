#include <canbus.h>
#include <globals.h>

uint32_t previous = 0;

void sendCanBus()
{
    uint32_t current = millis();

    if (current - previous >= 100)
    {
        sendBacklight();        // 传入当前小时数
        sendGear();

        previous = current;
    }
}

void readCanBus()
{
    readGear();
}