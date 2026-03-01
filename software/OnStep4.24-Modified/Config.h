// ---------------------------------------------------------------------------------------------------------------------------------
// OnStep 的配置

/*
 * 获取关于设置 OnStep 的更多信息请访问 http://www.stellarjourney.com/index.php?r=site/equipment_onstep 
 * 并且加入 OnStep Groups.io 群组 https://groups.io/g/onstep
 * * *** 阅读编译器警告和错误，它们有助于防止无效的配置 ***
*/

// ---------------------------------------------------------------------------------------------------------------------------------
// 调整以下内容以配置您的控制器功能 ----------------------------------------------------------------------
// <-Req'd = 始终必填, <-Often = 通常必填, Option = 可选, Adjust = 根据需要调整, Infreq = 极少更改

// 引脚映射 (PINMAP) ------------------------------------------------- 参见 https://onstep.groups.io/g/main/wiki/Configuration-Controller#PINMAP
//   *** 查看与您设置匹配的 Pins.xxx.h 文件（在 src/pinmaps/Models.h 中查找）获取详细信息 ***
//   *** 确保它与您的接线匹配。            *** 风险自负 *** ***

//      参数名称                        值   默认值   备注                                                                      提示
#define PINMAP                        OFF //    OFF，可选项：MksGenL2, MiniPCB2, MaxPCB2, MaxESP3, CNC3, STM32Blue,     <-Req'd
                                          //         MaxSTM3, FYSETC_S6_2 等。在 Constants.h 中可以找到其它主板和更多信息。

// 串口命令通道 (SERIAL PORT COMMAND CHANNELS) --------------------------- 参见 https://onstep.groups.io/g/main/wiki/Configuration-Controller#SERIAL
#define SERIAL_A_BAUD_DEFAULT        9600 //   9600, n。其中 n=9600,19200,57600,115200 (常用波特率。)                  Infreq
#define SERIAL_B_BAUD_DEFAULT        9600 //   9600, n。有关 MCU 串口号等，请参见 (src/HAL/)。                        Option
#define SERIAL_B_ESP_FLASHING         OFF //    OFF，ON 通过带有 :ESPFLASH# 命令的 SERIAL_B 上传 ESP8266 WiFi 固件。    Option
#define SERIAL_C_BAUD_DEFAULT         OFF //    OFF, n，为 ESP32 蓝牙开启 (ON)。                                               Option
#define SERIAL_C_BLUETOOTH_NAME  "OnStep" // "On..", ESP32 的蓝牙设备名称。                                         Option

// 赤道仪/经纬仪类型 (MOUNT TYPE) ----------------------------------------- 参见 https://onstep.groups.io/g/main/wiki/Configuration-Controller#MOUNT_TYPE
#define MOUNT_TYPE                    GEM //    GEM，GEM 代表德式赤道仪，FORK 代表叉式赤道仪，或者 ALTAZM 代表          <-Req'd
                                          //         道布森等经纬仪。GEM 赤道仪会执行子午线翻转。

// 用户反馈 (USER FEEDBACK) ----------------------------------- 参见 https://onstep.groups.io/g/main/wiki/Configuration-Controller#USER_FEEDBACK
#define LED_STATUS                     ON //     ON，恒星时追踪时闪烁，否则常亮表示正在活动。       Option
#define LED_STATUS2                   OFF //    OFF，ON 时通过 PPS 同步以 1 秒间隔闪烁，GOTO 期间常亮，待机时关闭。   Option
#define LED_RETICLE                   OFF //    OFF, n。其中 n=0..255 (0..100%) 激活功能并设置默认亮度。   Option
#define BUZZER                        OFF //    OFF, ON, n。其中 n=100..6000 (Hz 频率) 用于压电扬声器。ON 用于蜂鸣器。    Option
#define BUZZER_STATE_DEFAULT          OFF //    OFF，ON 启动时启用压电蜂鸣器/扬声器。                              Option

// 时间和位置 (TIME AND LOCATION) ----------------------------------------- 参见 https://onstep.groups.io/g/main/wiki/Configuration-Controller#TLS
#define TIME_LOCATION_SOURCE          OFF //    OFF，DS3231 (I2C)，DS3234 (SPI)，TEENSY (T3.2 内部) 或 GPS 时钟源。       Option
                                          //         提供日期/时间，如果可用，还提供 PPS（秒脉冲）和经纬度。

// 传感器 (SENSORS) ----------------------------------------------- 参见 https://onstep.groups.io/g/main/wiki/Configuration-Controller#SENSORS
// * = 也支持 ON_PULLUP 或 ON_PULLDOWN 以激活 MCU 内部电阻（如果存在）。
#define WEATHER                       OFF //    OFF，BME280 (I2C 0x77)，BME280_0x76，BME280_SPI (CS引脚见引脚映射)          Option
                                          //         BMP280 (I2C 0x77)，BMP280_0x76，BMP280_SPI (CS引脚见引脚映射)
                                          //         BME280 或 BMP280 用于温度、气压。BME280 还可测湿度。
                                          
#define TELESCOPE_TEMPERATURE         OFF //    OFF, DS1820, n。其中 n 是调焦器温度的 DS1820 序列号。            Adjust

#define HOME_SENSE                    OFF //    OFF, ON*。自动检测并使用 Home 限位开关。仅限 GEM 模式。      Option
#define HOME_SENSE_STATE_AXIS1       HIGH //   HIGH，从前方看，Home 位置顺时针方向的状态。设为 LOW 则反转。   Adjust
#define HOME_SENSE_STATE_AXIS2       HIGH //   HIGH，从上方看，Home 位置顺时针方向的状态。设为 LOW 则反转。   Adjust
                                          //         当运动逆时针越过 Home 位置时，信号状态反转。

#define LIMIT_SENSE                   OFF //    OFF, ON* 靠近接地的限位检测开关可停止 GOTO 和/或追踪。         Option
#define LIMIT_SENSE_STATE             LOW //    LOW，用于常开 (NO) 开关，HIGH 用于常闭 (NC) 开关。          Adjust

#define PEC_SENSE                     OFF //    OFF, ON*, n，检测数字信号或 n=0 到 1023（0 到 3.3V 或 5V）的模拟阈值。 Option
#define PEC_SENSE_STATE              HIGH //   HIGH，检测 PEC 信号的上升沿，或使用 LOW 检测下降沿。           Adjust
                                          //         在 ALTAZM (经纬仪) 模式下被忽略。

#define PPS_SENSE                     OFF //    OFF, ON* 启用 PPS (秒脉冲)，检测信号上升沿。           Option
                                          //         提供更好的追踪精度，特别是对于带有陶瓷谐振器的 Mega2560。

// ST4 接口 (ST4 INTERFACE) --------------------------------------------- 参见 https://onstep.groups.io/g/main/wiki/Configuration-Controller#ST4
// *** 您必须自行验证接口是否符合任何连接设备的电气规格，风险自负 ***
#define ST4_INTERFACE                 OFF //    OFF, ON, ON_PULLUP 启用接口。除非手控模式，否则以 <= 1X 的速度导星。  Option
                                          //         在 GOTO 期间按下按钮：中止 GOTO 或继续被子午线翻转暂停的复位
#define ST4_HAND_CONTROL              OFF //    OFF，ON 用于手控器特殊功能和 SHC 支持。                 Option
                                          //         按住 [E]+[W] 按钮 >2秒：导星速率   [E]减  [W]加  [N]追踪开/关 [S]同步
                                          //         按住 [N]+[S] 按钮 >2秒：用户星表项 [E]减  [W]加  [N]GOTO [S]声音开/关
#define ST4_HAND_CONTROL_FOCUSER      OFF //    OFF，ON 作为上述的替代：调焦器移动 [E]f1 [W]f2 [N]减     [S]加            Option

// 导星行为 (GUIDING BEHAVIOUR) ------------------------------------------ 参见 https://onstep.groups.io/g/main/wiki/Configuration-Mount#GUIDING
#define GUIDE_TIME_LIMIT                0 //      0，无导星时间限制。或者 n。其中 n=1..120 秒为时间限制保护。       Adjust
#define GUIDE_DISABLE_BACKLASH        OFF //    OFF，在 <= 1X 导星期间禁用齿隙补偿                          Option

// 追踪行为 (TRACKING BEHAVIOUR) ---------------------------------------- 参见 https://onstep.groups.io/g/main/wiki/Configuration-Mount#TRACKING
#define TRACK_AUTOSTART               OFF //    OFF，ON 启动时启用追踪。                                          Option
#define TRACK_REFRACTION_RATE_DEFAULT OFF //    OFF，ON 启动时带大气折射补偿（仅限 RA 轴/赤道仪）。   Option
#define TRACK_BACKLASH_RATE            25 //     25, n。其中 n=2..50 (x 恒星时速率) 在齿隙补偿期间使用。               Option
                                          //         太快会导致电机堵转/齿轮撞击，太慢则齿隙补偿迟钝。

// GOTO/旋转行为 (SLEWING BEHAVIOUR) ------------------------------------------ 参见 https://onstep.groups.io/g/main/wiki/Configuration-Mount#SLEWING
#define SLEW_RATE_BASE_DESIRED        1.0 //    1.0, n。所需的目标旋转速率 (度/秒)。运行时可调整范围为            <-Req'd
                                          //         该速率的 1/2 到 2 倍，且受 MCU 性能限制影响。
#define SLEW_RATE_MEMORY              OFF //    OFF，ON 在重启后记住设置的速率。                              Option
#define SLEW_ACCELERATION_DIST        5.0 //    5.0, n，(度)。用于加速 (和减速) 的近似距离。      Adjust
#define SLEW_RAPID_STOP_DIST          2.5 //    2.0, n，(度)。当中止 GOTO 旋转或超出限位时，              Adjust
                                          //         停止所需的近似距离。

// 镜墩侧行为 (PIER SIDE BEHAVIOUR) -------------------------------------- 参见 https://onstep.groups.io/g/main/wiki/Configuration---Mount#SYNCING
#define MFLIP_SKIP_HOME               OFF //    OFF，ON 直接 GOTO 目标，不返回 Home 位置。      Option
#define MFLIP_PAUSE_HOME_MEMORY       OFF //    OFF，ON 重启后记住子午线翻转在 Home 处暂停的设置。     Option
#define MFLIP_AUTOMATIC_MEMORY        OFF //    OFF，ON 重启后记住自动子午线翻转的设置。         Option

#define PIER_SIDE_SYNC_CHANGE_SIDES   OFF //    OFF，ON 允许同步更改镜墩侧，用于 GEM 德式赤道仪。                      Option
#define PIER_SIDE_PREFERRED_DEFAULT  BEST //   BEST，如果可能，保持在当前侧。EAST(东) 或 WEST(西) 尽可能切换。      Option

// 停车/归位行为 (PARKING BEHAVIOUR) ------------------------------------------ 参见 https://onstep.groups.io/g/main/wiki/Configuration-Mount#PARKING
#define STRICT_PARKING                OFF //    OFF，ON 仅在成功停车(Park)后才允许解除停车(Un-park)。                    Option

// 运动控制 (MOTION CONTROL) ---------------------------------------------- 参见 https://onstep.groups.io/g/main/wiki/Configuration-Mount#MOTION
#define STEP_WAVE_FORM             SQUARE // SQUARE, PULSE 较快速率的步进信号波形。SQUARE (方波) 提供最佳信号完整性。  Adjust

// 步进驱动器型号 (有关其它不常用的型号及更多信息，另请参见 ~/OnStep/src/sd_drivers/Models.h): 
// A4988, DRV8825, LV8729, S109, SSS TMC2209*, TMC2130* **, 和 TMC5160* ***
// * = 添加 _QUIET (静音步进/stealthChop 追踪)，例如 "TMC2130_QUIET"
// ** = SSS TMC2130 如果您选择设置步进驱动器电流(mA)，请将 Vref 电位器设置为 2.5V，而不是像通常那样通过电机电流设置。
// *** = SSS TMC5160 您必须使用 #define AXISn_TMC_IRUN (IHOLD 等) 来设置步进驱动器电流 (mA)。

// 驱动器细分、IRUN(运行电流)、Reverse(反向)、Limit Min(最小限位) 和 Limit Max(最大限位) 的设置存储在 NV (EEPROM) 中。这些运行时设置
// 可以从 SmartWebServer 的 Config 网页进行更改 (或恢复为以下默认值)。如果启用了运行时轴设置， 
// 对以下设置的更改可能会被忽略，因为会优先使用 NV (EEPROM) 中的运行时设置。

// AXIS1(轴1) 赤经(RA)/方位角(AZM)
// 参见 https://onstep.groups.io/g/main/wiki/Configuration-Mount#AXIS1
#define AXIS1_STEPS_PER_DEGREE    12800.0 //  12800, n。每度步数：                                          <-Req'd
                                          //         n = (步进电机步数 * 细分 * 总减速比)/360.0
#define AXIS1_STEPS_PER_WORMROT         0 //      0, n。蜗杆每转步数（仅限 PEC 赤道仪模式，0 为禁用 PEC。）   <-Req'd
                                          //         n = (AXIS1_STEPS_PER_DEGREE*360)/末级减速比

#define AXIS1_DRIVER_MODEL            OFF //    OFF，(见上文。) 步进驱动器型号。                                      <-Often
#define AXIS1_DRIVER_MICROSTEPS       OFF //    OFF, n。追踪时的细分模式。                                        <-Often
#define AXIS1_DRIVER_MICROSTEPS_GOTO  OFF //    OFF, n。GOTO 期间使用的细分模式。                                     Option
#define AXIS1_DRIVER_IHOLD            OFF //    OFF, n, (mA)。静止时的电流。OFF 时使用 IRUN/2.0                    Option
#define AXIS1_DRIVER_IRUN             OFF //    OFF, n, (mA)。追踪时的电流，适用于步进电机/驱动器等。    Option
#define AXIS1_DRIVER_IGOTO            OFF //    OFF, n, (mA)。GOTO 旋转时的电流。OFF 时与 IRUN 相同。                    Option
#define AXIS1_DRIVER_REVERSE          OFF //    OFF，ON 反转运动方向，或者通过反接线来纠正。   <-Often
#define AXIS1_DRIVER_STATUS           OFF //    OFF, TMC_SPI, HIGH, 或 LOW。轮询驱动器状态信息/故障检测。  Option

#define AXIS1_LIMIT_MIN              -180 //   -180, n。其中 n= -90..-270 (度)。赤道仪模式下的最小"时角"。      Adjust
                                          //         n。其中 n=-180..-360 (度)。经纬仪模式下的最小方位角。
#define AXIS1_LIMIT_MAX               180 //    180, n。其中 n=  90.. 270 (度)。赤道仪模式下的最大"时角"。      Adjust
                                          //         n。其中 n= 180.. 360 (度)。经纬仪模式下的最大方位角。

// AXIS2(轴2) 赤纬(DEC)/高度角(ALT)
// 参见 https://onstep.groups.io/g/main/wiki/Configuration-Mount#AXIS2
#define AXIS2_STEPS_PER_DEGREE    12800.0 //  12800, n。每度步数：                                          <-Req'd
                                          //         n = (步进电机步数 * 细分 * 总减速比)/360.0

#define AXIS2_DRIVER_MODEL            OFF //    OFF，(见上文。) 步进驱动器型号。                                      <-Often
#define AXIS2_DRIVER_MICROSTEPS       OFF //    OFF, n。追踪时的细分模式。                                        <-Often
#define AXIS2_DRIVER_MICROSTEPS_GOTO  OFF //    OFF, n。GOTO 期间使用的细分模式。                                     Option
#define AXIS2_DRIVER_IHOLD            OFF //    OFF, n, (mA)。静止时的电流。OFF 时使用 IRUN/2.0                    Option
#define AXIS2_DRIVER_IRUN             OFF //    OFF, n, (mA)。追踪时的电流，适用于步进电机/驱动器等。    Option
#define AXIS2_DRIVER_IGOTO            OFF //    OFF, n, (mA)。GOTO 旋转时的电流。OFF 时与 IRUN 相同。                    Option
#define AXIS2_DRIVER_POWER_DOWN       OFF //    OFF，ON 在运动停止10秒后或最后一次<=1x导星10分钟后断电。  Option
#define AXIS2_DRIVER_REVERSE          OFF //    OFF，ON 反转运动方向，或者通过反接线来纠正。   <-Often
#define AXIS2_DRIVER_STATUS           OFF //    OFF, TMC_SPI, HIGH, 或 LOW。轮询驱动器状态信息/故障检测。  Option
#define AXIS2_TANGENT_ARM             OFF //    OFF，ON 结合以下限位范围。使用 [Reset Home] 设置中心，使用 [Find Home] 返回中心 Infreq

#define AXIS2_LIMIT_MIN               -90 //    -90, n。其中 n=-90..0 (度)。允许的最小赤纬。                Infreq
#define AXIS2_LIMIT_MAX                90 //     90, n。其中 n=0..90 (度)。允许的最大赤纬。                 Infreq

// AXIS3(轴3) 旋转器/场旋器
// 参见 https://onstep.groups.io/g/main/wiki/Configuration-Rotator-and-Focusers#AXIS3
#define ROTATOR                       OFF //    OFF，ON 启用旋转器 (或用于经纬仪的消旋器)。              Option
#define AXIS3_STEPS_PER_DEGREE       64.0 //   64.0, n。旋转器/消旋器的每度步数。                    Adjust
                                          //         经纬仪消旋: n = (圆周像素数 * 2)/360，最小值
#define AXIS3_SLEW_RATE_DESIRED       1.0 //    1.0, n，(度/秒)。最大速度取决于处理器。                  Adjust

#define AXIS3_DRIVER_MODEL            OFF //    OFF, TMC2130, TMC5160。除这些型号外的所有驱动器均保持 OFF。         Option
#define AXIS3_DRIVER_MICROSTEPS       OFF //    OFF, n。追踪时的细分模式。                   用于 TMC2130, TMC5160。 Option
#define AXIS3_DRIVER_IHOLD            OFF //    OFF, n, (mA)。静止电流。OFF使用 IRUN/2.0。                   "       Option
#define AXIS3_DRIVER_IRUN             OFF //    OFF, n, (mA)。追踪电流，适用于步进/驱动器等。   "       Option
#define AXIS3_DRIVER_POWER_DOWN       OFF //    OFF，ON 在静止时切断电机电源。                                  Option
#define AXIS3_DRIVER_REVERSE          OFF //    OFF，ON 反转运动方向，或者通过反接线来纠正。    Option

#define AXIS3_LIMIT_MIN              -180 //   -180, n。其中 n=-360..0 (度)。允许的最小旋转器角度。             Infreq
#define AXIS3_LIMIT_MAX               180 //    180, n。其中 n=0..360 (度)。允许的最大旋转器角度。              Infreq

// AXIS4(轴4) 调焦器 1
// 参见 https://onstep.groups.io/g/main/wiki/Configuration-Rotator-and-Focusers#AXIS4
#define FOCUSER1                      OFF //    OFF，ON 启用此调焦器。                                               Option
#define AXIS4_STEPS_PER_MICRON        0.5 //    0.5, n。每微米步数。通过测试或其它方法算出这个值。      Adjust
#define AXIS4_SLEW_RATE_DESIRED       500 //    500, n。其中 n=200..5000 (um/s)。最大微米/秒。在直流模式下为最大功率百分比 %   Adjust

#define AXIS4_DRIVER_MODEL            OFF //    OFF, TMC2130, TMC5160。除这些型号外的所有驱动器均保持 OFF。         Option
#define AXIS4_DRIVER_MICROSTEPS       OFF //    OFF, n。追踪时的细分模式。                   用于 TMC2130, TMC5160。 Option
#define AXIS4_DRIVER_IHOLD            OFF //    OFF, n, (mA)。静止电流。OFF使用 IRUN/2.0。                   "       Option
#define AXIS4_DRIVER_IRUN             OFF //    OFF, n, (mA)。追踪电流，适用于步进/驱动器等。   "       Option
#define AXIS4_DRIVER_POWER_DOWN       OFF //    OFF，ON 在静止时切断电机电源。                                  Option
#define AXIS4_DRIVER_REVERSE          OFF //    OFF，ON 反转运动方向，或者通过反接线来纠正。    Option
#define AXIS4_DRIVER_DC_MODE          OFF //    OFF，ON 用于步进驱动器输出上的 PWM 直流电机控制。                   Option

#define AXIS4_LIMIT_MIN_RATE           50 //     50, n。其中 n=1..1000 (um/s)。最小微米/秒。在直流模式下为最小功率。  Adjust
#define AXIS4_LIMIT_MIN                 0 //      0, n。其中 n=0..500 (毫米)。允许的最小位置。               Adjust
#define AXIS4_LIMIT_MAX                50 //     50, n。其中 n=0..500 (毫米)。允许的最大位置。               Adjust

// AXIS5(轴5) 调焦器 2
// 参见 https://onstep.groups.io/g/main/wiki/Configuration-Rotator-and-Focusers#AXIS5
#define FOCUSER2                      OFF //    OFF，ON 启用此调焦器。                                               Option
#define AXIS5_STEPS_PER_MICRON        0.5 //    0.5, n。每微米步数。通过测试或其它方法算出这个值。      Adjust
#define AXIS5_SLEW_RATE_DESIRED       500 //    500, n。其中 n=200..5000 (um/s)。最大微米/秒。在直流模式下为最大功率百分比 %   Adjust

#define AXIS5_DRIVER_MODEL            OFF //    OFF, TMC2130, TMC5160。除这些型号外的所有驱动器均保持 OFF。         Option
#define AXIS5_DRIVER_MICROSTEPS       OFF //    OFF, n。追踪时的细分模式。                   用于 TMC2130, TMC5160。 Option
#define AXIS5_DRIVER_IHOLD            OFF //    OFF, n, (mA)。静止电流。OFF使用 IRUN/2.0。                   "       Option
#define AXIS5_DRIVER_IRUN             OFF //    OFF, n, (mA)。追踪电流，适用于步进/驱动器等。   "       Option
#define AXIS5_DRIVER_POWER_DOWN       OFF //    OFF，ON 在静止时切断电机电源。                                  Option
#define AXIS5_DRIVER_REVERSE          OFF //    OFF，ON 反转运动方向，或者通过反接线来纠正。    Option
#define AXIS5_DRIVER_DC_MODE          OFF //    OFF，ON 用于步进驱动器输出上的 PWM 直流电机控制。                   Option

#define AXIS5_LIMIT_MIN_RATE           50 //     50, n。其中 n=1..1000 (um/s)。最小微米/秒。在直流模式下为最小功率。  Adjust
#define AXIS5_LIMIT_MIN                 0 //      0, n。其中 n=0..500 (毫米)。允许的最小位置。               Adjust
#define AXIS5_LIMIT_MAX                50 //     50, n。其中 n=0..500 (毫米)。允许的最大位置。               Adjust

// 辅助功能控制 (AUXILIARY FEATURE CONTROL) ------------------------------ 参见 https://onstep.groups.io/g/main/wiki/6-ConfigurationMaster#AUXILIARY
// 有关其它不常用的 _PURPOSE 选项，请参见 Constants.h "various auxillary features" (各种辅助功能)
#define FEATURE_LIST_DS               OFF //    OFF，临时设置为 ON 以列出 ds18b20 和 ds2413 设备的序列号。     Adjust
#define FEATURE1_NAME          "FEATURE1" // "FE..", 受控功能的名称。                                        Adjust
#define FEATURE1_PURPOSE              OFF //    OFF, SWITCH(开关), ANALOG_OUT(模拟输出), DEW_HEATER(除露加热带) 等。                                     Option
#define FEATURE1_TEMP                 OFF //    OFF, DS1820, n。其中 n 是用于除露加热带(DEW_HEATER)温度的 ds18b20 序列号。        Adjust
#define FEATURE1_PIN                  OFF //    OFF, AUX, DS2413, n。其中 n 是 ds2413 的序列号 (gpio0) 或 n=0 到 255 (引脚号)。      Adjust
#define FEATURE1_DEFAULT_VALUE        OFF //    OFF, ON, n。其中 n=0..255 用于模拟输出 (ANALOG_OUT) 目的。                            Adjust
#define FEATURE2_NAME          "FEATURE2" // "FE..", 受控功能的名称。                                        Adjust
#define FEATURE2_PURPOSE              OFF //    OFF, SWITCH(开关), ANALOG_OUT(模拟输出), DEW_HEATER(除露加热带) 等。                                     Option
#define FEATURE2_TEMP                 OFF //    OFF, DS1820, n。其中 n 是用于除露加热带(DEW_HEATER)温度的 ds18b20 序列号。        Adjust
#define FEATURE2_PIN                  OFF //    OFF, AUX, CHAIN (ds2413 gpio1)，n。其中 n=0 到 255 (引脚号)。                   Adjust
#define FEATURE2_DEFAULT_VALUE        OFF //    OFF, ON, n。其中 n=0..255 用于模拟输出 (ANALOG_OUT) 目的。                            Adjust
#define FEATURE3_NAME          "FEATURE3" // "FE..", 受控功能的名称。                                        Adjust
#define FEATURE3_PURPOSE              OFF //    OFF, SWITCH(开关), ANALOG_OUT(模拟输出), DEW_HEATER(除露加热带) 等。                                     Option
#define FEATURE3_TEMP                 OFF //    OFF, DS1820, n。其中 n 是用于除露加热带(DEW_HEATER)温度的 ds18b20 序列号。        Adjust
#define FEATURE3_PIN                  OFF //    OFF, AUX, DS2413, n。其中 n 是 ds2413 的序列号 (gpio0) 或 n=0 到 255 (引脚号)。      Adjust
#define FEATURE3_DEFAULT_VALUE        OFF //    OFF, ON, n。其中 n=0..255 用于模拟输出 (ANALOG_OUT) 目的。                            Adjust
#define FEATURE4_NAME          "FEATURE4" // "FE..", 受控功能的名称。                                        Adjust
#define FEATURE4_PURPOSE              OFF //    OFF, SWITCH(开关), ANALOG_OUT(模拟输出), DEW_HEATER(除露加热带) 等。                                     Option
#define FEATURE4_TEMP                 OFF //    OFF, DS1820, n。其中 n 是用于除露加热带(DEW_HEATER)温度的 ds18b20 序列号。        Adjust
#define FEATURE4_PIN                  OFF //    OFF, AUX, CHAIN (ds2413 gpio1)，n。其中 n=0 到 255 (引脚号)。                   Adjust
#define FEATURE4_DEFAULT_VALUE        OFF //    OFF, ON, n。其中 n=0..255 用于模拟输出 (ANALOG_OUT) 目的。                            Adjust
#define FEATURE5_NAME          "FEATURE5" // "FE..", 受控功能的名称。                                        Adjust
#define FEATURE5_PURPOSE              OFF //    OFF, SWITCH(开关), ANALOG_OUT(模拟输出), DEW_HEATER(除露加热带) 等。                                     Option
#define FEATURE5_TEMP                 OFF //    OFF, DS1820, n。其中 n 是用于除露加热带(DEW_HEATER)温度的 ds18b20 序列号。        Adjust
#define FEATURE5_PIN                  OFF //    OFF, AUX, DS2413, n。其中 n 是 ds2413 的序列号 (gpio0) 或 n=0 到 255 (引脚号)。      Adjust
#define FEATURE5_DEFAULT_VALUE        OFF //    OFF, ON, n。其中 n=0..255 用于模拟输出 (ANALOG_OUT) 目的。                            Adjust
#define FEATURE6_NAME          "FEATURE6" // "FE..", 受控功能的名称。                                        Adjust
#define FEATURE6_PURPOSE              OFF //    OFF, SWITCH(开关), ANALOG_OUT(模拟输出), DEW_HEATER(除露加热带) 等。                                     Option
#define FEATURE6_TEMP                 OFF //    OFF, DS1820, n。其中 n 是用于除露加热带(DEW_HEATER)温度的 ds18b20 序列号。        Adjust
#define FEATURE6_PIN                  OFF //    OFF, AUX, CHAIN (ds2413 gpio1)，n。其中 n=0 到 255 (引脚号)。                   Adjust
#define FEATURE6_DEFAULT_VALUE        OFF //    OFF, ON, n。其中 n=0..255 用于模拟输出 (ANALOG_OUT) 目的。                            Adjust
#define FEATURE7_NAME          "FEATURE7" // "FE..", 受控功能的名称。                                        Adjust
#define FEATURE7_PURPOSE              OFF //    OFF, SWITCH(开关), ANALOG_OUT(模拟输出), DEW_HEATER(除露加热带) 等。                                     Option
#define FEATURE7_TEMP                 OFF //    OFF, DS1820, n。其中 n 是用于除露加热带(DEW_HEATER)温度的 ds18b20 序列号。        Adjust
#define FEATURE7_PIN                  OFF //    OFF, AUX, DS2413, n。其中 n 是 ds2413 的序列号 (gpio0) 或 n=0 到 255 (引脚号)。      Adjust
#define FEATURE7_DEFAULT_VALUE        OFF //    OFF, ON, n。其中 n=0..255 用于模拟输出 (ANALOG_OUT) 目的。                            Adjust
#define FEATURE8_NAME          "FEATURE8" // "FE..", 受控功能的名称。                                        Adjust
#define FEATURE8_PURPOSE              OFF //    OFF, SWITCH(开关), ANALOG_OUT(模拟输出), DEW_HEATER(除露加热带) 等。                                     Option
#define FEATURE8_TEMP                 OFF //    OFF, DS1820, n。其中 n 是用于除露加热带(DEW_HEATER)温度的 ds18b20 序列号。        Adjust
#define FEATURE8_PIN                  OFF //    OFF, AUX, CHAIN (ds2413 gpio1)，n。其中 n=0 到 255 (引脚号)。                   Adjust
#define FEATURE8_DEFAULT_VALUE        OFF //    OFF, ON, n。其中 n=0..255 用于模拟输出 (ANALOG_OUT) 目的。                            Adjust

// 用户配置就到这里！

// -------------------------------------------------------------------------------------------------------------------------
#define FileVersionConfig 4