# OnStep 优化补丁 (For N.I.N.A. & APP)

## 📌 背景与痛点

在使用 OnStep 驱动谐波赤道仪时，由于谐波减速器不具备物理离合、静摩擦力小且扭矩极大，如果在开机未回零（未建立绝对坐标）的情况下误触指令，或设备两端不平衡，极易导致设备撞台、甚至断电溜车砸毁设备。

同时，在拦截这些危险指令时，如果处理不当，会引发 **LX200 通讯协议底层冲突**，导致以下严重问题：

1. **N.I.N.A. 串口崩溃**：强行返回自定义中文字符串会导致 ASCOM 驱动解析失败 (`Parsing Error`)。
2. **蓝牙极高延迟 (Log Flooding)**：使用 `return` 强行掐断通讯或频繁触发 `CE_PARKED` 错误，会导致底层日志系统疯狂刷屏，撑爆 ESP32 的蓝牙缓存，致使 APP 按键指令严重卡顿。
3. **幽灵跟踪**：GOTO 被拦截后，天文软件的保护机制或预处理逻辑会强行唤醒恒星跟踪。

---

## 🎯 功能特性

本补丁对 `Command.ino`、`OnStep.ino` 及相关初始化逻辑进行了深度优化，实现了完美的“开机防撞锁”、“防溜车机制”与“脱困逻辑”：

* **开机防溜车抱闸**：通电瞬间自动使能电机并进入静止状态，利用电机的保持转矩（Holding Torque）充当电子抱闸，防止严重不平衡时的重力滑落。
* **物理级一键回零**：复用 ST4 导星接口，外接实体按键长按 3 秒即可触发寻找零位（Find Home），并带有硬件级防抖与电机脉冲屏蔽锁。
* **GOTO 拦截伪装**：开机未回零前拦截 GOTO，并向 N.I.N.A. 返回标准 `3` (Standby) 状态，优雅触发弹窗报错，强制关闭误唤醒的跟踪。
* **Tracking 拦截与防堵塞**：未回零前拒绝所有开启跟踪指令，回复标准 `0` (失败)，彻底消除 APP 死等造成的蓝牙高延迟。
* **方向键静默拦截**：未回零或触发限位时，静默锁死对应方向，伪装成无错误 (`CE_NONE`)，实现零延迟、无日志溢出的丝滑手控。
* **Unpark 拦截**：未回零前遵循 LX200 协议拒绝解除停放，防止意外启动。
* **智能限位脱困与单次急停**：撞击限位后仅触发单次急停报警（防刷屏），并仅锁死撞击方向，允许向反方向安全逃离。

---

## 🛠️ 代码修改指南

### 1. 开机自动使能电机防止“溜车”滑落 (修改初始化逻辑)

**定位**: 搜索 OnStep 初始化或开机默认跟踪状态配置的代码段（通常在 `setup()` 尾部或处理 `TrackingNone` 初始化的逻辑中）。
**修改逻辑**: 针对谐波减速器断电易滑落的物理特性，在开机瞬间强制物理激活步进电机驱动器。

```cpp
    // --- [修改开始] 开机防滑落抱死 ---
    // 1. 将状态设置为“TrackingNone”
    // 作用：系统进入工作状态（Active），但不发送步进脉冲，电机保持绝对静止
    trackingState = TrackingNone;
    
    // 2. 物理激活驱动器 
    // 作用：拉低 EN 引脚，让电机通电产生保持转矩（Holding Torque），充当电子抱闸
    enableStepperDrivers();
    // --- [修改结束] ---
```

### 2. 复用 ST4 接口实现物理“一键回零” (修改 `ST4` 逻辑)

**定位**: 搜索 `void ST4()` 函数内部，定位到 `// standard hand control` 下方区域。
**修改逻辑**: 巧妙利用闲置的 ST4 导星接口，外接物理按键（同时短接 East 和 West 引脚至 GND）。加入 `homingLockout` 防抖锁。

```cpp
  // standard hand control
  const long Shed_ms=4000;
  const long AltMode_ms=2000;

  // =================================================================
  // [新增] 长按 3 秒立即触发回零，附带按键屏蔽锁，保护电机脉冲
  // =================================================================
  static bool homingLockout = false; // 屏蔽锁状态标志

  // 1. 如果锁是开启的，说明已经触发了回原点，此时检测是否松手
  if (homingLockout) {
    // 只要有任意一个按键还按着，就会直接 return 退出，彻底屏蔽干扰！
    if (!st4e.isDown() && !st4w.isDown() && !st4n.isDown() && !st4s.isDown()) {
      homingLockout = false; 
    }
    return; // 拦截点，保护回原点过程不被打断
  }

  // 2. 长按检测逻辑
  if ((trackingState != TrackingMoveTo) && (!waitingHome)) {
    // 检测是否同时按下了东键(E)和西键(W)
    if (st4e.isDown() && st4w.isDown()) {
      
      // 如果按下的时间达到了 3000 毫秒（3秒）
      if ((st4e.timeDown() > 3000) && (st4w.timeDown() > 3000)) {
        homingLockout = true; // 第一步：立刻上锁，忽略接下来的所有按键状态
        soundBeep();          // 蜂鸣器滴一声，提示用户
        
        // 停止一切当前的追踪和导星动作
        stopGuideAxis1();
        stopGuideAxis2();
        stopSlewingAndTracking(SS_ALL_FAST);
        
        // 第二步：立刻触发回原点，不需要等松手
        goHome(true); 
        return; 
      }
      
      // 如果按下了，但是还没满 3 秒：
      // 需要阻止原本的 AltMode 或常规导星逻辑触发，叫停轴 1 并立刻返回等待计时
      stopGuideAxis1(); 
      return; 
    }
  }
  // =================================================================
```

### 3. 拦截 GOTO 并强制关闭跟踪 (修改 `Command.ino`)

**定位**: 搜索 `if (command[1] == 'S' && parameter[0] == 0)` (处理 `:MS#` 指令处)
**修改逻辑**: 伪装成待机状态 (`3`)，优雅弹窗，关闭跟踪，且不产生报错日志。

```cpp
      // :MS#       Goto the Target Object
      if (command[1] == 'S' && parameter[0] == 0)  {
        
        // --- [新增] 拦截 GOTO，伪装成 Standby (待机) 状态 ---
        if (!systemHasHomed) {
            reply[0] = '3'; // 返回 3，代表 controller in standby
            reply[1] = 0;
            boolReply = false;
            supress_frame = true;
            
            // 骗过 OnStep 底层日志系统，防止蓝牙卡顿
            commandError = CE_NONE; 

            // 顺手强制关闭可能被 APP 提前唤醒的跟踪功能
            trackingState = TrackingNone; 

        } else {
            // ... [保留原本的 GOTO 逻辑] ...
            newTargetRA=origTargetRA; newTargetDec=origTargetDec;
#if TELESCOPE_COORDINATES == TOPOCENTRIC
            topocentricToObservedPlace(&newTargetRA,&newTargetDec);
#endif
            CommandErrors e=goToEqu(newTargetRA,newTargetDec);
            if (e >= CE_GOTO_ERR_BELOW_HORIZON && e <= CE_GOTO_ERR_UNSPECIFIED) reply[0]=(char)(e-CE_GOTO_ERR_BELOW_HORIZON)+'1';
            if (e == CE_NONE) reply[0]='0';
            reply[1]=0;
            boolReply=false;
            supress_frame=true;
            commandError=e;
        }
      } else
```

### 4. 规范拦截开启跟踪消除闲置高延迟 (修改 `Command.ino`)

**定位**: 搜索 `if (command[0] == 'T'` 下方的 `if (command[1] == 'e') {` (即 `:Te#` 指令)
**修改逻辑**: 强制规范回复 `0`，防止 APP 死等导致蓝牙崩溃。

```cpp
        // :Te#       Tracking enable
        if (command[1] == 'e') {
          
          // --- [新增] 必须标准回复 0，否则 APP 死等导致蓝牙崩溃！ ---
          if (!systemHasHomed) {
              reply[0] = '0'; // 明确告诉 APP 被拒绝了
              reply[1] = 0; 
              boolReply = false; 
              supress_frame = true; 
              commandError = CE_NONE; // 保持静默，不刷日志
              return; 
          }
          // --------------------------------------------------------

          if (isParked()) commandError=CE_PARKED; else
          // ... [保留原有 trackingState 逻辑] ...
```

### 5. 拦截手动方向键实现零延迟静默锁 (修改 `Command.ino`)

**定位**: 搜索 `:Me#` 或 `Move Telescope East or West`
**修改逻辑**: 剥离 `return`，使用 `CE_NONE` 欺骗日志系统，实现静默锁死。

```cpp
      // :Me# :Mw#  Move Telescope East or West at current guide rate
      if ((command[1] == 'e' || command[1] == 'w') && parameter[0] == 0) {
        
        // --- [修改] 没回零，或者按了东且东面锁死，或者按了西且西面锁死 ---
        if (!systemHasHomed || (command[1] == 'e' && Axis1_LimitLock == 1) || (command[1] == 'w' && Axis1_LimitLock == -1)) {
            boolReply = false;       // 告诉底层：不用返回字符
            commandError = CE_NONE;  // 伪装成无错误！
        } else {
            commandError=startGuideAxis1(command[1],currentGuideRate,GUIDE_TIME_LIMIT*1000,false);
            boolReply=false;
        }
      } else
      
      // :Mn# :Ms#  Move Telescope North or South at current guide rate
      if ((command[1] == 'n' || command[1] == 's') && parameter[0] == 0) {
        
        // --- [修改] 没回零，或者按了北且北面锁死，或者按了南且南面锁死 ---
        if (!systemHasHomed || (command[1] == 'n' && Axis2_LimitLock == 1) || (command[1] == 's' && Axis2_LimitLock == -1)) {
            boolReply = false;       
            commandError = CE_NONE;  
        } else {
            commandError=startGuideAxis2(command[1],currentGuideRate,GUIDE_TIME_LIMIT*1000,false);
            boolReply=false;
        }
      } else
```

### 6. 拦截 Unpark 唤醒指令 (修改 `Command.ino`)

**定位**: 搜索 `:hR#` 或 `Restore parked telescope`
**修改逻辑**: 未回零前，仅返回 `0` 拒绝解除停放。

```cpp
      // :hR#       Restore parked telescope to operation
      if (command[1] == 'R' && parameter[0] == 0) {
        
        // --- [新增] 开机未回零前，拦截 Unpark 并返回失败 ---
        if (!systemHasHomed) {
            reply[0] = '0'; // 返回 '0' 代表被拒绝
            reply[1] = 0;   
            boolReply = false;
            supress_frame = true;
            commandError = CE_PARKED;
        } else {
            commandError=unPark(true); 
        }
      } else
```

### 7. 优化智能脱困与单次急停防刷屏 (修改 `OnStep.ino`)

**定位**: 搜索 `loop2()` 中限位判断代码。
**修改逻辑**: 移除易导致意外锁死的坐标推断逻辑，增加单次报错锁防刷屏。

```cpp
        // =========================================================
        // 恢复纯粹的撞击方向锁死逻辑
        // =========================================================
        if (Axis1_LimitLock == 0 && currentMotionDir1 != 0) {
            Axis1_LimitLock = currentMotionDir1;
        }
        if (Axis2_LimitLock == 0 && currentMotionDir2 != 0) {
            Axis2_LimitLock = currentMotionDir2;
        }

        // =========================================================
        // 执行急停 (带有单次触发锁)
        // =========================================================
        if (!isEscaping) {
            // --- [核心修改] 加一把单次触发锁，防止刷屏撑爆蓝牙 ---
            if (generalError != ERR_LIMIT_SENSE) {
                generalError = ERR_LIMIT_SENSE;
                stopGuideAxis1(); 
                stopGuideAxis2();
                stopSlewingAndTracking(SS_LIMIT_HARD);
            }
        }
```

---

## 💡 总结与建议

经过上述修改，基于 OnStep 的谐波赤道仪如同加上了“物理级”的安全外骨骼。从开机通电的瞬间到未回零前的任何误操作，都被死死按在安全的边界内，且完美兼顾了 ASCOM 驱动和 LX200 协议的底层脾气。建议搭配 N.I.N.A. 等上位机软件使用时，养成开机首选 **Find Home** 的良好习惯，享受丝滑且安全的星空探索。