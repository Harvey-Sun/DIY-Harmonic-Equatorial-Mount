// ---------------------------------------------------------------------------------------------------------------------------------
// OnStep WiFi 扩展模块配置文件

/*
 * 关于设置此插件的更多信息，请参阅 https://onstep.groups.io/g/main/wiki/7119 
 * 并加入 OnStep Groups.io 讨论组 https://groups.io/g/onstep
 * * *** 请务必阅读编译器的警告和错误信息，它们能帮你防止配置错误 ***
*/

// ---------------------------------------------------------------------------------------------------------------------------------
// 调整以下内容以配置您的扩展模块功能 ------------------------------------------------------------------------
// <-Req'd = 必须设置, <-Often = 通常需要设置, Option = 可选, Adjust = 按需调整, Infreq = 极少更改

// 首次成功启动时，会出现一个 SSID 为 "ONSTEP" 的热点。
// 连接后：网页管理地址是 "192.168.0.1"，
// 命令通道地址是 "192.168.0.1:9999"。
// 如果被锁死无法连接，在 Arduino 的 "Tools" (工具) 菜单中选择 "Erase Flash: All Flash Contents" (擦除所有 Flash 内容) 
// 然后重新上传/烧录，通常能恢复对 ESP8266 的访问。

//      参数名                        值       默认值    注释                                                                       提示
// 串口设置 (SERIAL PORTS) --------------------------------------------------------------------------------------------------------------------
#define SERIAL_BAUD_DEFAULT          9600 //   9600, 这些参数的常用波特率为 9600, 19200, 57600, 115200。                      极少改
#define SERIAL_BAUD                 57600 //  57600, 如果连接的是 Mega2560 版 OnStep，会自动使用 19200。                      极少改
                                          //         如果建立与 OnStep 的连接 ***失败***，ESP8266 可能会
                                          //         保留之前的设置（例如出厂默认的 SSID）。
#define SERIAL_SWAP                  AUTO //   AUTO, 自动检测。ON 为交换端口 (Swapped)，OFF 为仅使用默认端口。                极少改

// 用户反馈 (USER FEEDBACK) -------------------------------------------------------------------------------------------------------------------
#define LED_STATUS                      2 //      2, WeMos D1 Mini 的 GPIO LED 引脚号。连接中闪烁，连接成功常亮。                 极少改

// 显示设置 (DISPLAY - 网页界面) -------------------------------------------------------------------------------------------------------------------------
#define DISPLAY_LANGUAGE             L_en //   L_en, 英语。如果支持，指定两个字母的国家代码（如 L_cn）。                      按需
#define DISPLAY_WEATHER               OFF //    OFF, ON 以本地默认单位显示环境状况（需传感器支持）。                          可选
#define DISPLAY_INTERNAL_TEMPERATURE  OFF //    OFF, ON 以本地默认单位显示 MCU 内部温度。                                     可选
#define DISPLAY_WIFI_SIGNAL_STRENGTH   ON //     ON, 在 Web 界面报告无线信号强度。OFF 则不显示。                              可选

#define DISPLAY_SPECIAL_CHARS          ON //     ON, 显示标准 ASCII 特殊符号（为了兼容性）。                                  极少改
#define DISPLAY_ADVANCED_CHARS         ON //     ON, 显示标准的 "RA/Dec" 文字而不是符号。                                     极少改
#define DISPLAY_HIGH_PRECISION_COORDS OFF //    OFF, ON 在状态页面显示高精度坐标。                                            极少改

// 命令通道 (COMMAND CHANNELS) ----------------------------------------------------------------------------------------------------------------
#define STANDARD_COMMAND_CHANNEL       ON //     ON, 启用标准命令通道端口 9999，用于安卓 App 和 ASCOM 驱动。                  极少改
#define PERSISTENT_COMMAND_CHANNEL     ON //     ON, 启用持久命令通道端口 9998，用于 INDI? 和 Stellarium Mobile (星空漫步)。  极少改
                                          //         实验性功能，如果启用可能会导致标准命令通道出现问题。

// 编码器支持 (ENCODER SUPPORT - 仅当编码器接在 WiFi 模块上时) -----------------------------------------------------------------------------------------------------------------
#define ENC_AUTO_SYNC_DEFAULT          ON //     ON, 自动将编码器位置同步给 OnStep 主控。                                     可选

#define AXIS1_ENC                     OFF //    OFF, CWCCW, AB, BC_BISSC. 赤经/方位轴 (A/CW/MA) & (B/CCW/SLO.)                可选
#define AXIS1_ENC_REVERSE             OFF //    OFF, ON 反转计数方向。                                                        按需
#define AXIS1_ENC_TICKS_DEG      22.22222 // 22.222, n, (ticks/degree.) 每度编码器脉冲数 (步数)。                             按需
#define AXIS1_ENC_DIFF_LIMIT_TO       300 //    300, n, (角秒) 编码器与 OnStep 同步时的最小差异阈值 (小于此值不更正)。        按需
#define AXIS1_ENC_DIFF_LIMIT_FROM     OFF //    OFF, n, (角秒) 从 OnStep 同步时的最大差异阈值。                               按需
                                          //         对于绝对编码器，设置零点时保持关闭，然后再启用。

#define AXIS2_ENC                     OFF //    OFF, CWCCW, AB, BC_BISSC. 赤纬/高度轴 (配置同上)。                            可选
#define AXIS2_ENC_REVERSE             OFF //    OFF, ON 反转计数方向。                                                        可选
#define AXIS2_ENC_TICKS_DEG      22.22222 // 22.222, n. 每度脉冲数。                                                          按需
#define AXIS2_ENC_DIFF_LIMIT_TO       300 //    300, n. 最小差异阈值。                                                        按需
#define AXIS2_ENC_DIFF_LIMIT_FROM     OFF //    OFF, n. 最大差异阈值。                                                        按需
                                          //         对于绝对编码器，设置零点时保持关闭，然后再启用。

// 编码器速率控制 (ENCODER RATE CONTROL)
#define AXIS1_ENC_RATE_CONTROL        OFF //    OFF, ON 启用 RA (赤经) 高分辨率编码器的速率控制。仅限 EQ (赤道仪) 模式。      极少改
#define AXIS1_ENC_INTPOL_COS          OFF //    OFF, ON 启用余弦补偿功能。                                                    极少改
#define AXIS1_ENC_RATE_AUTO           OFF //    OFF, n, (蜗杆周期秒数) 调整平均编码器脉冲率以进行计算。                       可选
                                          //         用于修正上一个蜗杆周期内的平均导星速率偏差。                             可选
#define AXIS1_ENC_BIN_AVG             OFF //    OFF, n, (Bin数量) 启用分箱滚动平均功能。                                      可选

// 用户配置结束！
// -------------------------------------------------------------------------------