// ---------------------------------------------------------------------------------------------------------------------------------
// OnStep 智能网页服务器扩展模块 (SmartWebServer Add-on) 配置

/*
 * 有关设置此扩展模块的更多信息，请参阅 https://onstep.groups.io/g/main/wiki/7119 
 * 并加入 OnStep Groups.io 讨论组 https://groups.io/g/onstep
 * * *** 请务必阅读编译器的警告和错误信息，它们能帮你防止配置错误 ***
*/

// ---------------------------------------------------------------------------------------------------------------------------------
// 调整以下内容以配置您的扩展模块功能 ------------------------------------------------------------------------
// <-Req'd = 必须设置, <-Often = 通常需要设置, Option = 可选, Adjust = 按需调整, Infreq = 极少更改

// 若 WiFi 启动成功，将出现一个默认 SSID 为 "ONSTEP" 且默认密码为 "password" 的热点。
// 连接后，网页管理地址是 "192.168.0.1"，命令通道地址是 "192.168.0.1:9996 到 9999"。
// 如果被锁死无法连接，在再次上传/烧录之前，从 Arduino IDE 的 "工具" (Tools) 菜单中选择 
// "Erase Flash: All Flash Contents" (擦除所有 Flash 内容) 可以帮助恢复对 ESP8266 或 ESP32 的访问；e:\模型\谐波赤道仪\Onstep\OnStep\SmartWebServer\src\locales\Locale.h
// 或者查看 Extended.config.h 以清除 NV (非易失性存储)。

//      参数名                        值       默认值    注释                                                                       提示
// 运行模式 (OPERATIONAL MODE) ----------------------------------------------------------------------------------------------------------------
#define OPERATIONAL_MODE             WIFI //   WIFI, 或者使用 ETHERNET_W5100 或 ETHERNET_W5500 (有线网模块)                  <-必须设置

// 串口设置 (SERIAL PORTS) --------------------------------------------------------------------------------------------------------------------
#define SERIAL_BAUD_DEFAULT          9600 //   9600, 此参数的常用波特率为 9600, 19200, 57600, 115200 等。                      极少改
                                          //         与其连接的 OnStep 串口必须使用与上面相同的速率。
#define SERIAL_BAUD                115200 // 115200, 或者使用 19200, 57600, 115200, 230400, 460800 (并非所有设备都支持 > 115200)
                                          //         (OnStep 和这里都要改。) 如果与 Mega2560 版 OnStep 通信，自动使用 19200。
#define SERIAL_SWAP                  AUTO //   AUTO, 自动检查两者。ON 为交换端口 (Swapped)，OFF 为仅使用默认端口。             极少改
                                          //         此选项在 ETHERNET (以太网) 模式下被忽略。

// 状态指示灯 (STATUS LED) ----------------------------------------------------------------------------------------------------------------------
#define LED_STATUS                     ON //     ON, 启用 LED，连接过程中闪烁，连接成功后常亮。                                极少改
#define LED_STATUS_ON_STATE          HIGH //   HIGH, LED 点亮时的电平状态 (HIGH 或 LOW)。

// 显示设置 (DISPLAY - 网页界面) -------------------------------------------------------------------------------------------------------------------------
#define DISPLAY_LANGUAGE             L_cn //   L_en, 英语。或者 L_ce, L_de, L_en, L_us, L_es (两字母国家代码)。                按需
#define DISPLAY_WEATHER               OFF //    OFF, ON 以本地默认单位显示环境状况 (需传感器)。                               可选
#define DISPLAY_INTERNAL_TEMPERATURE   ON //    OFF, ON 以本地默认单位显示 MCU 内部温度。                                     可选
#define DISPLAY_WIFI_SIGNAL_STRENGTH   ON //     ON, 通过 Web 界面报告无线信号强度。OFF 则不显示。                              可选
#define DISPLAY_RESET_CONTROLS        OFF //    OFF, ON 允许复位 (如果支持)，FWU 用于 STM32 固件上传引脚置高。                可选

#define DISPLAY_SERVO_MONITOR         OFF //    OFF, ON 显示 OnStepX 的伺服电机监视器 (任意轴)。                              可选
#define DISPLAY_SERVO_ORIGIN_CONTROLS OFF //    OFF, ON 显示用于设置伺服绝对编码器原点的控制选项。                            可选

// 编码器支持 (ENCODER SUPPORT) -----------------------------------------------------------------------------------------------------------------
// 以下部分设置在首次上传时作为默认值存储在 NV (EEPROM) 中。除非擦除 NV 
// 或配置为运行时恢复默认值，否则以后对下面的更改可能会被忽略。
#define ENC_AUTO_SYNC_DEFAULT          ON //     ON, 自动将编码器位置同步给 OnStep。                                          可选
#define ENC_AUTO_SYNC_MEMORY          OFF //    OFF, ON 掉电保存“自动同步”的设置。                                            可选
#define ENC_SYNC_DURING_GOTO           ON //    OFF, ON 高分辨率编码器即使在 GOTO (高速转向) 期间也能修正指向。               可选

#define AXIS1_ENCODER                 OFF //    OFF, AB, AB_ESP32, CW_CCW, PULSE_DIR, AS37_H39B_B. 赤经/方位轴 (A/MA) & (B/SLO.) 可选
#define AXIS1_ENCODER_TICKS_DEG  22.22222 // 22.222, n, (ticks/degree.) 每度编码器脉冲数 (步数)。                             按需
#define AXIS1_ENCODER_REVERSE         OFF //    OFF, ON 反转计数方向。                                                        可选
#define AXIS1_ENCODER_DIFF_LIMIT_TO   300 //    300, n, (角秒) 编码器同步 *到* OnStep 时的最小差异阈值 (小于此值不更正)。     按需
#define AXIS1_ENCODER_DIFF_LIMIT_FROM OFF //    OFF, n, (角秒) 从 OnStep 同步 *回* 编码器时的最大差异阈值。                   按需
                                          //         用于绝对编码器。

#define AXIS2_ENCODER                 OFF //    OFF, AB, AB_ESP32, CW_CCW, PULSE_DIR, AS37_H39B_B. 赤纬/高度轴 (配置同上)。    可选
#define AXIS2_ENCODER_TICKS_DEG  22.22222 // 22.222, n, (ticks/degree.) 每度编码器脉冲数 (步数)。                             按需
#define AXIS2_ENCODER_REVERSE         OFF //    OFF, ON 反转计数方向。                                                        可选
#define AXIS2_ENCODER_DIFF_LIMIT_TO   300 //    300, n, (角秒) 最小差异阈值。                                                 按需
#define AXIS2_ENCODER_DIFF_LIMIT_FROM OFF //    OFF, n, (角秒) 最大差异阈值。                                                 按需
                                          //         用于绝对编码器。

// 用户配置结束！
// -------------------------------------------------------------------------------
#include "Extended.config.h"