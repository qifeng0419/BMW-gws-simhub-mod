/*
// 获取挡位信息
var game_gear = $prop('DataCorePlugin.GameRawData.TruckValues.CurrentValues.DashboardValues.GearDashboards');
// 获取手动换挡模式状态（1为手动，0为自动）
var game_manual = Number($prop('GameRawData.TruckValues.ConstantsValues.MotorValues.ShifterTypeValue') === 3);
// 获取当前时间并提取小时（24小时制）
var currentDateTime = $prop('DataCorePlugin.CurrentDateTime');
var current_hour = currentDateTime ? new Date(currentDateTime).getHours() : 0; // 转换为日期对象后提取小时

// 输出格式：SH;挡位;手动模式;小时;\n
return `ETS|${game_gear}|${game_manual}|${current_hour}|\n`;

//串口输出格式：
ETS|${game_gear}|${game_manual}|${current_hour}|\n
//测试输出格式：
ETS|0|0|14|\n
*/