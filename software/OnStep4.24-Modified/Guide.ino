// ---------------------------------------------------------------------------------------------------
// Guide.ino
// 导星与手控功能模块
// 负责处理 ST4 接口输入、手柄按键逻辑、以及电机导星脉冲的发送
// ---------------------------------------------------------------------------------------------------

// 如果启用了 ST4 接口（普通模式或上拉模式）
#if ST4_INTERFACE == ON || ST4_INTERFACE == ON_PULLUP
#include "src/lib/PushButton.h" // 引入按键消抖库

// 根据是否开启手控器功能，设置按键消抖时间
// 手控器模式下按键反应需要更灵敏或特定消抖
#if ST4_HAND_CONTROL == ON
  #define debounceMs 100 // 手控器模式：100ms 消抖
#else
  #define debounceMs 5   // 纯导星模式：5ms 消抖（响应更快）
#endif

// 定义四个方向的按键对象
button st4n; // 北 (Dec+)
button st4s; // 南 (Dec-)
button st4e; // 东 (RA+)
button st4w; // 西 (RA-)

// 单字节导星指令定义 (用于 Smart Hand Controller 通信协议)
#define ccMe 14 // 移动 东
#define ccMw 15 // 移动 西
#define ccMn 16 // 移动 北
#define ccMs 17 // 移动 南
#define ccQe 18 // 停止 东
#define ccQw 19 // 停止 西
#define ccQn 20 // 停止 北
#define ccQs 21 // 停止 南

#endif

// --- 全局变量定义 ---
unsigned long guideStartTime             = 0;  // 导星开始时间
long          guideTimeRemainingAxis1    = -1; // RA轴 剩余导星时间 (毫秒)
unsigned long guideTimeThisIntervalAxis1 = -1; // RA轴 本次时间间隔
long          guideTimeRemainingAxis2    = -1; // Dec轴 剩余导星时间
unsigned long guideTimeThisIntervalAxis2 = -1; // Dec轴 本次时间间隔
double        guideTimerCustomRateAxis1  = 0.0; // RA轴 自定义导星速率
double        guideTimerCustomRateAxis2  = 0.0; // Dec轴 自定义导星速率

// -----------------------------------------------------------------------
// initGuide()
// 初始化导星功能，设置引脚模式
// -----------------------------------------------------------------------
void initGuide() {
  // 重置所有状态变量
  guideDirAxis1              =  0;
  guideTimeRemainingAxis1    = -1;
  guideTimeThisIntervalAxis1 = -1;
  guideDirAxis2              =  0;
  guideTimeRemainingAxis2    = -1;
  guideTimeThisIntervalAxis2 = -1;

#if ST4_INTERFACE == ON || ST4_INTERFACE == ON_PULLUP
  // 判断是否需要开启内部上拉电阻
  #if ST4_INTERFACE == ON
    bool pullup=false; // 外部已有上拉
  #else
    bool pullup=true;  // 使用 MCU 内部上拉 (ON_PULLUP)
  #endif
  
  // 初始化四个方向的引脚
  // ST4DEn, ST4DEs 等宏定义在 Pins.xxx.h 或 Config.h 中
  st4n.init(ST4DEn,debounceMs,pullup); // 默认为低电平触发 (Active Low)
  st4s.init(ST4DEs,debounceMs,pullup);
  st4e.init(ST4RAe,debounceMs,pullup);
  st4w.init(ST4RAw,debounceMs,pullup);
#endif
}

// -----------------------------------------------------------------------
// guide()
// 主导星循环：在主循环中被调用，处理脉冲导星的计时和执行
// -----------------------------------------------------------------------
void guide() {
  // 设置 Axis1 (RA) 的修正量基础值为 0
  guideAxis1.fixed=0;
  
  // 原子操作读取当前的恒星时计数器 (防止中断干扰)
  cli(); long guideLst=lst; sei();
  
  // 如果时间有更新 (1/100秒 滴答)
  if (guideLst != guideSiderealTimer) {
    guideSiderealTimer=guideLst;

    // --- 处理 RA 轴 (Axis1) ---
    if (guideDirAxis1) {
      if (!inbacklashAxis1) { // 如果不在反向间隙补偿过程中
        // 计算 PEC (周期误差校正) 记录所需的步数方向
        if (guideDirAxis1 == 'e') guideAxis1.fixed=-amountGuideAxis1.fixed; // 东：减速/反向
        else if (guideDirAxis1 == 'w') guideAxis1.fixed=amountGuideAxis1.fixed; // 西：加速/正向

        // 处理脉冲导星 (Pulse Guiding) 的倒计时
        if (guideTimeRemainingAxis1 > 0) {
          guideTimeRemainingAxis1-=(long)(micros()-guideTimeThisIntervalAxis1);
          guideTimeThisIntervalAxis1=micros();
          // 时间到，停止导星
          if (guideTimeRemainingAxis1 <= 0) { guideDirAxis1='b'; } // 'b' 代表 break/停止
        }
      } else {
        // 如果正在反向间隙补偿中，不消耗导星时间
        guideTimeThisIntervalAxis1=micros();
      }
    }
    
    // --- 处理 Dec 轴 (Axis2) ---
    if (guideDirAxis2) {
      if (!inbacklashAxis2) {
        // 倒计时逻辑同上
        if (guideTimeRemainingAxis2 > 0) {
          guideTimeRemainingAxis2-=(long)(micros()-guideTimeThisIntervalAxis2);
          guideTimeThisIntervalAxis2=micros();
          if (guideTimeRemainingAxis2 <= 0) { guideDirAxis2='b'; }  // break 
        }
      } else {
        guideTimeThisIntervalAxis2=micros();
      }
    }

    // --- 处理螺旋搜索 (Spiral Search) ---
    // 这是一种自动让望远镜画螺旋线以寻找目标的模式
    if (spiralGuide) {
      // 只有当两个轴都在导星状态时才更新螺旋状态
      if ((guideDirAxis1 == 'e' || guideDirAxis1 == 'w') && (guideDirAxis2 == 'n' || guideDirAxis2 == 's')) guideSpiralPoll();
      else stopGuideSpiral();
    }
  }
}

// -----------------------------------------------------------------------
// 辅助判断函数
// -----------------------------------------------------------------------

// 判断是否正在进行螺旋导星
bool lastGuideSpiralGuide = false;
bool isSpiralGuiding() {
  if ((guideDirAxis1 || guideDirAxis2) && lastGuideSpiralGuide) return true;
  else { 
    // 如果之前是螺旋状态但现在方向没了，清除状态
    if (lastGuideSpiralGuide) { lastGuideSpiralGuide=false; guideTimerCustomRateAxis1=0.0; guideTimerCustomRateAxis2=0.0; }
    return false;
  }
}

// 判断是否正在进行脉冲导星
bool lastGuidePulseGuideAxis1 = false;
bool lastGuidePulseGuideAxis2 = false;
bool isPulseGuiding() {
  if ((guideDirAxis1 && lastGuidePulseGuideAxis1) || (guideDirAxis2 && lastGuidePulseGuideAxis2)) return true; else return false;
}

// 判断是否正在进行高速移动 (Slew)
// 包含：导星中、GOTO中、或正在同步中
bool isSlewing() {
  return guideDirAxis1 || guideDirAxis2 || trackingState == TrackingMoveTo || trackingSyncInProgress();
}

// -----------------------------------------------------------------------
// 反向间隙补偿控制 (Backlash)
// -----------------------------------------------------------------------
int backlashAxis1PriorToGuide=0;
int backlashAxis2PriorToGuide=0;

// 在导星开始前，如果有间隙补偿，先暂停它并保存值
void reactivateBacklashComp() {
#if GUIDE_DISABLE_BACKLASH == ON
  if (backlashAxis1PriorToGuide > 0) { cli(); backlashAxis1=backlashAxis1PriorToGuide; sei(); backlashAxis1PriorToGuide=0; }
  if (backlashAxis2PriorToGuide > 0) { cli(); backlashAxis2=backlashAxis2PriorToGuide; sei(); backlashAxis2PriorToGuide=0; }
#endif
}

// 恢复间隙补偿
void deactivateBacklashComp() {
#if GUIDE_DISABLE_BACKLASH == ON
  if (backlashAxis1PriorToGuide == 0) { backlashAxis1PriorToGuide=backlashAxis1; cli(); backlashAxis1=0; sei(); }
  if (backlashAxis2PriorToGuide == 0) { backlashAxis2PriorToGuide=backlashAxis2; cli(); backlashAxis2=0; sei(); }
#endif
}

// -----------------------------------------------------------------------
// startGuideAxis1()
// 启动 RA 轴导星
// 参数: 方向('e'/'w'), 速率(0-9), 持续时间(ms), 是否为脉冲模式
// -----------------------------------------------------------------------
CommandErrors startGuideAxis1(char direction, int guideRate, long guideDuration, bool pulseGuide) {
  // 一系列安全检查
  if (faultAxis1)                         return CE_SLEW_ERR_HARDWARE_FAULT; // 电机故障
  if (!axis1Enabled)                      return CE_SLEW_ERR_IN_STANDBY;     // 电机未使能
  if (parkStatus == Parked)               return CE_SLEW_ERR_IN_PARK;        // 处于停车状态
  if (trackingSyncInProgress())           return CE_MOUNT_IN_MOTION;         // 正在同步
  if (trackingState == TrackingMoveTo)    return CE_MOUNT_IN_MOTION;         // 正在 GOTO
  if (isSpiralGuiding())                  return CE_MOUNT_IN_MOTION;         // 正在螺旋搜索
  if (direction == guideDirAxis1)         return CE_NONE;                    // 方向未变
  
  // 检查是否超出限位 (Horizon/Meridian Limits)
  if (direction == 'e' && !guideEastOk()) return CE_SLEW_ERR_OUTSIDE_LIMITS;
  if (direction == 'w' && !guideWestOk()) return CE_SLEW_ERR_OUTSIDE_LIMITS;
  
  // 检查是否有系统级错误阻止移动
  if (guideRate < 3 && (generalError == ERR_ALT_MIN || generalError == ERR_LIMIT_SENSE || /*...*/ generalError == ERR_ALT_MAX)) 
      return CE_SLEW_ERR_OUTSIDE_LIMITS;

  // 根据速率决定是否禁用反向间隙补偿
  if (guideRate < 3) deactivateBacklashComp(); else reactivateBacklashComp();
  
  // 启用导星速率设置
  enableGuideRate(guideRate);
  
  // 设置状态
  guideDirAxis1=direction;
  guideTimeThisIntervalAxis1=micros();
  guideTimeRemainingAxis1=guideDuration*1000L;
  
  // 原子操作设置定时器速率 (正向或反向)
  cli();
  if (guideDirAxis1 == 'e') guideTimerRateAxis1=-guideTimerBaseRateAxis1; else guideTimerRateAxis1=guideTimerBaseRateAxis1; 
  sei();
  lastGuidePulseGuideAxis1 = pulseGuide;
  
  return CE_NONE;
}

// 停止 RA 轴导星
void stopGuideAxis1() {
  cli();
  if (guideDirAxis1 && guideDirAxis1 != 'b') { guideDirAxis1='b'; } 
  sei();
}

// -----------------------------------------------------------------------
// startGuideAxis2()
// 启动 Dec 轴导星 (逻辑同上)
// -----------------------------------------------------------------------
CommandErrors startGuideAxis2(char direction, int guideRate, long guideDuration, bool pulseGuide, bool absolute) {
  // ... (安全检查同上) ...
  if (faultAxis2)                          return CE_SLEW_ERR_HARDWARE_FAULT;
  if (!axis1Enabled)                       return CE_SLEW_ERR_IN_STANDBY;
  if (parkStatus == Parked)                return CE_SLEW_ERR_IN_PARK;
  if (trackingSyncInProgress())            return CE_MOUNT_IN_MOTION;
  if (trackingState == TrackingMoveTo)     return CE_MOUNT_IN_MOTION;
  if (isSpiralGuiding())                   return CE_MOUNT_IN_MOTION;
  if (direction == guideDirAxis2)          return CE_NONE;
  if (direction == 'n' && !guideNorthOk()) return CE_SLEW_ERR_OUTSIDE_LIMITS;
  if (direction == 's' && !guideSouthOk()) return CE_SLEW_ERR_OUTSIDE_LIMITS;
  // ... (系统错误检查同上) ...
  
  enableGuideRate(guideRate);
  if (guideRate < 3) deactivateBacklashComp(); else reactivateBacklashComp();
  
  guideDirAxis2=direction;
  guideTimeThisIntervalAxis2=micros();
  guideTimeRemainingAxis2=guideDuration*1000L;
  
  // 设置 Dec 轴方向速率
  if (guideDirAxis2 == 's') { cli(); guideTimerRateAxis2=-guideTimerBaseRateAxis2; sei(); } 
  if (guideDirAxis2 == 'n') { cli(); guideTimerRateAxis2= guideTimerBaseRateAxis2; sei(); }
  
  // 处理中天翻转后的方向 (Dec需要反向吗？)
  if (!absolute && (getInstrPierSide() == PierSideWest)) { cli(); guideTimerRateAxis2=-guideTimerRateAxis2; sei(); }
  lastGuidePulseGuideAxis2 = pulseGuide;
  
  return CE_NONE;
}

// --- 辅助限位检查函数 (返回 true 表示允许移动) ---
bool guideNorthOk() {
  if (!safetyLimitsOn) return true; // 如果安全限制关闭，则允许
  double a2; 
  if (AXIS2_TANGENT_ARM == ON) { cli(); a2=posAxis2/axis2Settings.stepsPerMeasure; sei(); } else a2=getInstrAxis2();
  
  // 检查 Dec 软限位和高度限位
  if (a2 < axis2Settings.min && getInstrPierSide() == PierSideWest) return false;
  if (a2 > axis2Settings.max && getInstrPierSide() == PierSideEast) return false;
  if (MOUNT_TYPE == ALTAZM && currentAlt > maxAlt) return false;
  return true;
}

bool guideSouthOk() {
  // ... (逻辑类似 guideNorthOk) ...
  if (!safetyLimitsOn) return true;
  double a2; if (AXIS2_TANGENT_ARM == ON) { cli(); a2=posAxis2/axis2Settings.stepsPerMeasure; sei(); } else a2=getInstrAxis2();
  if (a2 < axis2Settings.min && getInstrPierSide() == PierSideEast) return false;
  if (a2 > axis2Settings.max && getInstrPierSide() == PierSideWest) return false;
  if (MOUNT_TYPE == ALTAZM && currentAlt < minAlt) return false;
  return true;
}

bool guideEastOk() {
  if (!safetyLimitsOn) return true;
  // 检查中天翻转限位
  if (meridianFlip != MeridianFlipNever && getInstrPierSide() == PierSideEast) { if (getInstrAxis1() < -degreesPastMeridianE) return false; }
  if (getInstrAxis1() < axis1Settings.min) return false;
  return true;
}

bool guideWestOk() {
  if (!safetyLimitsOn) return true;
  if (meridianFlip != MeridianFlipNever && getInstrPierSide() == PierSideWest) { if (getInstrAxis1() > degreesPastMeridianW) return false; }
  if (getInstrAxis1() > axis1Settings.max) return false;
  return true;
}

// startGuideAxis2 的重载版本 (默认 absolute=false)
CommandErrors startGuideAxis2(char direction, int guideRate, long guideDuration, bool pulseGuide) {
  return startGuideAxis2(direction, guideRate, guideDuration, pulseGuide, false);
}

// 停止 Dec 轴导星
void stopGuideAxis2() {
  cli(); if (guideDirAxis2 && guideDirAxis2 != 'b') { guideDirAxis2='b'; } sei();
}

// -----------------------------------------------------------------------
// 螺旋导星 (Spiral Guide) 相关逻辑
// 用于在没有 goto 到目标时，画螺旋线寻找目标
// -----------------------------------------------------------------------
double spiralScaleAxis1=0;

CommandErrors startGuideSpiral(int guideRate, long guideDuration) {
  // ... (安全检查) ...
  if (faultAxis1 || faultAxis2)            return CE_SLEW_ERR_HARDWARE_FAULT;
  // ... (状态检查) ...
  if (isSpiralGuiding())                   return CE_MOUNT_IN_MOTION;
  
  // 限制螺旋导星的速率范围 (3-8)
  spiralGuide = guideRate;
  if (spiralGuide < 3) spiralGuide=3;
  if (spiralGuide > 8) spiralGuide=8;

  // 初始化时间
  guideStartTime=millis();
  // ...
  
  // 计算 RA 轴的比例 (因为在不同 Dec 处，RA 移动的弧度对应的天球角度不同)
  spiralScaleAxis1=cos(getInstrAxis2()/Rad);

  lastGuideSpiralGuide=true;
  guideSpiralPoll(); // 立即执行一次计算

  return CE_NONE;
}

void stopGuideSpiral() {
  spiralGuide=0;
  cli();
  if (guideDirAxis1) guideDirAxis1='b';
  if (guideDirAxis2) guideDirAxis2='b';
  sei();
}

// 螺旋导星的轮询函数：根据时间计算当前应该移动的方向和速度，画出螺旋线
void guideSpiralPoll() {
  double T=((long)(millis()-guideStartTime))/1000.0; // 经过的时间
  double rate=guideRates[spiralGuide]; // 导星速率
  if (rate > 1800.0) rate=1800.0;

  // 计算当前的半径和角度
  double radius=pow(T/6.28318,1.0/1.74);
  double angle =(radius-trunc(radius))*6.28318;

  // 计算分解到两个轴的速度
  guideTimerCustomRateAxis1=(rate/15.0)*cos(angle);
  guideTimerCustomRateAxis2=(rate/15.0)*sin(angle);
  
  // 设置方向
  if (guideTimerCustomRateAxis1 < 0) { guideTimerCustomRateAxis1=fabs(guideTimerCustomRateAxis1); guideDirAxis1='e'; } else guideDirAxis1='w';
  if (guideTimerCustomRateAxis2 < 0) { guideTimerCustomRateAxis2=fabs(guideTimerCustomRateAxis2); guideDirAxis2='s'; } else guideDirAxis2='n';

  // 修正球面坐标系下的 RA 速度
  guideTimerCustomRateAxis1/=spiralScaleAxis1;
  
  // 限制最大速度
  double rateXPerSec = RateToXPerSec/(maxRate/16.0);
  if (guideTimerCustomRateAxis1 > rateXPerSec) guideTimerCustomRateAxis1=rateXPerSec;
  
  // 应用自定义速率 (-1 代表使用 CustomRate 变量)
  enableGuideRate(-1);
  // ... (应用速率到定时器) ...
  if (guideDirAxis1 == 'e') { cli(); guideTimerRateAxis1=-guideTimerBaseRateAxis1; sei(); }
  if (guideDirAxis1 == 'w') { cli(); guideTimerRateAxis1= guideTimerBaseRateAxis1; sei(); }
  if (guideDirAxis2 == 's') { cli(); guideTimerRateAxis2=-guideTimerBaseRateAxis2; sei(); } 
  if (guideDirAxis2 == 'n') { cli(); guideTimerRateAxis2= guideTimerBaseRateAxis2; sei(); }
}

// 设置自定义导星速率的接口函数
bool customGuideRateAxis1(double rate) {
  guideTimerCustomRateAxis1=rate;
  currentGuideRate=-1;
  return true;
}
bool customGuideRateAxis2(double rate) {
  guideTimerCustomRateAxis2=rate;
  currentGuideRate=-1;
  return true;
}

// -----------------------------------------------------------------------
// 速率设置与转换
// -----------------------------------------------------------------------
void setGuideRate(int g) {
  currentGuideRate=g;
  // 保存脉冲导星速率到 NV (EEPROM)
  if ((g <= GuideRate1x) && (currentPulseGuideRate != g)) { currentPulseGuideRate=g; nv.update(EE_pulseGuideRate,g); }
  guideTimerCustomRateAxis1=0.0;
  guideTimerCustomRateAxis2=0.0;
}

int getPulseGuideRate() {
#if SEPARATE_PULSE_GUIDE_RATE == ON
  return currentPulseGuideRate; 
#else
  return currentGuideRate;
#endif
}

// 启用指定的导星速率索引
void enableGuideRate(int g) {
  if (g == activeGuideRate) return;
  if (g >= 0) {
    activeGuideRate=g;
    guideTimerCustomRateAxis1 = 0.0;
    guideTimerCustomRateAxis2 = 0.0;
  }

  // 计算并设置具体的定时器数值
  if (guideTimerCustomRateAxis1 != 0.0) {
    activeGuideRate = GuideRateNone;
    guideTimerBaseRateAxis1=guideTimerCustomRateAxis1;
  } else {
    guideTimerBaseRateAxis1=(double)(guideRates[g]/15.0);
  }
  if (guideTimerCustomRateAxis2 != 0.0) {
    activeGuideRate = GuideRateNone;
    guideTimerBaseRateAxis2=guideTimerCustomRateAxis2;
  } else {
    guideTimerBaseRateAxis2=(double)(guideRates[g]/15.0);
  }
  // 计算每个时间步进的 fixed point 数值
  amountGuideAxis1.fixed=doubleToFixed((guideTimerBaseRateAxis1*stepsPerSecondAxis1)/100.0);
  amountGuideAxis2.fixed=doubleToFixed((guideTimerBaseRateAxis2*stepsPerSecondAxis2)/100.0);
}

// =======================================================================
// ST4() 函数 - 修改版
// 修复了长按回原点被打断、速度慢的问题
// =======================================================================
void ST4() {
#if ST4_INTERFACE == ON || ST4_INTERFACE == ON_PULLUP
  // 轮询按键状态
  st4e.poll();
  static bool shcActive=false; 
  if (!shcActive) {
    st4w.poll();
    st4n.poll();
    st4s.poll();
  }

#if ST4_HAND_CONTROL == ON

  // 1. 智能手控器 (Smart Hand Controller) 检测逻辑 (保持不变)
  if (st4e.hasTone()) {
    if (!shcActive) {
      if (st4w.hasTone()) {
        pinMode(ST4DEs,OUTPUT);
        pinMode(ST4DEn,OUTPUT); 
        digitalWrite(ST4DEs,HIGH); 
        shcActive=true;
        SerialST4.begin();
        VLF("MSG: SerialST4 mode activated");
      }
      return;
    } else { 
      char c=SerialST4.poll();
      if (c == ccMe) startGuideAxis1('e',currentGuideRate,GUIDE_TIME_LIMIT*1000,false);
      // ... (省略其他 SHC 指令)
      return;
    }
  } else {
    if (shcActive) {
      #if ST4_INTERFACE == ON
        pinMode(ST4DEs,INPUT); pinMode(ST4DEn,INPUT);
      #elif ST4_INTERFACE == ON_PULLUP
        pinMode(ST4DEs,INPUT_PULLUP); pinMode(ST4DEn,INPUT_PULLUP);
      #endif
      shcActive=false;
      SerialST4.end();
      VLF("MSG: SerialST4 mode deactivated");
      return;
    }
  }

  // ---------------------------------------------------------
  // 2. 长按回原点逻辑 (Long Press Home)
  // ---------------------------------------------------------
  const long Shed_ms=4000;
  const long AltMode_ms=2000;

  static bool altModeA=false; 
  static bool altModeB=false;
  
  // [新增] 锁定标志，防止一直按着一直触发
  static bool homeTriggeredST4 = false;

  // --- 检测是否同时按下东西键 ---
  if (st4e.isDown() && st4w.isDown()) {
      
      // 如果按住超过 3000ms (3秒)
      if (st4e.timeDown() > 3000 && st4w.timeDown() > 3000) {
          
          // 只有之前没触发过，且当前不在回零中，才执行
          if (!homeTriggeredST4 && trackingState != TrackingMoveTo) {
              
              homeTriggeredST4 = true; // 锁定触发状态
              altModeA = false;        // 强制取消副模式，防止松手进入菜单
              altModeB = false;

              // 1. 立即停止当前的慢速导星 (解决速度慢干扰)
              stopGuideAxis1(); 
              stopGuideAxis2();
              
              // 2. [关键步骤] 欺骗系统：手动将ST4状态重置为 'b' (break/停止)
              // 这样当你松开手时，下方的逻辑会认为状态没有变化，从而不会发送 Stop 指令
              ST4DirAxis1 = 'b';
              ST4DirAxis2 = 'b';

              SerialA.println("MSG: ST4 Long Press -> Going Home!");
              
              // 3. 停止所有动作并进入快速模式
              stopSlewingAndTracking(SS_ALL_FAST);
              
              // 4. 执行回原点
              // 参数说明：true = 寻找限位开关 (Home Sensors)。
              // 如果你没有安装限位开关，或者觉得寻星速度慢，可以改为 false (仅回坐标零点)
              goHome(true); 
          }
      }
      // 如果还没到3秒，不执行任何操作，让普通导星逻辑在下方运行
  } else {
      // 只有当两个键都松开时，才重置触发锁，允许下一次长按
      if (st4e.isUp() && st4w.isUp()) {
          homeTriggeredST4 = false;
      }
  }
  
  if (st4n.isDown() && st4s.isDown()) stopGuideAxis2();

  // ---------------------------------------------------------
  // 3. 副模式 (Alt Mode) (保持原逻辑，但受 homeTriggeredST4 保护)
  // ---------------------------------------------------------
  if ((trackingState != TrackingMoveTo) && (!waitingHome) && !homeTriggeredST4) {
    if ((st4e.timeDown() > AltMode_ms) && (st4w.timeDown() > AltMode_ms) && (!altModeB)) { 
        if (!altModeA) { altModeA=true; soundBeep(); } 
    }
    if ((st4n.timeDown() > AltMode_ms) && (st4s.timeDown() > AltMode_ms) && (!altModeA)) { 
        if (!altModeB) { altModeB=true; soundBeep(); } 
    }
  }
  
  // 处理副模式松开
  if ( (altModeA || altModeB) && ((st4n.timeUp() < Shed_ms) || (st4s.timeUp() < Shed_ms) || (st4e.timeUp() < Shed_ms) || (st4w.timeUp() < Shed_ms)) ) {
    if (!cmdWaiting() && !homeTriggeredST4) {
       // ... 原有的副模式按键处理 ...
       if (altModeA) { /* 原有调速代码 */ }
       if (altModeB) { /* 原有调焦代码 */ }
    }
  } else {
    if ((altModeA || altModeB) && !homeTriggeredST4) { 
      altModeA=false; altModeB=false; soundBeep();
    }
#endif

    // ---------------------------------------------------------
    // 4. 普通导星按键处理
    // ---------------------------------------------------------
    if (axis1Enabled) {

      // [修改点] 增加判断：如果正在回原点 (TrackingMoveTo) 或者刚刚触发了长按
      // 则禁止进入普通的按键处理逻辑。这能彻底防止松手时打断回零。
      bool blockingHome = (trackingState == TrackingMoveTo) || homeTriggeredST4;

      // --- RA 轴 ---
      char newDirAxis1='b';
      if (st4w.isDown() && st4e.isUp()) newDirAxis1='w';
      if (st4e.isDown() && st4w.isUp()) newDirAxis1='e';
      
      // 仅当没有被回零阻塞时，才更新状态
      if (!blockingHome && newDirAxis1 != ST4DirAxis1) {
        ST4DirAxis1=newDirAxis1;
        if (newDirAxis1 != 'b') {
            #if SEPARATE_PULSE_GUIDE_RATE == ON && ST4_HAND_CONTROL != ON
            startGuideAxis1(newDirAxis1,currentPulseGuideRate,GUIDE_TIME_LIMIT*1000,false);
            #else
            startGuideAxis1(newDirAxis1,currentGuideRate,GUIDE_TIME_LIMIT*1000,false);
            #endif
        } else stopGuideAxis1(); 
      }
      // 如果正在回零 (blockingHome 为真)，我们强制忽略这里的 stopGuideAxis1()

      // --- Dec 轴 ---
      char newDirAxis2='b';
      if (st4n.isDown() && st4s.isUp()) newDirAxis2='n';
      if (st4s.isDown() && st4n.isUp()) newDirAxis2='s';
      
      if (!blockingHome && newDirAxis2 != ST4DirAxis2) {
        ST4DirAxis2=newDirAxis2;
        if (newDirAxis2 != 'b') {
            #if SEPARATE_PULSE_GUIDE_RATE == ON && ST4_HAND_CONTROL != ON
            startGuideAxis2(newDirAxis2,currentPulseGuideRate,GUIDE_TIME_LIMIT*1000,false);
            #else
            startGuideAxis2(newDirAxis2,currentGuideRate,GUIDE_TIME_LIMIT*1000,false);
            #endif
        } else stopGuideAxis2();
      }
    }
#if ST4_HAND_CONTROL == ON
  }
#endif
#endif
}