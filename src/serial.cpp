/*
ETS|0|0|0|16|end
ETS|game_gear|game_manual|brake|{current_hour}|end\n`;
*/

#include <globals.h>
#include <serial.h>

/* 假设全局变量已调整为以下类型（需在globals.h中定义）：
int8_t  g_game_gear;            // 来自串口的数据，表示当前游戏内档位
boolean g_game_shift_mode;      // 变速箱模式（0 =AUTO, 1=MANUAL）
boolean g_game_parkbrake;       // 手刹状态（0=OFF, 1=ON）
int8_t  g_game_hour;            // 当前小时数（0-23）
*/

void readSerial() {
    while (Serial.available()) {
        // 读取起始标识（"ETS"），分隔符为"|"
        String start = Serial.readStringUntil('|');
        if (start != "ETS") {
            // 起始标识错误，清空当前缓存避免干扰后续解析
            Serial.flush();
            continue;
        }

        // 解析4个核心字段（协议固定为4个字段）
        String field1 = Serial.readStringUntil('|');  // 字段1： 
        String field2 = Serial.readStringUntil('|');  // 字段2：
        String field3 = Serial.readStringUntil('|');  // 字段3：
        String field4 = Serial.readStringUntil('|');  // 字段4：

        // 读取结束标识（"end"），验证数据包完整性
        String endFlag = Serial.readStringUntil('\n');  // 假设数据包以换行结尾（如示例中的"\n"）
        // 移除可能的空格或空字符，确保匹配"end"
        endFlag.trim();
        if (endFlag == "end") {
               
            // 更新全局变量（根据实际业务场景可对字段进行类型转换）
            g_game_gear = field1.toInt();               // 假设字段1为整数类型
            g_game_shift_mode = (field2.toInt() != 0);  // 假设字段2为布尔类型
            g_game_parkbrake = (field3.toInt() != 0);   // 假设字段3为布尔类型
            g_game_hour = field4.toInt();               // 假设字段4为整数类型
            
            g_serial_rcv_ok = true;
            
            // （可选）如果参数需要数值类型，可在此转换
            // 例如：int paramValue = field4.toInt();
            Serial.print(   "g_game_gear = ");      // 来自串口的数据，表示当前游戏内档位
            Serial.println( g_game_gear,DEC );          // 来自串口的数据，表示当前游戏内档位
        
            Serial.print("g_game_shift_mode = " );  // 变速箱模式（0 =AUTO, 1=MANUAL）
            Serial.println(g_game_shift_mode ,BIN );     // 变速箱模式（0 =AUTO, 1=MANUAL）
        
            Serial.print("g_game_parkbrake = ");    // 手刹状态（0=OFF, 1=ON）
            Serial.println(g_game_parkbrake,HEX );       // 手刹状态（0=OFF, 1=ON）
        
            Serial.print("g_game_hour = ");         // 当前小时数（0-23）
            Serial.println(g_game_hour,DEC );            // 当前小时数（0-23）
            
            g_serial_rcv_ok = true;                  //

        }    
    }
}