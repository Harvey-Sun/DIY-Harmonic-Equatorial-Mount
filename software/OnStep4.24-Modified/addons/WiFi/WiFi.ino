/*
 * 标题          OnStep WiFi Server
 * 作者          Howard Dutton
 *
 * 版权所有 (C) 2016 至 2021 Howard Dutton
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
 * *
 * 修订历史：请参阅 GitHub
 *
 *
 * 作者: Howard Dutton
 * http://www.stellarjourney.com
 * hjd1964@gmail.com
 *
 * 描述
 *
 * ESP8266 OnStep 控制
 *
 */

#define Product "WiFi Server"
#define FirmwareDate          __DATE__
#define FirmwareTime          __TIME__
#define FirmwareVersionMajor  "2"
#define FirmwareVersionMinor  "1"
#define FirmwareVersionPatch  "v"

#define Version FirmwareVersionMajor "." FirmwareVersionMinor FirmwareVersionPatch

// 启用传递给 OnStep 的调试和/或状态消息，以便使用其调试设施进行显示
// 默认 "DEBUG OFF"，使用 "DEBUG ON" 仅显示后台错误，使用 "DEBUG VERBOSE" 显示所有错误和状态消息
#define DEBUG OFF

#include <limits.h>

#ifdef ESP32
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WebServer.h>
  #include <WiFiAP.h>
#else
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266WiFiAP.h>
#endif
[cite_start]#define Ser Serial  // 默认=Serial，这是连接 OnStep 的硬件串口 [cite: 146]

#include <EEPROM.h>
#include "EEProm.h"

#include "Constants.h"
#include "Locales.h"
#include "Config.h"
#define DISPLAY_RESET_CONTROLS OFF
#if AXIS1_ENC > 0 && AXIS2_ENC > 0
  #define ENCODERS ON
#endif
#include "Locale.h"
#include "Globals.h"

// 
// 以下设置仅用于初始化，之后它们将存储在 EEPROM 中并从中调用，
[cite_start]// 必须在 Web 界面中更改，或者通过复位（再次初始化）来更改，如 Config.h 注释中所述 [cite: 153]
#define TIMEOUT_WEB 60
#define TIMEOUT_CMD 60

int webTimeout=TIMEOUT_WEB;
int cmdTimeout=TIMEOUT_CMD;

#include "Encoders.h"
#if ENCODERS == ON
Encoders encoders;
#endif

#ifndef LEGACY_TRANSMIT_ON
  // 辅助发送网页数据的宏，分块传输 (chunked)
  #define sendHtmlStart() server.setContentLength(CONTENT_LENGTH_UNKNOWN); server.sendHeader("Cache-Control","no-cache"); server.send(200, "text/html", String());
  #define sendHtml(x) server.sendContent(x); x=""
  #define sendHtmlDone(x) server.sendContent("");
#else
  // 辅助发送网页数据的宏，普通方法
  #define sendHtmlStart()
  #define sendHtml(x)
  #define sendHtmlDone(x) server.send(200, "text/html", x)
#endif

#define Default_Password "password"
char masterPassword[40]=Default_Password;
bool accessPointEnabled=true;
bool stationEnabled=false;
bool stationDhcpEnabled=true;

char wifi_sta_ssid[40]="";
char wifi_sta_pwd[40]="";

IPAddress wifi_sta_ip = IPAddress(192,168,0,1);
IPAddress wifi_sta_gw = IPAddress(192,168,0,1);
IPAddress wifi_sta_sn = IPAddress(255,255,255,0);

char wifi_ap_ssid[40]="ONSTEP";
char wifi_ap_pwd[40]="password";
byte wifi_ap_ch=7;

IPAddress wifi_ap_ip = IPAddress(192,168,0,1);
IPAddress wifi_ap_gw = IPAddress(192,168,0,1);
IPAddress wifi_ap_sn = IPAddress(255,255,255,0);

#ifdef ESP32
  WebServer server(80);
#else
  ESP8266WebServer server(80);
#endif

#if STANDARD_COMMAND_CHANNEL == ON
  WiFiServer cmdSvr(9999);
  WiFiClient cmdSvrClient;
#endif

#if PERSISTENT_COMMAND_CHANNEL == ON
  WiFiServer persistentCmdSvr(9998);
  WiFiClient persistentCmdSvrClient;
#endif

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

#include "Accessories.h"
#include "MountStatus.h"

void setup(void){
  WiFi.disconnect();
  WiFi.softAPdisconnect(true);

#if LED_STATUS != OFF
  pinMode(LED_STATUS,OUTPUT);
#endif
  nv.init();

  // EEPROM 初始化
  if (nv.readInt(EE_KEY_HIGH) != 8266 || nv.readInt(EE_KEY_LOW) != 0) {
    nv.writeInt(EE_KEY_HIGH,8266);
    nv.writeInt(EE_KEY_LOW,0);

    nv.writeInt(EE_AP_EN,(int)accessPointEnabled);
    nv.writeInt(EE_STA_EN,(int)stationEnabled);
    nv.writeInt(EE_DHCP_EN,(int)stationDhcpEnabled);

    nv.writeInt(EE_TIMEOUT_WEB,(int)webTimeout);
    nv.writeInt(EE_TIMEOUT_CMD,(int)cmdTimeout);

    nv.writeString(EE_STA_SSID,wifi_sta_ssid);
    nv.writeString(EE_STA_PWD,wifi_sta_pwd);
    nv.writeString(EE_PASSWORD,masterPassword);
    for (int i=0;i<4;i++) nv.write(EE_STA_IP+i,wifi_sta_ip[i]);
    for (int i=0;i<4;i++) nv.write(EE_STA_GW+i,wifi_sta_gw[i]);
    for (int i=0;i<4;i++) nv.write(EE_STA_SN+i,wifi_sta_sn[i]);

    nv.writeString(EE_AP_SSID,wifi_ap_ssid);
    nv.writeString(EE_AP_PWD,wifi_ap_pwd);
    nv.writeInt(EE_AP_CH,(int)wifi_ap_ch);
    for (int i=0;i<4;i++) nv.write(EE_AP_IP+i,wifi_ap_ip[i]);
    for (int i=0;i<4;i++) nv.write(EE_AP_GW+i,wifi_ap_gw[i]);
    for (int i=0;i<4;i++) nv.write(EE_AP_SN+i,wifi_ap_sn[i]);
#if ENCODERS == ON
    nv.writeLong(EE_ENC_A1_DIFF_TO,AXIS1_ENC_DIFF_LIMIT_TO);
    nv.writeLong(EE_ENC_A2_DIFF_TO,AXIS2_ENC_DIFF_LIMIT_TO);
    nv.writeLong(EE_ENC_RC_STA,20);     // 编码器短期平均采样数
    nv.writeLong(EE_ENC_RC_LTA,200);    // 编码器长期平均采样数
    nv.writeLong(EE_ENC_RC_RCOMP,0);    // 编码器速率补偿
    nv.writeLong(EE_ENC_RC_INTP_P,1);   // 插值相位
    nv.writeLong(EE_ENC_RC_INTP_M,0);   // 插值幅度
    nv.writeLong(EE_ENC_RC_PROP,10);    // 比例 (P)
    nv.writeLong(EE_ENC_MIN_GUIDE,100); // 最小导星持续时间
    nv.writeLong(EE_ENC_A1_ZERO,0);     // 轴1 绝对编码器零点
    nv.writeLong(EE_ENC_A2_ZERO,0);     [cite_start]// 轴2 绝对编码器零点 [cite: 170]
#endif

    nv.commit();
  }
  
  accessPointEnabled=nv.readInt(EE_AP_EN);
  stationEnabled=nv.readInt(EE_STA_EN);
  if (!accessPointEnabled && !stationEnabled) accessPointEnabled=true;
  stationDhcpEnabled=nv.readInt(EE_DHCP_EN);

  webTimeout=nv.readInt(EE_TIMEOUT_WEB);
  cmdTimeout=nv.readInt(EE_TIMEOUT_CMD);
  if (cmdTimeout > 100) cmdTimeout=30;

#if ENCODERS == ON
  Axis1EncDiffTo=nv.readLong(EE_ENC_A1_DIFF_TO);
  Axis2EncDiffTo=nv.readLong(EE_ENC_A2_DIFF_TO);
  #if AXIS1_ENC_RATE_CONTROL == ON
    Axis1EncStaSamples=nv.readLong(EE_ENC_RC_STA);
    Axis1EncLtaSamples=nv.readLong(EE_ENC_RC_LTA);
    long l=nv.readLong(EE_ENC_RC_RCOMP); axis1EncRateComp=(float)l/1000000.0;
    #if AXIS1_ENC_INTPOL_COS == ON
      Axis1EncIntPolPhase=nv.readLong(EE_ENC_RC_INTP_P);
      Axis1EncIntPolMag=nv.readLong(EE_ENC_RC_INTP_M);
    #endif
    Axis1EncProp=nv.readLong(EE_ENC_RC_PROP);
    Axis1EncMinGuide=nv.readLong(EE_ENC_MIN_GUIDE);
  #endif
#endif

  nv.readString(EE_STA_SSID,wifi_sta_ssid);
  nv.readString(EE_STA_PWD,wifi_sta_pwd);
  nv.readString(EE_PASSWORD,masterPassword);
  for (int i=0;i<4;i++) wifi_sta_ip[i]=nv.read(EE_STA_IP+i);
  for (int i=0;i<4;i++) wifi_sta_gw[i]=nv.read(EE_STA_GW+i);
  for (int i=0;i<4;i++) wifi_sta_sn[i]=nv.read(EE_STA_SN+i);

  nv.readString(EE_AP_SSID,wifi_ap_ssid);
  nv.readString(EE_AP_PWD,wifi_ap_pwd);
  wifi_ap_ch=nv.readInt(EE_AP_CH);
  for (int i=0;i<4;i++) wifi_ap_ip[i]=nv.read(EE_AP_IP+i);
  for (int i=0;i<4;i++) wifi_ap_gw[i]=nv.read(EE_AP_GW+i);
  for (int i=0;i<4;i++) wifi_ap_sn[i]=nv.read(EE_AP_SN+i);  

  int serialSwap=SERIAL_SWAP;
  if (serialSwap == AUTO) serialSwap = AUTO_OFF;

  long serial_baud = SERIAL_BAUD;
  serialBegin(SERIAL_BAUD_DEFAULT,serialSwap);
  byte tb=1;

Again:
  clearSerialChannel();
  [cite_start]// 寻找 On-Step [cite: 178]
  Ser.print(":GVP#"); delay(100);
  // 确保响应正常
  if (Ser.available() == 8 && 
      Ser.read() == 'O' && Ser.read() == 'n' && Ser.read() == '-' && Ser.read() == 'S' &&
      Ser.read() == 't' && Ser.read() == 'e' && Ser.read() == 'p' && Ser.read() == '#') {

    // 检查最快波特率
    Ser.print(":GB#");
    delay(100);
    if (Ser.available() != 1) { serialRecvFlush(); goto Again; }
    if (Ser.read() == '4' && serial_baud > 19200) serial_baud = 19200;
    // Mega2560 返回 '4' 推荐使用 19200 波特率

    // 设置最快波特率
    Ser.print(HighSpeedCommsStr(serial_baud));
    delay(100);
    if (Ser.available() != 1) { serialRecvFlush(); goto Again; }
    if (Ser.read() != '1') goto Again;
    // 设置完成，只需更改波特率以匹配 OnStep
    serialBegin(serial_baud,serialSwap);
    VLF("WEM: WiFi Connection established");
  } else {
#if LED_STATUS != OFF
    digitalWrite(LED_STATUS,LED_STATUS_OFF_STATE);
#endif
    [cite_start]// 没有收到回复，切换波特率和/或交换端口 [cite: 184]
    serialRecvFlush();
    tb++;
    if (tb == 16) { tb=1; if (serialSwap == AUTO_OFF) serialSwap=AUTO_ON; else if (serialSwap == AUTO_ON) serialSwap=AUTO_OFF;
    }
    if (tb == 1) serialBegin(SERIAL_BAUD_DEFAULT,serialSwap);
    if (tb == 6) serialBegin(serial_baud,serialSwap);
    if (tb == 11) if (SERIAL_BAUD_DEFAULT == 9600) serialBegin(19200,serialSwap); else tb=15;
    goto Again;
  }
  
  // say hello
  VF("WEM: WiFi Addon "); V(FirmwareVersionMajor); V("."); V(FirmwareVersionMinor); VL(FirmwareVersionPatch);
  VF("WEM: MCU = ");
  VLF(MCU_STR);

  VF("WEM: Access Point Enabled  = "); VL(accessPointEnabled);
  VF("WEM: Station Enabled       = ");
  VL(stationEnabled);
  VF("WEM: Station DHCP Enabled  = "); VL(stationDhcpEnabled);

  VF("WEM: Web Channel Timeout ms= "); VL(webTimeout);
  VF("WEM: Cmd Channel Timeout ms= "); VL(cmdTimeout);

  VF("WEM: WiFi STA SSID   = "); VL(wifi_sta_ssid);
  VF("WEM: WiFi STA PWD    = "); VL(wifi_sta_pwd);
  VF("WEM: WiFi STA IP     = ");
  VL(wifi_sta_ip.toString());
  VF("WEM: WiFi STA GATEWAY= "); VL(wifi_sta_gw.toString());
  VF("WEM: WiFi STA SN     = "); VL(wifi_sta_sn.toString());
  VF("WEM: WiFi AP SSID    = "); VL(wifi_ap_ssid);
  VF("WEM: WiFi AP PWD     = ");
  VL(wifi_ap_pwd);
  VF("WEM: WiFi AP CH      = "); VL(wifi_ap_ch);
  VF("WEM: WiFi AP IP      = "); VL(wifi_ap_ip.toString());
  VF("WEM: WiFi AP GATEWAY = "); VL(wifi_ap_gw.toString());
  VF("WEM: WiFi AP SN      = "); VL(wifi_ap_sn.toString());
TryAgain:
  
  if (accessPointEnabled && !stationEnabled) {
    VLF("WEM: Starting WiFi Soft AP");
    WiFi.softAP(wifi_ap_ssid, wifi_ap_pwd, wifi_ap_ch);
    WiFi.mode(WIFI_AP);
  } else
  if (!accessPointEnabled && stationEnabled) {
    VLF("WEM: Starting WiFi Station");
    WiFi.begin(wifi_sta_ssid, wifi_sta_pwd);
    WiFi.mode(WIFI_STA);
  } else
  if (accessPointEnabled && stationEnabled) {
    VLF("WEM: Starting WiFi Soft AP");
    WiFi.softAP(wifi_ap_ssid, wifi_ap_pwd, wifi_ap_ch);
    VLF("WEM: Starting WiFi Station");
    WiFi.begin(wifi_sta_ssid, wifi_sta_pwd);
    WiFi.mode(WIFI_AP_STA);
  }

  delay(100);
  if ((stationEnabled) && (!stationDhcpEnabled)) WiFi.config(wifi_sta_ip, wifi_sta_gw, wifi_sta_sn);
  if (accessPointEnabled) WiFi.softAPConfig(wifi_ap_ip, wifi_ap_gw, wifi_ap_sn);

  [cite_start]// 在工作站模式下等待连接，如果失败则回退到热点模式 [cite: 202]
  if (!accessPointEnabled && stationEnabled) {
    for (int i=0; i<8; i++) if (WiFi.status() != WL_CONNECTED) delay(1000);
    else break;
    if (WiFi.status() != WL_CONNECTED) {
      VLF("WEM: Starting WiFi Station, failed");
      WiFi.disconnect(); delay(3000);
      VLF("WEM: Switching to WiFi Soft AP mode");
      stationEnabled=false;
      accessPointEnabled=true;
      goto TryAgain;
    }
  }

  clearSerialChannel();
  
  VLF("WEM: Connecting web-page handlers");
  server.on("/", handleRoot);
  server.on("/index.htm", handleRoot);
  server.on("/configuration.htm", handleConfiguration);
  server.on("/configurationA.txt", configurationAjaxGet);
  server.on("/settings.htm", handleSettings);
  server.on("/settingsA.txt", settingsAjaxGet);
  server.on("/settings.txt", settingsAjax);
#if ENCODERS == ON
  server.on("/enc.htm", handleEncoders);
  server.on("/encA.txt", encAjaxGet);
  server.on("/enc.txt", encAjax);
#endif
  server.on("/library.htm", handleLibrary);
  server.on("/libraryA.txt", libraryAjaxGet);
  server.on("/library.txt", libraryAjax);
  server.on("/control.htm", handleControl);
  server.on("/controlA.txt", controlAjaxGet);
  server.on("/control.txt", controlAjax);
  server.on("/auxiliary.htm", handleAux);
  server.on("/auxiliaryA.txt", auxAjaxGet);
  server.on("/auxiliary.txt", auxAjax);
  server.on("/pec.htm", handlePec);
  server.on("/pec.txt", pecAjax);
  server.on("/wifi.htm", handleWifi);
  
  server.onNotFound(handleNotFound);
#if STANDARD_COMMAND_CHANNEL == ON
  VLF("WEM: Starting port 9999 cmd svr");
  cmdSvr.begin();
  cmdSvr.setNoDelay(true);
#endif

#if PERSISTENT_COMMAND_CHANNEL == ON
  VLF("WEM: Starting port 9998 persistant cmd svr");
  persistentCmdSvr.begin();
  persistentCmdSvr.setNoDelay(true);
#endif

  VLF("WEM: Starting port 80 web svr");
  server.begin();

  // 留出时间让后台服务器启动
  delay(2000);
  // 最后一次清除串口通道
  clearSerialChannel();

#if ENCODERS == ON
  VLF("WEM: Starting Encoders");
  encoders.init();
#endif

  VLF("WEM: WiFi Addon is ready");
}

void loop(void) {
  server.handleClient();
#if ENCODERS == ON
  encoders.poll();
#endif

#if STANDARD_COMMAND_CHANNEL == ON
  // -------------------------------------------------------------------------------------------------------------------------------
  [cite_start]// 端口 9999 上的标准 IP 连接 [cite: 213]

  // 断开客户端连接
  static unsigned long clientTime = 0;
  if (cmdSvrClient && (!cmdSvrClient.connected())) cmdSvrClient.stop();
  if (cmdSvrClient && ((long)(clientTime-millis())<0)) cmdSvrClient.stop();
  // 新客户端
  if (!cmdSvrClient && (cmdSvr.hasClient())) {
    // 寻找空闲/断开连接的位置
    cmdSvrClient = cmdSvr.available();
    clientTime=millis()+2000UL;
  }

  [cite_start]// 检查客户端是否有数据，如果找到则获取命令，传递给 OnStep 并提取响应，然后将响应返回给客户端 [cite: 216]
  while (cmdSvrClient && cmdSvrClient.connected() && (cmdSvrClient.available()>0)) {
    static char cmdBuffer[40]="";
    static int cmdBufferPos=0;

    // 获取数据
    byte b=cmdSvrClient.read();
    cmdBuffer[cmdBufferPos]=b; cmdBufferPos++; if (cmdBufferPos>39) cmdBufferPos=39; cmdBuffer[cmdBufferPos]=0;
    // 发送命令并提取响应
    if (b == '#' || (strlen(cmdBuffer) == 1 && b == (char)6)) {
      char result[40]="";
      processCommand(cmdBuffer,result,cmdTimeout);               // 发送命令给 OnStep，提取响应
      if (strlen(result) > 0) { if (cmdSvrClient && cmdSvrClient.connected()) { cmdSvrClient.print(result);
      delay(2); } } // 客户端响应
      cmdBuffer[0]=0; cmdBufferPos=0;
    } else idle();
  }
  // -------------------------------------------------------------------------------------------------------------------------------
#endif

#if PERSISTENT_COMMAND_CHANNEL == ON
  // -------------------------------------------------------------------------------------------------------------------------------
  [cite_start]// 端口 9998 上的持久 IP 连接 [cite: 221]

  // 断开客户端连接
  static unsigned long persistentClientTime = 0;
  if (persistentCmdSvrClient && (!persistentCmdSvrClient.connected())) persistentCmdSvrClient.stop();
  if (persistentCmdSvrClient && ((long)(persistentClientTime-millis())<0)) persistentCmdSvrClient.stop();
  // 新客户端
  if (!persistentCmdSvrClient && (persistentCmdSvr.hasClient())) {
    // 寻找空闲/断开连接的位置
    persistentCmdSvrClient = persistentCmdSvr.available();
    persistentClientTime=millis()+120000UL;
  }

  // 检查客户端是否有数据，如果找到则获取命令，传递给 OnStep 并提取响应，然后将响应返回给客户端
  while (persistentCmdSvrClient && persistentCmdSvrClient.connected() && (persistentCmdSvrClient.available()>0)) {
    static char cmdBuffer[40]="";
    static int cmdBufferPos=0;

    // 仍然活跃？将断开连接时间推迟 2 分钟
    persistentClientTime=millis()+120000UL;
    // 获取数据
    byte b=persistentCmdSvrClient.read();
    cmdBuffer[cmdBufferPos]=b; cmdBufferPos++; if (cmdBufferPos>39) cmdBufferPos=39; cmdBuffer[cmdBufferPos]=0;
    // 发送命令并提取响应
    if (b == '#' || (strlen(cmdBuffer) == 1 && b == (char)6)) {
      char result[40]="";
      processCommand(cmdBuffer,result,cmdTimeout);               // 发送命令给 OnStep，提取响应
      if (strlen(result) > 0) { if (persistentCmdSvrClient && persistentCmdSvrClient.connected()) { persistentCmdSvrClient.print(result);
      delay(2); } } // 客户端响应
      cmdBuffer[0]=0; cmdBufferPos=0;
    } else idle();
  }
  // -------------------------------------------------------------------------------------------------------------------------------
#endif

}

const char* HighSpeedCommsStr(long baud) {
  if (baud==115200) { return ":SB0#";
  }
  if (baud==57600) { return ":SB1#"; }
  if (baud==38400) { return ":SB2#";
  }
  if (baud==28800) { return ":SB3#"; }
  if (baud==19200) { return ":SB4#"; } else { return ":SB5#";
  }
}

void idle() {
  server.handleClient(); 
#if ENCODERS == ON
  encoders.poll();
#endif
}

void serialBegin(long baudRate, int swap) {
  static bool firstRun = true;
  if (SERIAL_BAUD_DEFAULT == SERIAL_BAUD && (swap == ON || swap == OFF) && !firstRun) return;
  firstRun=false;
  if (swap == ON || swap == AUTO_ON) swap=1; else swap=0;
#ifdef ESP32
  // wemos d1 mini esp32
  // 未交换: TX 和 RX 在默认引脚
  //     交换: TX 在 gpio 5，RX 在 gpio 23
  if (swap) Ser.begin(baudRate,SERIAL_8N1,23,5);
  else Ser.begin(baudRate);
#else
  Ser.begin(baudRate); if (swap) Ser.swap();
#endif
  delay(1000);
}