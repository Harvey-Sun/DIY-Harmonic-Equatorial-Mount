/*
 * 标题       OnStep Smart Web Server (智能网页服务器)
 * 作者       Howard Dutton
 *
 * 版权所有 (C) 2016 至 2024 Howard Dutton
 *
 * 本程序为自由软件：您可以根据自由软件基金会发布的 GNU 通用公共许可证（GNU GPL）
 * 的条款重新分发和/或修改它，无论是许可证的第 3 版，还是（由您选择）任何更高版本。
 *
 * 本程序的发布是希望它能有用，但【没有任何担保】；
 * 甚至没有对【适销性】或【特定用途适用性】的暗示性担保。
 * 有关更多详细信息，请参阅 GNU 通用公共许可证。
 *
 * 您应该随本程序一起收到了一份 GNU 通用公共许可证的副本。
 * 如果没有，请参阅 <http://www.gnu.org/licenses/>。
 *
 * 修订历史：请参阅 GitHub
 *
 * 作者: Howard Dutton
 * http://www.stellarjourney.com
 * hjd1964@gmail.com
 *
 * 描述
 *
 * 用于 OnStep 和 OnStepX 的网页和 IP 服务器
 *
 */

#define Product "Smart Web Server"
#define FirmwareVersionMajor  "2"
#define FirmwareVersionMinor  "08"
#define FirmwareVersionPatch  "l"

// 请使用 Config.h 根据您的要求配置 SWS (智能网页服务器)

#include "src/Common.h"
NVS nv; // 非易失性存储 (EEPROM替代品)
#include "src/lib/tasks/OnTask.h"
#include "src/libApp/cmd/Cmd.h"
#include "src/libApp/bleGamepad/BleGamepad.h"
#include "src/libApp/encoders/Encoders.h"
#include "src/pages/Pages.h"
#include "src/libApp/status/Status.h"

#include "src/lib/ethernet/cmdServer/CmdServer.h"
#include "src/lib/ethernet/webServer/WebServer.h"
#include "src/lib/wifi/cmdServer/CmdServer.h"
#include "src/lib/wifi/webServer/WebServer.h"

#if DEBUG == PROFILER
  extern void profiler();
#endif

#if COMMAND_SERVER == PERSISTENT || COMMAND_SERVER == BOTH
  // 持久连接命令服务器 (通常用于 Planetarium 软件)
  CmdServer persistentCmdSvr1(9996, 10L*1000L, true);
  #if OPERATIONAL_MODE != ETHERNET_W5100
    CmdServer persistentCmdSvr2(9997, 10L*1000L, true);
  #endif
  CmdServer persistentCmdSvr3(9998, 10L*1000L, true);
#endif

#if COMMAND_SERVER == STANDARD || COMMAND_SERVER == BOTH
  // 标准命令服务器 (通常用于 App，端口 9999)
  CmdServer cmdSvr(9999, 1L*1000L);
#endif

void systemServices() {
  nv.poll(); // 轮询存储服务
}

void pollWebSvr() {
  www.handleClient(); // 处理网页客户端请求
}

void pollCmdSvr() {
  #if COMMAND_SERVER == PERSISTENT || COMMAND_SERVER == BOTH
    persistentCmdSvr1.handleClient(); Y; // Y 是 yield() 的宏，让出 CPU 时间
    #if OPERATIONAL_MODE != ETHERNET_W5100
      persistentCmdSvr2.handleClient(); Y;
    #endif
    persistentCmdSvr3.handleClient(); Y;
  #endif

  #if COMMAND_SERVER == STANDARD || COMMAND_SERVER == BOTH
    cmdSvr.handleClient();
    Y;
  #endif
}

void setup(void) {
  #if OPERATIONAL_MODE == WIFI
    WiFi.disconnect();
    WiFi.softAPdisconnect(true); // 断开之前的 WiFi 连接，重置状态
  #endif

  strcpy(firmwareVersion.str, FirmwareVersionMajor "." FirmwareVersionMinor FirmwareVersionPatch);

  // 启动调试串口 (如果开启了调试模式)
  if (DEBUG == ON || DEBUG == VERBOSE || DEBUG == PROFILER) SERIAL_DEBUG.begin(SERIAL_DEBUG_BAUD);
  delay(2000);

  #ifdef PIN_INIT
    PIN_INIT();
  #endif

  VF("MSG: SmartWebServer "); VL(firmwareVersion.str);
  VF("MSG: MCU = "); VF(MCU_STR); V(", ");
  VF("Pinmap = "); VLF(PINMAP_STR); // 打印引脚映射信息

  delay(2000);

  // 调用蓝牙手柄初始化
  #if (BLE_GAMEPAD == ON && ESP32)
    VLF("MSG: Init BLE");
    bleInit();
  #endif

  // 调用硬件特定初始化 (HAL)
  VLF("MSG: Init HAL");
  HAL_INIT();
  HAL_NV_INIT();
  #if LED_STATUS != OFF
    pinMode(LED_STATUS_PIN, OUTPUT);
  #endif

  // 尝试连接 OnStep 主控
  int serialSwap = OFF;
  serialSwap = SERIAL_SWAP;
  if (serialSwap == AUTO) serialSwap = AUTO_OFF;

  long serial_baud = SERIAL_BAUD;
  onStep.serialBegin(SERIAL_BAUD_DEFAULT, serialSwap); // 以默认波特率启动串口
  uint8_t tb = 1;
Again:
  VLF("MSG: Clearing serial channel");
  onStep.clearSerialChannel();

  // 寻找 OnStep 信号
  VLF("MSG: Attempting to contact OnStep");
  SERIAL_ONSTEP.print(":GVP#"); delay(100); // 发送查询产品命令
  String s = SERIAL_ONSTEP.readString();
  if (s == "On-Step#") {
    // 如果配置了不止一种波特率 (也就是想要高速通讯)
    if (SERIAL_BAUD != SERIAL_BAUD_DEFAULT) {
      // 获取 OnStep 支持的最快波特率，Mega2560 推荐返回 '4' 代表 19200
      SERIAL_ONSTEP.print(":GB#");
      delay(100);
      if (SERIAL_ONSTEP.available() != 1) { onStep.serialRecvFlush(); goto Again; }
      if (SERIAL_ONSTEP.read() == '4' && serial_baud > 19200) serial_baud = 19200;
      // 设置更快的波特率
      SERIAL_ONSTEP.print(onStep.highSpeedCommsStr(serial_baud)); delay(100);
      if (SERIAL_ONSTEP.available() != 1) { onStep.serialRecvFlush(); goto Again;
      }
      if (SERIAL_ONSTEP.read() != '1') goto Again;
    }

    // 一切就绪，只需更改本地波特率以匹配 OnStep
    onStep.serialBegin(serial_baud, serialSwap);
    #if DEBUG == REMOTE
      debugRemoteConnected = true;
    #endif
    
    VLF("MSG: OnStep Connection established"); // 连接建立成功
  } else {
    if (DEBUG == ON || DEBUG == VERBOSE) { VF("MSG: No valid reply found ("); V(s); VL(")");
    }
    #if LED_STATUS == ON
      digitalWrite(LED_STATUS_PIN, LED_STATUS_OFF_STATE);
    #endif
    // 没有收到有效回复，尝试切换波特率和/或交换串口引脚 (Swap)
    onStep.serialRecvFlush();
    tb++;
    // 循环尝试不同的组合：默认/交换引脚，默认/高速波特率
    if (tb == 16) { tb = 1; if (serialSwap == AUTO_OFF) serialSwap = AUTO_ON;
    else if (serialSwap == AUTO_ON) serialSwap = AUTO_OFF; }
    if (tb == 1) onStep.serialBegin(SERIAL_BAUD_DEFAULT, serialSwap);
    if (tb == 6) onStep.serialBegin(serial_baud, serialSwap);
    if (tb == 11) { if (SERIAL_BAUD_DEFAULT == 9600) { onStep.serialBegin(19200, serialSwap);
    } else tb = 15; }
    goto Again; // 重试
  }
  onStep.clearSerialChannel();
  // 系统服务
  // 添加系统服务任务，每 10ms 运行一次，因此写入 1KB NV 数据大约需要 10 秒
  VF("MSG: Setup, starting system services");
  VF(" task (rate 10ms priority 7)... ");
  if (tasks.add(10, 0, true, 7, systemServices, "SysSvcs")) { VL("success"); } else { VL("FAILED!");
  }

  // 准备 NV (非易失性存储)
  if (!nv.isKeyValid(INIT_NV_KEY)) {
    VF("MSG: NV, invalid key wipe "); V(nv.size);
    VLF(" bytes");
    if (nv.verify()) { VLF("MSG: NV, ready for reset to defaults");
    }
  } else { VLF("MSG: NV, correct key found");
  }

  // 获取命令和网页超时设置
  if (!nv.hasValidKey()) {
    nv.write(NV_TIMEOUT_CMD, (int16_t)cmdTimeout);
    nv.write(NV_TIMEOUT_WEB, (int16_t)webTimeout);
  }
  cmdTimeout = nv.readUI(NV_TIMEOUT_CMD);
  webTimeout = nv.readUI(NV_TIMEOUT_WEB);

  // 启动网络服务器 (WiFi 或 以太网)
  #if OPERATIONAL_MODE == WIFI
    VLF("MSG: Init WiFi");
    wifiManager.init();
  #else
    VLF("MSG: Init Ethernet");
    ethernetManager.init();
  #endif

  // 准备编码器
  VLF("MSG: Initialize Encoders");
  encoders.init();
  // 初始化完成，必要时写入 NV 密钥
  if (!nv.hasValidKey()) {
    nv.writeKey((uint32_t)INIT_NV_KEY);
    nv.wait();
    if (!nv.isKeyValid(INIT_NV_KEY)) { DLF("ERR: NV, failed to read back key!"); } else { VLF("MSG: NV, reset complete");
    }
  }

  VLF("MSG: Set webpage handlers"); // 设置网页路由处理程序
  www.on("/index.htm", handleRoot);
  www.on("/index-ajax-get.txt", indexAjaxGet);
  www.on("/index.txt", indexAjax);

  www.on("/mount.htm", handleMount);
  www.on("/mount-ajax-get.txt", mountAjaxGet);
  www.on("/mount-ajax.txt", mountAjax);
  www.on("/libraryHelp.htm", handleLibraryHelp);

  www.on("/rotator.htm", handleRotator);
  www.on("/rotator-ajax-get.txt", rotatorAjaxGet);
  www.on("/rotator-ajax.txt", rotatorAjax);

  www.on("/focuser.htm", handleFocuser);
  www.on("/focuser-ajax-get.txt", focuserAjaxGet);
  www.on("/focuser-ajax.txt", focuserAjax);

  www.on("/auxiliary.htm", handleAux);
  www.on("/auxiliary-ajax-get.txt", auxAjaxGet);
  www.on("/auxiliary-ajax.txt", auxAjax);
  #if ENCODERS == ON
    www.on("/enc.htm", handleEncoders);
    www.on("/enc-ajax-get.txt", encAjaxGet);
    www.on("/enc-ajax.txt", encAjax);
  #endif

  www.on("/net.htm", handleNetwork);

  www.on("/", handleRoot);
  
  www.onNotFound(handleNotFound);
  #if COMMAND_SERVER == PERSISTENT || COMMAND_SERVER == BOTH
    #if OPERATIONAL_MODE != ETHERNET_W5100
      VLF("MSG: Starting port 9996 cmd server");
      persistentCmdSvr3.begin();
      VLF("MSG: Starting port 9997 cmd server");
      persistentCmdSvr2.begin();
    #endif
    VLF("MSG: Starting port 9998 cmd server");
    persistentCmdSvr1.begin();
  #endif

  #if COMMAND_SERVER == STANDARD || COMMAND_SERVER == BOTH
    VLF("MSG: Starting port 9999 cmd server");
    cmdSvr.begin();
  #endif

  VLF("MSG: Starting port 80 web server");
  www.begin();

  // 留出时间让后台服务器启动
  delay(2000);
  // 最后一次清空串口通道
  onStep.clearSerialChannel();

  if (status.onStepFound) {
    status.update();
    delay(100);
  }

  state.init();

  VF("MSG: Setup, starting cmd channel polling"); // 启动命令通道轮询
  VF(" task (rate 5ms priority 2)... ");
  if (tasks.add(5, 0, true, 2, pollCmdSvr, "cmdPoll")) { VL("success"); } else { VL("FAILED!");
  }

  VF("MSG: Setup, starting web server polling"); // 启动网页服务器轮询
  VF(" task (rate 20ms priority 3)... ");
  if (tasks.add(20, 0, true, 3, pollWebSvr, "webPoll")) { VL("success"); } else { VL("FAILED!");
  }

  // 启动任务管理器调试事件
  #if DEBUG == PROFILER
    tasks.add(142, 0, true, 7, profiler, "Profilr");
  #endif

  #if (BLE_GAMEPAD == ON && ESP32)
    bleSetup();
  #endif

  VLF("MSG: SmartWebServer ready");
}

void loop(void) {
  #if (BLE_GAMEPAD == ON && ESP32)
    bleTimers(); Y;
    bleConnTest(); Y;
  #endif

  tasks.yield(); // 任务调度器让步
}