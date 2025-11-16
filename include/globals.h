//全局变量 / 常量的定义
#include <mcp_can.h>
#include <Joystick.h>

extern MCP_CAN CAN;
extern Joystick_ joystick;
extern int      signum(int val);

extern uint8_t  g_attempts_count;
extern bool     g_serial_rcv_ok;

extern int8_t   g_gws_gear;
extern bool     g_gws_shifter_manual;

extern uint8_t  previous_Lever_position;
extern uint8_t  now_Lever_position;
extern uint8_t  Lever_position;
extern uint8_t  Park_button;

extern bool     g_button_side_pressed;
extern bool     g_button_middle_pressed;

extern int8_t   g_game_gear;            // 来自串口的数据，表示当前游戏内档位
extern boolean  g_game_shift_mode;      // 变速箱模式（0 =AUTO, 1=MANUAL）
extern boolean  g_game_parkbrake;       // 手刹状态（0=OFF, 1=ON）
extern uint8_t  g_game_hour;            // 当前小时数（0-23）

extern uint8_t  padbutton_num;          // 需要动作的游戏杆按键编号

extern bool     g_serial_rcv_ok;        // 数据包接收标记

// Value Meaning,GWS由0x197 CAN ID发送档位动作值定义 Byte[2]
#define Centre      0x0E	//Centre middle autoMatic gear position
#define UpOne       0x1E	//Pushed "up" (towards front of car)
#define UpTwo       0x2E	//Pushed "up" two notches
#define DownOne     0x3E	//Pushed "down" (towards back of car)
#define DownTwo     0x4E	//Pushed "down" two notches

#define CentreSide  0x7E	//Centre side Manual gear position
#define SideUp      0x5E	//Pushed "side and up"
#define SideDown    0x6E	//Pushed "side and down"
#define Park_button_PRESS 0xD5 //if Park is pressed

// GWS档位定义,需要发送到GWS ID 0x3FD 的数据，循环周期：100-200ms
// Byte[2] (Payload byte 0)	Display
#define	shift_Blank   0x00  // No gear selected   
#define shift_P       0x20  //  Park
#define shift_R       0x40  //  Reverse  
#define shift_N       0x60  //  Neutral
#define shift_D       0x80  //  Drive
#define shift_D_MS    0x81	//  D,  can move to M/S
#define shift_MS      0x81	//  M/S (if lever moved to side)
#define shift_Flash   0x08	//  To flash the selected gear light

// 需要动作的游戏杆按键编号
#define pad_auto_manual     1   //
#define pad_parkbarke       2   //

#define pad_shift_N         3   //
#define pad_shift_D         4   //
#define pad_shift_R         5   //
#define pad_shift_SideUp    6   //
#define pad_shift_SideDown  7   //
