#include <Arduino.h>

#include <configuration.h>
#include <globals.h> //
#include <canbus.h>
#include <gamepad.h>
#include <serial.h>

// constants won't change. They're used here to set pin numbers:
const int settingPin = 7;  // the number of the pushbutton pin
const int CAN_ledPin = 6;  // the number of the LED pin
const int UART_ledPin = 8; // the number of the LED pin

unsigned long previousTime = 0;     // 存储上一次循环执行的时间（毫秒）
const unsigned long interval = 500; // 循环间隔（100ms）

bool g_serial_rcv_ok = 0;
bool setting_stat = 1;

// 定义全局变量（分配内存，可初始化）
int8_t g_game_gear = 0;        // 来自串口的数据，表示当前游戏内档位
boolean g_game_shift_mode = 0; // 变速箱模式（0 =AUTO, 1=MANUAL）
boolean g_game_parkbrake = 0;  // 手刹状态（0=OFF, 1=ON）
uint8_t g_game_hour = 0;       // 当前小时数（0-23）

uint8_t padbutton_num = 0; // 假设是int类型，根据实际声明调整

MCP_CAN CAN(SPI_CS_PIN);
Joystick_ joystick(0x03, JOYSTICK_TYPE_GAMEPAD, 32, 0, false, false, false, false, false, false, false, false, false, false, false);

int signum(int val)
{
    return (0 < val) - (val < 0);
}

/*
输入：
来自串口接收的数据
    int8_t  g_game_gear;            // 来自串口的数据，表示当前游戏内档位
    boolean g_game_shift_mode;      // 变速箱模式（0 =AUTO, 1=MANUAL）
    boolean g_game_parkbrake;       // 手刹状态（0=OFF, 1=ON）
    int8_t  g_game_hour;            // 当前小时数（0-23）
gws数据：
    Lever_position = rxBuf[2];
    Park_button =   rxBuf[3];
开关状态：
    setting_switch                  // 0:设置；1：工作
输出：
    g_gws_gear                      // GWS档位定义,需要发送到GWS ID 0x3FD 的数据，循环周期：100-200ms

*/
void processGwsFunction()
{
    if (Park_button == 0xD5  // if Park is pressed
        && g_game_gear == 0) // 并且游戏内档位为：N
    {
        g_gws_gear = shift_P;
        padbutton_num = pad_parkbarke; // 2
    }

    switch (now_Lever_position)
    {
    case UpOne:
    case UpTwo:
        if (setting_stat == 0) // 配置模式，或者当前 “D”档
        {
            g_gws_gear = shift_N; // 切换到“N”档
            padbutton_num = pad_shift_N;
        }
        else
        {
            if (g_game_gear == 0)
            {                         // 当前 “N”档（且setting_stat != 0）
                g_gws_gear = shift_R; // 切换到“R”档
                padbutton_num = pad_shift_R;
            }
            else
            { // 当前 “R”档（且setting_stat != 0）
              // 不执行操作
            }
        }
        break;

    case DownOne:
    case DownTwo:

        break;

    case Centre:
        g_gws_gear = shift_D_MS; // 手动切自动
        padbutton_num = pad_auto_manual;
        break;

    case CentreSide:
        g_gws_gear = shift_D_MS; // 自动切手动
        padbutton_num = pad_auto_manual;
        break;

    case SideUp:
        padbutton_num = pad_shift_SideUp; // 手动 -
        break;

    case SideDown:
        padbutton_num = pad_shift_SideDown; // 手动 +
        break;

    default:
        break;
    }

    previous_Lever_position = now_Lever_position; // 更新杆位记录

    if (g_serial_rcv_ok) // 收到数据包
    {
        Serial.print("g_gws_gear = ");
        Serial.println(g_gws_gear, HEX);
        Serial.println("");

        g_serial_rcv_ok = false;
    }
}

void setup()
{
    // initialize the LED pin as an output:
    pinMode(CAN_ledPin, OUTPUT);
    pinMode(UART_ledPin, OUTPUT);

    // initialize the pushbutton pin as an input:
    pinMode(settingPin, INPUT);

    Serial.begin(SERIAL_BAUD_RATE);
    g_serial_rcv_ok = false; //

    if (CAN.begin(MCP_ANY, CAN_500KBPS, CAN_CLOCK) == CAN_OK)
    {
        Serial.println("MCP2515 Initialized successfully");
    }

    CAN.setMode(MCP_NORMAL);

    joystick.begin();

    g_gws_gear = shift_P;
    setting_stat = digitalRead(settingPin);
}

void loop()
{

    // 获取当前系统时间（从程序启动到现在的毫秒数）
    unsigned long currentTime = millis();

    // 检查是否达到100ms间隔：当前时间 - 上一次执行时间 >= 100ms
    if (currentTime - previousTime >= interval)
    {
        // 更新上一次执行时间（为当前时间，确保下次间隔从这里开始计算）
        previousTime = currentTime;

        // --------------------------
        // 这里是100ms循环内要执行的代码
        // --------------------------
        sendGear(); // Renew CAN bus data to GWS
    }

    if (setting_stat == 1)  // 工作模式
    {
        readSerial();       // Read incoming serial data form simhub(ETS2/ATS)
    }
    readCanBus();           // Read CAN bus data from GWS
                            // uint8_t  Lever_position = rxBuf[2];
                            // uint8_t  Park_button = rxBuf[3];
    processGwsFunction();   // Process GWS gear and mode

    // sendJoystick();      // Update gamepad state
}