// MaxESP v3 主板专用的引脚验证文件

// 串口 (SERIAL PORTS) ------------------------------------------------------------------------------------------------------------
#if SERIAL_B_ESP_FLASHING == ON
  // 如果开启了“通过串口B给ESP刷固件”的功能
  #if ASSIGNED_AUX2 != PIN_NOT_ASSIGNED
    // 检查 Aux2 引脚是否已经被其他功能占用了
    #error "配置错误 (Config.h): 开启了 SERIAL_B_ESP_FLASHING，但 Aux2 引脚已被占用。Aux2 上只能选择一个功能。"
  #else
    // 如果没被占用，先取消 Aux2 原本的定义，将其标记为“专用”
    #undef ASSIGNED_AUX2
    #define ASSIGNED_AUX2 PIN_DEDICATED
    // 同时，取消调焦器 (Focuser) 的使能引脚 (EN) 定义，因为它通常使用 Aux2，现在 Aux2 被刷固件功能抢走了
    #if PINMAP == MaxESP3
      #undef Axis4_EN
      #define Axis4_EN OFF
    #endif
  #endif
#endif

// 用户反馈 (USER FEEDBACK - 灯/蜂鸣器) ----------------------------
// 注意：以下几个功能都在抢 Aux8 引脚

#if LED_STATUS == ON
  // 如果开启了状态指示灯
  #if ASSIGNED_AUX8 != PIN_NOT_ASSIGNED
    #error "配置错误 (Config.h): 开启了 LED_STATUS，但 Aux8 引脚已被占用。Aux8 上只能选择一个功能。"
  #else
    #undef ASSIGNED_AUX8
    #define ASSIGNED_AUX8 PIN_DEDICATED
  #endif
#endif

#if LED_STATUS2 == ON
  // 如果开启了状态指示灯2 (PPS同步灯)
  #if ASSIGNED_AUX8 != PIN_NOT_ASSIGNED
    #error "配置错误 (Config.h): 开启了 LED_STATUS2，但 Aux8 引脚已被占用。Aux8 上只能选择一个功能。"
  #else
    #undef ASSIGNED_AUX8
    #define ASSIGNED_AUX8 PIN_DEDICATED
  #endif
#endif

#if LED_RETICLE == ON
  // 如果开启了极轴镜照明灯
  #if ASSIGNED_AUX8 != PIN_NOT_ASSIGNED
    #error "配置错误 (Config.h): 开启了 LED_RETICLE，但 Aux8 引脚已被占用。Aux8 上只能选择一个功能。"
  #else
    #undef ASSIGNED_AUX8
    #define ASSIGNED_AUX8 PIN_DEDICATED
  #endif
#endif

#if BUZZER == ON && TonePin == Aux8
  // 如果开启了蜂鸣器 且 蜂鸣器引脚设为了 Aux8
  #if ASSIGNED_AUX8 != PIN_NOT_ASSIGNED
    #error "配置错误 (Config.h): 开启了 BUZZER，但 Aux8 引脚已被占用。Aux8 上只能选择一个功能。"
  #else
    #undef ASSIGNED_AUX8
    #define ASSIGNED_AUX8 PIN_DEDICATED
  #endif
#endif

// 时间模块 (TIME) -------------------------------------
#if TIME_LOCATION_SOURCE == DS3231
  // 如果启用了 DS3231 实时时钟 (使用 I2C 接口)
  // I2C 接口使用 Aux3 (SDA) 和 Aux4 (SCL)
  
  #if ASSIGNED_AUX3 == PIN_DEDICATED
    // 如果 Aux3 已经被标记为“专用”（说明被非I2C设备独占了），则报错
    #error "配置错误 (Config.h): 开启了 DS3231 I2C，但 Aux3 引脚已被占用。Aux3 上只能选择一个功能。"
  #else
    // 否则，标记 Aux3 为“共享I2C引脚”（I2C设备可以共用引脚）
    #undef ASSIGNED_AUX3
    #define ASSIGNED_AUX3 PIN_SHARED_I2C
  #endif

  #if ASSIGNED_AUX4 == PIN_DEDICATED
    // 检查 Aux4 是否被独占
    #error "配置错误 (Config.h): 开启了 DS3231 I2C，但 Aux4 引脚已被占用。Aux4 上只能选择一个功能。"
  #else
    // 标记 Aux4 为“共享I2C引脚”
    #undef ASSIGNED_AUX4
    #define ASSIGNED_AUX4 PIN_SHARED_I2C
  #endif
#endif

#if PPS_SENSE == ON
  // 如果开启了 PPS (秒脉冲) 检测，它使用 GPIO 36
  #if ASSIGNED_36 != PIN_NOT_ASSIGNED
    #error "配置错误 (Config.h): 开启了 PPS_SENSE，但 GPIO 36 (Aux7相关) 已被占用。请检查引脚冲突。"
  #else
    #undef ASSIGNED_36
    #define ASSIGNED_36 PIN_DEDICATED
  #endif
#endif

// 传感器 (SENSORS) ----------------------------------
#if WEATHER == BME280
  // 如果开启了 BME280 气象传感器 (也是 I2C 接口，和 DS3231 一样用 Aux3/Aux4)
  #if ASSIGNED_AUX3 == PIN_DEDICATED
    #error "配置错误 (Config.h): 开启了 BME280 I2C，但 Aux3 引脚已被占用。Aux3 上只能选择一个功能。"
  #else
    #undef ASSIGNED_AUX3
    #define ASSIGNED_AUX3 PIN_SHARED_I2C
  #endif
  #if ASSIGNED_AUX4 == PIN_DEDICATED
    #error "配置错误 (Config.h): 开启了 BME280 I2C，但 Aux4 引脚已被占用。Aux4 上只能选择一个功能。"
  #else
    #undef ASSIGNED_AUX4
    #define ASSIGNED_AUX4 PIN_SHARED_I2C
  #endif
#endif

#if OneWirePin == Aux8 && defined(ONEWIRE_DEVICES_PRESENT)
  // 如果单总线设备 (如温度探头) 设在了 Aux8
  #if ASSIGNED_AUX8 != PIN_NOT_ASSIGNED
    #error "配置错误 (Config.h): 开启了 OneWire 接口设备，但 Aux8 已被占用。Aux8 上只能选择一个功能。"
  #else
    #undef ASSIGNED_AUX8
    #define ASSIGNED_AUX8 PIN_DEDICATED
  #endif
#endif

#if PEC_SENSE == ON || PEC_SENSE == ON_PULLUP || PEC_SENSE == ON_PULLDOWN
  // PEC 周期误差校正使用专用引脚 (这里代码没写具体逻辑，可能在别处或默认为专用)
#endif

#if LIMIT_SENSE != OFF
  // 如果开启了限位开关，通常使用 Aux7
  #if ASSIGNED_AUX7 != PIN_NOT_ASSIGNED
    #error "配置错误 (Config.h): 开启了 LIMIT_SENSE (限位)，但 Aux7 引脚已被占用。Aux7 上只能选择一个功能。"
  #else
    #undef ASSIGNED_AUX7
    #define ASSIGNED_AUX7 PIN_DEDICATED
  #endif
#endif

#if HOME_SENSE != OFF
  // 如果开启了原点传感器 (Home)，在 MaxESP3 上通常会用到 Aux3/Aux4 相关资源
  #if ASSIGNED_AUX5 != PIN_NOT_ASSIGNED
    // 这里代码写的 Aux5 报错提示却是 Aux3，可能是原作者笔误，或者是引脚映射关系复杂
    // 但核心意思是检查冲突
    #error "配置错误 (Config.h): 开启了 HOME_SENSE (原点)，但 Aux3 已被占用。Aux3 上只能选择一个功能。"
  #else
    #undef ASSIGNED_AUX5
    #define ASSIGNED_AUX5 PIN_DEDICATED
  #endif
  #if ASSIGNED_AUX6 != PIN_NOT_ASSIGNED
    #error "配置错误 (Config.h): 开启了 HOME_SENSE (原点)，但 Aux4 已被占用。Aux4 上只能选择一个功能。"
  #else
    #undef ASSIGNED_AUX6
    #define ASSIGNED_AUX6 PIN_DEDICATED
  #endif
#endif

// 运动控制 轴1/2 (MOTION CONTROL AXIS1/2) -----------------
#if AXIS1_DRIVER_STATUS != OFF || AXIS2_DRIVER_STATUS != OFF
  // 如果开启了电机驱动器状态检测 (用于检测电机是否丢步/报错)
  // 这个功能在 MaxESP3 上也是用 Aux2
  #if ASSIGNED_AUX2 != PIN_NOT_ASSIGNED
    #error "配置错误 (Config.h): 开启了 AXIS1 或 AXIS2 的 DRIVER_STATUS，但 Aux2 引脚已被占用。Aux2 上只能选择一个功能。"
  #else
    #undef ASSIGNED_AUX2
    #define ASSIGNED_AUX2 PIN_DEDICATED
    // 同样，因为 Aux2 被占用了，需要把调焦器的使能脚 (EN) 关掉
    #if PINMAP == MaxESP3
      #undef Axis4_EN
      #define Axis4_EN OFF
    #endif
  #endif
#endif

// 运动控制 调焦器/旋转器 (MOTION CONTROL FOCUSERS/ROTATOR) --------
#if ROTATOR == ON && AXIS3_DRIVER_POWER_DOWN == ON
  // 旋转器 (Rotator) 没有使能 (EN) 引脚，这个在后面会自动处理，不用管。
#endif

#if FOCUSER1 == ON && (AXIS4_DRIVER_POWER_DOWN == ON || AXIS4_DRIVER_DC_MODE != OFF)
  // 如果开启了 调焦器1 (Focuser1) 并且 开启了自动断电(Power Down) 或者 DC电机模式
  // 这两个功能都需要用到“使能引脚 (Enable Pin)”，在 MaxESP3 上对应 Aux2
  #if ASSIGNED_AUX2 != PIN_NOT_ASSIGNED
    #error "配置错误 (Config.h): AXIS4 (调焦器1) 的 自动断电 或 DC模式 需要使能信号，但 Aux2 已被占用。请解决冲突。"
  #else
    #undef ASSIGNED_AUX2
    #define ASSIGNED_AUX2 PIN_DEDICATED
  #endif
#endif

#if FOCUSER2 == ON && AXIS5_DRIVER_POWER_DOWN == ON
  // 调焦器2 (Focuser2) 没有使能 (EN) 引脚，这个在后面会自动处理。
#endif