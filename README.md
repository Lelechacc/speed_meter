# 🚲 基于 STM32 + LVGL 的智能自行车码表

![Language](https://img.shields.io/badge/Language-C-blue)
![Platform](https://img.shields.io/badge/Platform-STM32-orange)
![Framework](https://img.shields.io/badge/Framework-LVGL-green)
![OS](https://img.shields.io/badge/OS-FreeRTOS-lightgrey)
![Driver](https://img.shields.io/badge/Library-HAL-blueviolet)
![IDE](https://img.shields.io/badge/IDE-Keil--MDK-red)

这是一个从零开始手敲的嵌入式 GUI 项目。本项目实现了从 Windows 模拟器验证到硬件移植的完整闭环，解决了大图片缓存、SPI 带宽限制以及多线程安全等核心工程问题。

👉 **[点击这里访问：项目在线演示主页（iMac 风格）](https://lelechacc.github.io/speed_meter/)**

---

## 📺 全功能实机演示

<div align="center">
  <video src="images/demo_video.mp4" width="100%" controls poster="images/实物路书页面.jpg">
    您的浏览器不支持播放该视频。
  </video>
  <p><i>包含：地图滚动、中英文切换、深/浅色模式动态重绘</i></p>
</div>

---

## 🛠️ 硬件架构与选型

| 模块 | 选型 | 说明 |
| :--- | :--- | :--- |
| **主控芯片** | STM32F405 / H750 | 高性能 ARM Cortex-M 核心 |
| **屏幕显示** | 240*320 TFT (ST7789驱动) | 带电容触摸 (CST驱动) |
| **GPS 模块** | 中科微 ATGM336H | 国产高性能定位芯片 |
| **陀螺仪** | LSM6DSMTR | 用于坡度计算与姿态感知 |
| **供电系统** | 3.7V 锂电池 | 集成充电与电量监测 |

---

## 🔥 核心技术攻坚

### 1. 九宫格地图加载机制
为了在 RAM 有限的单片机上实现无缝地图滚动，采用了“九宫格”动态加载算法。图片仅在进入屏幕范围时才读取并渲染，通过 **DMA2D** 硬件加速解决了 SPI 传输的带宽瓶颈。

### 2. 线程安全与异步更新
底层传感器（GPS/ADC）与 GUI 渲染存在竞态。项目通过 `lv_async_call()` 将外设数据更新委托给主循环，并结合 **FreeRTOS 互斥锁** 解决了渲染冲突引发的 `HardFault` 问题。

### 3. 多语言与主题引擎
利用 LVGL 的静态翻译机制与回调函数，实现了一键全局 UI 重绘，支持中/英动态字典映射及深色/浅色模式的动态切换。

---

## 💻 开发历程

### 第一阶段：PC 模拟开发 (Windows + SDL)
在硬件打样前，基于 PC 模拟环境完成了 90% 的 UI 逻辑编写，极大提高了调试效率。

### 第二阶段：底层驱动对接 (STM32 HAL)
编写 `stm32_lcd` 与 `indev` 接口，完成 ST7789 与触摸芯片驱动移植，并针对 Flash 空间进行了 LVGL 资源裁剪。

---

## 📦 如何运行
1. **克隆仓库**：`git clone https://github.com/Lelechacc/speed_meter.git`
2. **打开工程**：使用 Keil MDK 打开 `MDK-ARM/speed_meter.uvprojx`。
3. **编译烧录**：Rebuild 后通过 ST-Link 烧录至目标板。
4. **获取固件**：也可在 [Releases](https://github.com/Lelechacc/speed_meter/releases) 页面直接下载 `.bin` 固件。
---
### ⚠️ 版权声明 (Copyright & Anti-Plagiarism)
1. 本项目所有源码、文档、视频均为作者 **Lelechacc** 原创，严禁任何形式的造假（Resume Fraud）行为。
2. 允许个人学习与参考，但未经授权禁止用于任何商业用途。
3. 如果你在面试或商业演示中发现有人盗用本项目，请通过邮件联系我。
---

© 2026 Developed by Lelechacc. | [GitHub 主页](https://github.com/Lelechacc)
