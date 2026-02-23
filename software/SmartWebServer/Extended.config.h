// ---------------------------------------------------------------------------------------------------------------------------------
// OnStep 智能网页服务器 (SmartWebServer) 扩展配置（不常用选项）

//                                 *** 大多数用户不需要修改此文件中的任何内容！ ***

// ---------------------------------------------------------------------------------------------------------------------------------
// 调整以下内容以配置您的控制器功能 ----------------------------------------------------------------------
// <-Req'd = 必须设置, <-Often = 通常设置, Option = 可选, Adjust = 按需调整, Infreq = 极少更改

// 调试 (DEBUG) ------------------------------------------------------------ 参阅 https://onstep.groups.io/g/main/wiki/6-Configuration#DEBUG
// 在指定的 SERIAL_DEBUG 端口上启用额外的调试和/或状态消息
// 请注意，SERIAL_DEBUG 端口不能用于与 OnStep 的正常通信

//      参数名                        值          默认值   注释                                                                  提示
#define DEBUG                         OFF //         OFF, 使用 ON 仅显示后台错误消息，使用 VERBOSE 显示所有                   极少改
                                          //              错误和状态消息，使用 PROFILER 启用 VT100 任务分析器，
                                          //              使用 REMOTE 将详细调试输出转发给 OnStep。
#define SERIAL_DEBUG               Serial //      Serial, 或使用任何硬件串口。Serial1 或 Serial2 等（如果支持）。             可选
#define SERIAL_DEBUG_BAUD            9600 //        9600, n. n=9600,19200,57600,115200 (常用波特率)。                         可选

// 非易失性存储 (NON-VOLATILE STORAGE - EEPROM/Flash) -------------------------------------------------------------------------------
#define NV_WIPE                       OFF //         OFF, 导致将默认值写回 NV（FLASH，EEPROM 等）。                           极少改
                                          //              *** 重要提示：启用此选项然后上传，等待一两分钟      ***
                                          //              *** 然后将此选项设置为 OFF 并再次上传。            ***
                                          //              *** 保持此选项启用会导致 NV 过度磨损                ***
                                          //              *** 并永久损坏微控制器的存储子系统！！！            ***

// 密码 (PASSWORD) -----------------------------------------------------------------------------------------------------------------
#define PASSWORD_DEFAULT       "password" //  "password", 按需调整，这也可以在运行时更改。                                    按需
                                          //              用于运行时访问网络设置的密码。

// 蓝牙手柄设置 (仅限 ESP32) ------------------------------------------------ 参阅 https://onstep.groups.io/g/main/wiki/26762
#define BLE_GAMEPAD                   OFF //         OFF, ON 允许蓝牙手柄连接（仅限 ESP32）。                                 可选
#define BLE_GP_ADDR   "ff:ff:de:09:f5:cf" // ...f:ff:ff", 手柄 MAC 地址 #1                                                    按需
                                          //              手柄设备地址必须完全匹配，否则无法连接！
                                          //              将地址替换为你的地址，使用小写字母，如上所示使用冒号分隔。
#define BLE_GP_ADDR1  "ff:ff:ff:ff:ff:ff" // ...f:ff:ff", 手柄 MAC 地址 #2                                                    按需
                                          //              允许两个手柄，以“切换”模式运行，一次只能连接一个。

// IP 地址设置 (IP ADDRESS SETTINGS) -----------------------------------------------------------------------------------------------

#define MDNS_NAME                "onstep" //   "onstep",  mDNS 设备名称。                                                     按需

// SWS 的唯一（以太网）MAC 地址
#define MAC {0xDE,0xAD,0xBE,0xEF,0xFE,0xEE}

// 以下设置存储在 NV (EEPROM) 中，SWS 不会识别下面的设置，除非是第一次上传或擦除 NV。
// 但是，这些设置（如适用）可以在运行时通过网页界面更改。
#define AP_ENABLED                   true //        true, 启用 WiFi 热点 (Access Point) 模式。                                按需
#define AP_SSID                  "ONSTEP" //    "ONSTEP", WiFi 热点名称 (SSID)。                                              按需
#define AP_PASSWORD            "password" //  "password", WiFi 热点密码。                                                     按需
#define AP_CHANNEL                      7 //           7, WiFi 热点信道。                                                     按需
#define AP_IP_ADDR          {192,168,0,1} // ..,168,0,1}, WiFi 热点 IP 地址。                                                 按需
#define AP_GW_ADDR          {192,168,0,1} // ..,168,0,1}, WiFi 热点网关地址。                                                 按需
#define AP_SN_MASK        {255,255,255,0} // ..55,255,0}, WiFi 热点子网掩码。                                                 按需

#define STA_ENABLED                 false //       false, 启用 WiFi 客户端 (Station) 模式。                                   按需
#define STA_SSID                   "Home" //      "Home", 要连接的家庭 WiFi 名称 (SSID)。                                     按需
#define STA_PASSWORD           "password" //  "password", WiFi 客户端模式密码。                                               按需
#define STA_DHCP_ENABLED            false //       false, 启用 WiFi 客户端/以太网 DHCP (自动获取IP)。                         按需
#define STA_IP_ADDR        {192,168,1,55} // ..168,1,55}, WiFi 客户端/以太网 IP 地址 (静态IP时使用)。                         按需
#define STA_GW_ADDR         {192,168,1,1} // ..,168,1,1}, WiFi 客户端/以太网网关地址。                                        按需
#define STA_SN_MASK       {255,255,255,0} // ..55,255,0}, WiFi 客户端/以太网子网掩码。                                        按需

// ONSTEP 复位控制 (ONSTEP RESET CONTROL) ------------------------------------------------------------------------------------------
#define RESET_PIN                     OFF //         OFF, 指定一个用于复位 OnStep 主控板的引脚号。                            极少改
#define RESET_PIN_STATE               LOW //         LOW, 指定 RESET_PIN 引脚的有效复位电平，不活动时则为开路 (OPEN)。        极少改
                                          //