// -----------------------------------------------------------------------------------
// Locale Chinese ISO ISO639-1 language code "cn"
#pragma once

// General
#define L_UPLOAD "上传"
#define L_DOWNLOAD "下载"
#define L_ACCEPT "确认"
#define L_CONTINUE "继续"
#define L_ARE_YOU_SURE "你确定吗"
#define L_LOGOUT "退出"

#define L_UT "格林威治时间"
#define L_LST "恒星时" // LST = Local Sidereal Time，修正为恒星时，原意为“实时”不准确
#define L_LONG "经度" // Longitude，原翻译“长度”错误
#define L_LAT "纬度"  // Latitude，原翻译“宽度”错误
#define L_EAST "东"
#define L_WEST "西"
#define L_ZENITH "天顶"
#define L_NCP "北天极"
#define L_SCP "南天极"

#define L_NONE "无" // None，原翻译“没有”口语化，改为“无”更标准
#define L_AUTO "自动"
#define L_SET_LABEL "前往" // Goto，原翻译“放”错误，应为GOTO指令
#define L_OK "确定" // Ok
#define L_ON "开"
#define L_OFF "关"
#define L_START "开始"
#define L_DELTA "差值" // Delta，数学/物理术语，原翻译“对冲值”错误
#define L_ZERO "零"
#define L_SPAN "跨度"
#define L_POWER "电源" // Pwr，原翻译“功率”易歧义，此处通常指电源开关或状态
#define L_GUIDE "导星" // Guide，天文语境下通常指导星
#define L_CENTER "居中"
#define L_BLUE "蓝色"
#define L_GREEN "绿色"
#define L_RATE "速率"
#define L_RANGE "范围"
#define L_UNKNOWN "未知"
#define L_FAULT "故障"
#define L_POINT "点" // Point，原翻译“观点”错误，此处指校准点或数据点
#define L_COMPLETE "完成" // Complete
#define L_ACTIVE "活动" // Active，原翻译“积极的”不符合语境
#define L_INACTIVE "非活动" // Inactive
#define L_BEST "最佳"
#define L_ALTERNATE "备用"

// ---------------------------- Index ----------------------------
// date/time
#define L_TIME_BROWSER_UT "UT (网络浏览器)"

// weather
#define L_TEMPERATURE "温度"
#define L_AMBIENT_TEMPERATURE "环境温度"
#define L_PRESSURE "气压"
#define L_HUMIDITY "相对湿度"
#define L_DEW_POINT "露点温度"

// operation
#define L_TRACKING "跟踪"
#define L_SITE "地点" // Site，指观测地
#define L_COORDINATES "坐标"
#define L_CURRENT "当前位置" // Position，原翻译“电流”错误，变量名L_CURRENT易误导，但英文原文是Position
#define L_TARGET "目标"
#define L_ENCODER "编码器"
#define L_POLAR_ALIGN "极轴校准" // Polar Alignment，原翻译“极点对齐”

// pier side
#define L_PIER_SIDE "指向侧" // Pier Side，指赤道仪目前在桥墩的哪一侧
#define L_MERIDIAN_FLIPS "子午线翻转"
#define L_ALIGN_MESSAGE "赤道仪相对于" // Mount relative to，原翻译“相对于安装量”错误，Mount指赤道仪
#define L_MERIDIAN_FLIP_W_TO_E "子午线翻转, 从西到东"
#define L_MERIDIAN_FLIP_E_TO_W "子午线翻转, 从东到西"

#define L_OPERATIONS "操作" // Operations，原翻译“运转”

// parking
#define L_PARKING "停车中" // Parking (ing)
#define L_PARKED "已停车"
#define L_NOT_PARKED "未停车"
#define L_PARKING "停车中" // 重复定义
#define L_PARK_FAILED "停车失败"

// home
#define L_HOMING "归位中" // Homing
#define L_AT_HOME "在零位" // At Home，Home通常指零位/原点
#define L_SLEWING "正在GOTO" // Slewing，指望远镜高速移动寻找目标
#define L_PPS_SYNC "PPS同步" // PPS Sync (Pulse Per Second)，原翻译“同步卫星信号”不准确
#define L_HOME_REV "反向" // Reverse，指归位方向反向，原翻译“撤销”错误
#define L_HOME_OFFSET "与零位偏移:" // Home sense position
#define L_HOME_OFFSET_RANGE_AXIS1 "轴1, +/-" STR(HOME_OFFSET_RANGE_AXIS1) " 角秒"
#define L_HOME_OFFSET_RANGE_AXIS2 "轴2, +/-" STR(HOME_OFFSET_RANGE_AXIS2) " 角秒"
#define L_HOME_AUTO "启动时自动归位:"

// tracking
#define L_REFR_COMP_RA "RC"
#define L_REFR_COMP_BOTH "RCD"
#define L_FULL_COMP_RA "FC"
#define L_FULL_COMP_BOTH "FCD"
#define L_TRACKING "跟踪"
#define L_CURRENT_MAXRATE "当前最大速率"
#define L_DEFAULT_MAXRATE "默认最大速率" // Default
#define L_CURRENT_MAXSLEW "最大GOTO速度" // Maximum slew speed

#define L_FREQUENCY_ADJUST "频率调整"

// driver status
#define L_DRIVER "驱动器" // Driver，指电机驱动，原翻译“运转”错误
#define L_STATUS "状态"
#define L_STATE "状态"
#define L_COMMS_FAILURE "通讯失败" // Comms failure
#define L_STANDSTILL "静止"
#define L_OPEN_LOAD "开路" // Open load，电子术语
#define L_SHORT_GND "对地短路" // Short gnd
#define L_SHUTDOWN_OVER "关机/过热" // Shutdown over (temp/current)
#define L_PRE_WARNING "预警"
#define L_DRIVER_FAULT "故障" // Fault，原翻译“过错”指人为错误，此处指设备故障

// misc. errors/workload
#define L_INTERNAL_TEMP "控制器温度"
#define L_LAST_GENERAL_ERROR "上一个错误" // Last Error
#define L_WORKLOAD "负载" // Workload，指CPU/MCU负载
#define L_WIRELESS_SIGNAL_STRENGTH "无线信号强度"

// --------------------------- Control ---------------------------

#if DISPLAY_SPECIAL_CHARS == ON
  #define HOME_CH "&#x1F3E0;"
  #define ARROW_DR "&#x27A5;"
  #define ARROW_UR "&#x27A6;"
  #define ARROW_R2 "&#x27A4;"
  #define CAUTION_CH "&#9888;"
  #define CLOCK_CH "&#x1F565;"
  #define ARROW_LL "&lt;&lt;"
  #define ARROW_L "&lt;"
  #define ARROW_R "&gt;"
  #define ARROW_RR "&gt;&gt;"
  #define ARROW_DD "&lt;&lt;"
  #define ARROW_D "&lt;"
  #define ARROW_U "&gt;"
  #define ARROW_UU "&gt;&gt;"
  #define SIDEREAL_CH "&#9733;"
  #define LUNAR_CH "&#9790;"
  #define SOLAR_CH "&#9737;"
  #define KING_CH "&#9812;"
#else
  #define HOME_CH "H"
  #define ARROW_DR "-&gt;"
  #define ARROW_UR "-&gt;"
  #define ARROW_R2 "&gt;"
  #define CAUTION_CH "!"
  #define CLOCK_CH "T"
  #define ARROW_LL "&lt;&lt;"
  #define ARROW_L "&lt;"
  #define ARROW_R "&gt;"
  #define ARROW_RR "&gt;&gt;"
  #define ARROW_DD "&lt;&lt;"
  #define ARROW_D "&lt;"
  #define ARROW_U "&gt;"
  #define ARROW_UU "&gt;&gt;"
  #define SIDEREAL_CH "*"
  #define LUNAR_CH "("
  #define SOLAR_CH "O"
  #define KING_CH "K"
#endif

#define BUTTON_N "北"
#define BUTTON_S "南"
#define BUTTON_W "西"
#define BUTTON_E "东"
#define BUTTON_SYNC "@"

#define L_HINT_CLOCK "设置日期/时间"
#define L_HINT_CONTINUE_GOTO "继续GOTO (当在零位暂停时)"
#define L_HOME_PARK_TITLE "归位和停车"
#define L_FIND_HOME "寻找零位" // Find Home
#define L_RESET_HOME "设置零位 (重置)" // Home (Reset)
#define L_UNPARK "取消停车"
#define L_PARK "停车"
#define L_HINT_EMERGENCY_STOP "紧急停止!"
#define L_HINT_RATE_SIDEREAL "恒星速"
#define L_HINT_RATE_LUNAR "月球速"
#define L_HINT_RATE_SOLAR "太阳速"
#define L_HINT_RATE_KING "大气折射修正速" // King Rate，针对大气折射优化的恒星速，翻译为King或大气折射速，原翻译“特大价”完全错误
#define L_HINT_FOC_SEL "选择调焦器"
#define L_HINT_DEROTATE_ON "开启消旋" // Derotate On，原翻译“取消旋转”意思相反
#define L_HINT_ROTATOR_REV "反向旋转"
#define L_HINT_ROTATOR_PAR "移至赤道角" // Parallactic angle
#define L_HINT_DEROTATE_OFF "关闭消旋"

#define L_ALIGN "校准" // Align，指星点校准
#define L_AUX_FEATURES "辅助功能"

#define L_FOCUSER "调焦器"
#define L_ROTATOR "旋转器"
#define L_DEROTATOR "消旋器" // Derotator，场旋校正器，原翻译“反义词”严重错误
#define L_CAMERA_EXPOSURE "曝光"
#define L_CAMERA_DELAY "延迟"
#define L_CAMERA_COUNT "张数" // Count

// ---------------------------- Library --------------------------

#define L_CATALOG "星表" // Catalog，天文语境下为星表
#define L_CATALOGS "星表"
#define L_RECS_AVAIL "个记录可用"
#define L_NO_CATALOG "未选择星表"
#define L_CAT_DATA "数据 (已下载或待上传)"

#define L_CAT_EXAMPLE1 "字段:<br /> <pre>"
#define L_CAT_EXAMPLE2 "  第1行必须包含星表名称记录，<br />"
#define L_CAT_EXAMPLE3 "  由'$'开头后接最多10个字符组成。<br /><br />"
#define L_CAT_EXAMPLE4 "  天体名称最多11个字符。<br /><br />"
#define L_CAT_EXAMPLE5 "  Cat是类别代码，如下:<br />"
#define L_CAT_EXAMPLE6 "  赤经 RA (Jnow) 格式: 时, 分, 秒。<br />"
#define L_CAT_EXAMPLE7 "  赤纬 Dec (Jnow) 格式: +/- 度, 分, 秒。<br />"

#define L_CAT_NO_NAME "已选星表无名称。"
#define L_CAT_GET_NAME_FAIL "已选星表，获取名称失败。"
#define L_CAT_NO_CAT "未选择星表。"
#define L_CAT_NO_OBJECT "未选择天体。"
#define L_SELECTED "已选择"
#define L_CAT_DOWNLOAD_FAIL "下载失败, 数据无效。"
#define L_CAT_DOWNLOAD_SUCCESS "下载成功。"
#define L_CAT_DOWNLOAD_INDEX_FAIL "下载失败, 无法索引至星表。"
#define L_CAT_DATA_REMOVED "星表数据已删除"
#define L_CAT_UPLOAD_FAIL "上传失败, 星表名称无效。"
#define L_CAT_UPLOAD_INDEX_FAIL "上传失败, 索引星表名称记录时出错。"
#define L_CAT_DELETE_FAIL "上传失败, 删除星表名称记录时出错。"
#define L_CAT_WRITE_NAME_FAIL "上传失败, 写入星表名称记录时出错。"
#define L_CAT_UPLOAD_NO_NAME_FAIL "上传失败, 第1行必须包含星表名称。"
#define L_CAT_BAD_FORM "上传失败, 格式无效于行# "
#define L_CAT_UPLOAD_BAD_OBJECT_NAME "上传失败, 天体名称无效于行# "
#define L_CAT_BAD_CATEGORY "上传失败, 类别无效于行# "
#define L_CAT_BAD_RA "上传失败, RA格式无效于行# "
#define L_CAT_BAD_DEC "上传失败, Dec格式无效于行# "
#define L_CAT_UPLOAD_RA_FAIL "上传失败, 发送RA时于行# "
#define L_CAT_UPLOAD_DEC_FAIL "上传失败, 发送Dec时于行#"
#define L_CAT_UPLOAD_LINE_FAIL "上传失败, 发送行# "
#define L_CAT_UPLOAD_SUCCESS "上传成功"
#define L_CAT_UPLOAD_LINES_WRITTEN "行已写入"
#define L_CAT_UPLOAD_SELECT_FAIL "上传失败, 无法选择星表。"
#define L_CAT_UPLOAD_NO_CAT "上传失败, 未选择星表。"
#define L_CAT_CLEAR "清除星表"
#define L_CAT_CLEAR_LIB "清除天体库" // Clear Library，原翻译“清除图书馆”不当

// --------------------------- Encoders --------------------------

// general settings
#define L_ENC_SYNC_TO_ENC "同步 OnStep -> 编码器"
#define L_ENC_SYNC_TO_ONS "同步 编码器 -> OnStep"
#define L_ENC_ORIGIN_TO_ONS "设置 OnStep -> 编码器原点"
#define L_ENC_AUTO_SYNC "自动同步 编码器 -> OnStep"
#define L_ENC_CONF "编码器配置"
#define L_ENC_SET_TPD "步数/度" // Counts per degree
#define L_ENC_SYNC_THLD "自动同步阈值"
#define L_ENC_SYNC_THLD_UNITS "0 到 9999 角秒"
#define L_ENC_AUTO_RATE "OnStep速率控制" // OnStep rate control，原翻译“步速”不准确
#define L_NOW "现在"

// ------------------------------ PEC ----------------------------

#define L_PEC_STATUS "周期误差校正(PEC) 状态" // Periodic Error Correction，原翻译“定期错误”错误
#define L_PEC_CONTROL "PEC 控制"
#define L_PEC_PLAY "播放"
#define L_STOP "停止"
#define L_PEC_CLEAR "清除"
#define L_PEC_REC "录制" // Record
#define L_PEC_CLEAR_MESSAGE "清除仅擦除内存缓冲区而非EEPROM。录制期间修正值按3:1平均，除非已清除缓冲区，此时使用完整修正值。"
#define L_PEC_EEWRITE "写入EEPROM"
#define L_PEC_EEWRITE_MESSAGE "将PEC数据写入EEPROM以便重启后保留。写入数据可能需要几秒钟。"
#define L_DISABLED_MESSAGE "已禁用"
#define L_PEC_IDLE "空闲"
#define L_PEC_WAIT_PLAY "等待播放开始"
#define L_PEC_PLAYING "播放中"
#define L_PEC_WAIT_REC "录制等待索引信号"
#define L_PEC_RECORDING "录制中"
#define L_PEC_UNK "未知"
#define L_PEC_EEWRITING "正在写入EEPROM"

// --------------------------- Settings --------------------------

#define L_REFINE_POLAR_ALIGN "精细极轴校准" // Refine Polar Alignment
#define L_REFINE_PA "优化极轴" // Refine PA，原翻译“改变停车位”严重错误
#define L_REFINE_MESSAGE1 "设置并进行3星以上校准。GOTO到北天极或南天极附近的亮星，赤纬范围50到80度(N或S)。"
#define L_REFINE_MESSAGE2 "按下 [优化极轴] 按钮。使用赤道仪的机械调节旋钮将星星重新居中。"
#define L_REFINE_MESSAGE3 "可选：再次进行校准。"

#define L_PARK "停车"
#define L_SET_PARK "设置停车位"
#define L_SET_PARK_CURRENT_COORDS "将当前坐标设为停车位"

#define L_TRK_FASTER "更快" // Faster，原翻译“跟进”错误
#define L_TRK_SLOWER "更慢"
#define L_TRK_RESET "重置(默认)"
#define L_TRK_COMP "补偿跟踪速率 (指向模型/大气折射)"
#define L_TRK_FULL "全补偿"
#define L_TRK_REFR "仅折射"
#define L_TRK_DUAL "双轴"
#define L_TRK_SINGLE "单轴"

#define L_BUZZER "GOTO到位蜂鸣器" // Goto Alert, Buzzer

#define L_MERIDIAN_FLIP_AUTO "限位处自动子午线翻转"
#define L_MERIDIAN_FLIP_NOW "立即翻转"
#define L_MERIDIAN_FLIP_PAUSE "翻转，在零位暂停"
#define L_MERIDIAN_FLIP_PPS "翻转，优先指向侧" // Preferred Pier Side

#define L_ORIENTATION_AUTO "方向改变"
#define L_ORIENTATION_CHANGE_NOW L_MERIDIAN_FLIP_NOW
#define L_ORIENTATION_CHANGE_PAUSE "翻转，在零位暂停"
#define L_ORIENTATION_CHANGE_PPS "翻转，优先方向"

// ------------------------ Configuration ------------------------

#define L_BASIC_SET_TITLE "基本:"

#define L_MAX_SLEW_SPEED "最大GOTO速度" // Slew通常指GOTO时的快速移动

// goto Rate
#define L_VSLOW "极慢"
#define L_SLOW "慢"
#define L_NORMAL "正常"
#define L_FAST "快"
#define L_VFAST "极快"

// backlash
#define L_BACKLASH "回差(间隙)"
#define L_BACKLASH_RANGE_AXIS1 "轴1, 0到3600 角秒"
#define L_BACKLASH_RANGE_AXIS2 "轴2, 0到3600 角秒"
#define L_BACKLASH_RANGE_AXIS3UP "回差, 0到32767 步"

// deadband/TCF
#define L_DEADBAND "TCF 死区"
#define L_TCF_COEF "TCF 系数"
#define L_TCF_COEF_EN "TCF 启用"
#define L_DEADBAND_RANGE_AXIS4UP "TCF死区，1至32767 步"
#define L_TCF_COEF_RANGE_AXIS4UP "TCF系数，+/- 999.0 微米/摄氏度"
#define L_TCF_COEF_EN_AXIS4UP "TCF启用，0假或1真"

// limits
#define L_LIMITS_TITLE "地平线和天顶限制"
#define L_LIMITS_RANGE_HORIZON "地平线，最小高度 +/- 30度"
#define L_LIMITS_RANGE_OVERHEAD "天顶，最大高度 60到90度"
#define L_LIMITS_RANGE_MERIDIAN_E "越过子午线，东侧 +/- 270度"
#define L_LIMITS_RANGE_MERIDIAN_W "越过子午线，西侧 +/- 270度"

// location
#define L_LOCATION_TITLE "时间地点"
#define L_LOCATION_LONG "经度，度 分 秒。+/-180，西为+"
#define L_LOCATION_LAT "纬度，度 分 秒。+/-90，北为+"
#define L_LOCATION_RANGE_UTC_OFFSET "UTC 偏移，小时和分钟。-14到+12"
#define L_LOCATION_MESSAGE_UTC_OFFSET "标准时间 (非夏令时!)"

// advanced configuration
#define L_REVERT "恢复默认值"
#define L_ADV_SET_TITLE "高级"
#define L_ADV_SET_HEADER_MSG "除非另有说明，以下更改将在重启OnStep后生效:"
#define L_ADV_MOUNT_TYPE " 选择 1 德式赤道仪(GEM), 2 叉式(Fork), 3 经纬仪(Alt/Azm)"
#define L_ADV_BOOL "0 假 或 1 真"

#define L_ADV_SET_SPWR "蜗杆旋转一周的步数"
#define L_ADV_SET_SPD "每度步数"
#define L_ADV_SET_SPM "每微米步数"
#define L_ADV_SET_REV "反向，0假或1真"
#define L_ADV_SET_MIN "最小位置"
#define L_ADV_SET_MAX "最大位置"

#define L_ADV_SET_SPECIAL "步进/方向驱动器：<br /><br />值为-1相当于Config.h OFF。 <i>请谨慎修改！</i>"
#define L_ADV_SET_IMMEDIATE "伺服驱动器：<br /><br />设置立即生效，无需重启。 <i>请谨慎修改！</i>"
#define L_ADV_SET_TRAK_us "跟踪细分(Microsteps)"
#define L_ADV_SET_GOTO_us "GOTO细分"
#define L_ADV_SET_HOLD_ma "mA 保持电流"
#define L_ADV_SET_TRAK_ma "mA 跟踪电流"
#define L_ADV_SET_SLEW_ma "mA GOTO电流"
#define L_ADV_SET_P "跟踪 P (比例)"
#define L_ADV_SET_I "跟踪 I (积分)"
#define L_ADV_SET_D "跟踪 D (微分)"
#define L_ADV_SET_P_GOTO "GOTO P (比例)"
#define L_ADV_SET_I_GOTO "GOTO I (积分)"
#define L_ADV_SET_D_GOTO "GOTO D (微分)"

#define L_ADV_ENABLE "启用高级配置"
#define L_ADV_DISABLE "禁用高级配置 (重启后恢复默认)"
#define L_ADV_SET_AXIS_NO_EDIT "此轴的编辑已被禁用。"
#define L_ADV_SET_NO_EDIT "所有轴的编辑已被禁用。可能未启用此功能，或者需要重启OnStep。"
#define L_ADV_SET_FOOTER_MSG0 "大多数轴设置更改需要重启才能生效。"
#define L_ADV_SET_FOOTER_MSG1 "按下 [上传] 后，请注意上方变化，因为无效参数可能导致上传被拒绝。"
#define L_ADV_SET_FOOTER_MSG2 "选择 [恢复默认] 后，该组在重启OnStep前不可编辑。"
#define L_ADV_SET_FOOTER_MSG3 "如果IRUN设置为非Config.h默认值，IHOLD将被设为50% (一半)。"
#define L_ADV_SET_FOOTER_MSG4 "更改轴1的蜗杆步数和每度步数比率可能会损坏库NV存储区，如有需要请先备份星表。"

// reset control
#define L_RESET_TITLE "OnStep 重启控制:"
#define L_WIPE_RESET "清除所有设置并重启 OnStep"
#define L_RESET "重启 OnStep"
#define L_RESET_FWU "重启 OnStep 以上传固件"
#define L_RESET_MSG1 "OnStep 必须处于“零位”或“停车”状态，否则重启请求将被忽略。"

// ----------------------------  WiFi ----------------------------

#define L_NETWORK_PERFORMANCE "性能和兼容性"
#define L_NETWORK_CMD_TO "命令通道串口读取超时:"
#define L_NETWORK_WWW_TO "Web通道串口读取超时："

#define L_NETWORK_STA_TITLE "终端模式 Station (连接到路由器)" // Station mode
#define L_NETWORK_PWD "密码"
#define L_NETWORK_PWD_MSG "至少8个字符。"
#define L_NETWORK_EN_DHCP "启用DHCP"
#define L_NETWORK_EN_DHCP_MSG "注意：如果启用DHCP，上述地址将被忽略"
#define L_NETWORK_EN_STA "启用终端模式 (Station)"
#define L_IP_ADDRESS "IP地址"
#define L_GATEWAY "网关"
#define L_SUBNET "子网掩码"
#define L_NETWORK_CHA "信道"
#define L_NETWORK_EN_AP_MODE "启用热点模式 (AP)"

#define L_NETWORK_CONFIG_SECURITY "网络配置安全"

#define L_NETWORK_AP "热点模式 (AP)"

#define L_NETWORK_RESTART_MSG "您必须手动<u>重启</u>才能使更改生效。"

#define L_NETWORK_TITLE "输入密码以更改 WiFi 配置:"

#define L_NETWORK_ADVICE1 "设置：<br/> <br/>"
#define L_NETWORK_ADVICE2 "建议仅启用 <i>终端模式</i> <b>或</b> <i>热点模式</i> 之一。同时启用可能导致性能问题！ <BR/>"
#define L_NETWORK_ADVICE3 "如果希望 OnStep 连接到家庭WiFi，请仅启用终端模式并 "
#define L_NETWORK_ADVICE4 "禁用热点模式。如果 OnStep 无法连接到家庭WiFi "
#define L_NETWORK_ADVICE5 "热点模式会自动重新激活。这在野外无WiFi环境 "
#define L_NETWORK_ADVICE6 "或防止被锁定时很有用。<br/> <br/>"
#define L_NETWORK_ADVICE7 "如果因WiFi问题无法连接，可使用Arduino IDE重置ESP闪存（恢复默认设置）"
#define L_NETWORK_ADVICE8 "在Arduino工具菜单中，将 Erase Flash 选项改为 'All Flash Contents' "
#define L_NETWORK_ADVICE9 "然后再次上传WiFi固件。"

// --------------------------- Constants -------------------------

#define L_DOWN_TITLE "未连接到 OnStep 串口!"
#define L_DOWN_MESSAGE1 "如果是刚刚重启OnStep，请等待几秒钟。"
#define L_DOWN_MESSAGE2 "其他可能原因：<br /> <ul>"
#define L_DOWN_MESSAGE3 "波特率不匹配，请尝试断电重开。"
#define L_DOWN_MESSAGE4 "SWS 波特率配置错误。"
#define L_DOWN_MESSAGE5 "OnStep 波特率配置错误。"
#define L_DOWN_MESSAGE6 "COM端口接线错误。"
#define L_DOWN_MESSAGE7 "电源接线错误。OnStep的MCU与附加模块的MCU必须共地(GND)。"

#define L_CONTROLLER "控制器" // Controller，原翻译“状态”不准确
#define L_PAGE_MOUNT "赤道仪" // Mount，原翻译“控制”，此处指赤道仪页面
#define L_PAGE_AUXILIARY "辅助"
#define L_PAGE_LIBRARY "天体库" // Library
#define L_PAGE_ENCODERS "编码器"
#define L_PAGE_PEC "PEC" // 定期校正->PEC
#define L_SETTINGS "设置"
#define L_CONTROLS "控制"
#define L_PAGE_CONFIG "配置"
#define L_PAGE_NETWORK "网络"

// ------------------------- MountStatus -------------------------

// general (background) errors
#define L_GE_NONE "无"
#define L_GE_MOTOR_FAULT "电机/驱动器故障"
#define L_GE_ALT_MIN "低于地平线限制" 
#define L_GE_LIMIT_SENSE "限位开关触发"
#define L_GE_DEC "超出赤纬限制"
#define L_GE_AZM "超出方位限制"
#define L_GE_UNDER_POLE "超出极下限制"
#define L_GE_MERIDIAN "超出子午线限制"
#define L_GE_SYNC "超出同步安全限制"
#define L_GE_PARK "停车失败"
#define L_GE_GOTO_SYNC "GOTO 同步失败"
#define L_GE_UNSPECIFIED "未知错误"
#define L_GE_ALT_MAX "超出天顶限制"
#define L_GE_WEATHER_INIT "天气传感器初始化失败"
#define L_GE_SITE_INIT "时间或地点未更新"
#define L_GE_NV_INIT "NV/EEPROM 初始化错误"
#define L_GE_OTHER "未知错误，代码"

// command errors
#define L_CE_NONE "无错误"
#define L_CE_0 "回复 0"
#define L_CE_CMD_UNKNOWN "命令未知"
#define L_CE_REPLY_UNKNOWN "回复无效"
#define L_CE_PARAM_RANGE "参数超出范围"
#define L_CE_PARAM_FORM "参数格式无效"
#define L_CE_ALIGN_FAIL "校准失败"
#define L_CE_ALIGN_NOT_ACTIVE "校准未激活"
#define L_CE_NOT_PARKED_OR_AT_HOME "未停车或不在零位"
#define L_CE_PARKED "已停车"
#define L_CE_PARK_FAILED "停车失败"
#define L_CE_NOT_PARKED "未停车"
#define L_CE_NO_PARK_POSITION_SET "未设置停车位"
#define L_CE_GOTO_FAIL "GOTO 失败"
#define L_CE_LIBRARY_FULL "库已满"
#define L_CE_GOTO_ERR_BELOW_HORIZON "GOTO 低于地平线"
#define L_CE_GOTO_ERR_ABOVE_OVERHEAD "GOTO 超出天顶"
#define L_CE_SLEW_ERR_IN_STANDBY "待机中无法移动"
#define L_CE_SLEW_ERR_IN_PARK "停车中无法移动"
#define L_CE_GOTO_ERR_GOTO "已在 GOTO 中"
#define L_CE_GOTO_ERR_OUTSIDE_LIMITS "GOTO 超出限制"
#define L_CE_SLEW_ERR_HARDWARE_FAULT "硬件故障"
#define L_CE_MOUNT_IN_MOTION "赤道仪运动中" // Mount in motion
#define L_CE_GOTO_ERR_UNSPECIFIED "其他"
#define L_CE_UNK "未知"