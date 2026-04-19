# 🚲 基于 STM32 + LVGL 的全地形越野导航仪表 (Off-road Navigation Meter)

![Language](https://img.shields.io/badge/Language-C-blue)
![Platform](https://img.shields.io/badge/Platform-STM32F405-orange)
![Framework](https://img.shields.io/badge/Framework-LVGL_9-green)
![OS](https://img.shields.io/badge/OS-FreeRTOS-lightgrey)
![Driver](https://img.shields.io/badge/Library-HAL-blueviolet)

本项目是一款针对户外越野场景设计的嵌入式导航终端。实现了从 Windows 模拟器逻辑验证到硬件平台深度优化的完整开发闭环，重点攻克了低算力平台下的大规模地图渲染与高频传感器数据处理等工程难题。

👉 **[点击这里访问：项目在线演示主页（iMac 风格）](https://lelechacc.github.io/speed_meter/)**

---

## 📺 全功能实机演示

<div align="center">
  <video src="images/demo_video.mp4" width="100%" controls poster="images/实物路书页面.jpg">
    您的浏览器不支持播放该视频。
  </video>
  <p><i>包含：九宫格瓦片地图、中英文实时切换、深/浅色模式动态重绘</i></p>
</div>

---

## 🛠️ 硬件架构与选型

| 模块 | 选型 | 说明 |
| :--- | :--- | :--- |
| **主控芯片** | **STM32F405RGT6** | Cortex-M4 核心, 168MHz 主频 |
| **外部存储** | **SD Card (SDIO总线)** | 用于存储大规模离线 BMP 瓦片地图 |
| **显示/触摸** | 240*320 TFT (ST7789) | 集成电容触摸 (CST系列驱动) |
| **定位/姿态** | ATGM336H + LSM6DSM | GPS 经纬度解算与姿态感知 |
| **电源管理** | 3.7V 锂电系统 | 集成 ADC 电量监测与硬件自持电路 |

---

## 🔥 核心技术攻坚 (Key Technical Features)

### 1. SDIO+DMA 硬件级地图调度
为了在有限的 RAM 下实现离线地图流畅滚动，项目设计了 **九宫格瓦片加载机制**。利用 **SDIO + DMA** 直接从外部存储提取 BMP 像素数据并刷入显示缓冲区，避开了传统文件系统的软读取瓶颈，实现了 168MHz 主频下的丝滑地图平移。

### 2. 传感器数据平滑与异步更新
- **滑动平均滤波**：针对越野颠簸场景，对六轴陀螺仪数据进行滤波处理，消除高频噪声，保障了坡度与方位显示的稳定性。
- **线程安全机制**：利用 `lv_async_call()` 将高频 GPS 中断数据解耦，结合 **FreeRTOS 互斥锁** 解决了 GUI 渲染与外设更新间的竞态冲突。

### 3. 坐标投影与多主题引擎
- **墨卡托投影算法**：自主编写经纬度坐标转换逻辑，实现 GPS 原始数据与地图像素坐标的实时映射。
- **动态重绘**：基于 LVGL 静态字典与回调机制，支持中/英动态重绘及深色/浅色主题的实时无缝切换。

---

## 💻 开发周期

### 第一阶段：PC 模拟器开发 (Windows + SDL)
在硬件打样前，基于 PC 模拟环境完成了 90% 的 UI 交互与状态机逻辑编写，显著缩短了调试周期。

### 第二阶段：硬件抽象层 (HAL) 移植
完成显示驱动 (ST7789)、触摸驱动以及存储驱动的底层对接，并针对 Flash 空间进行了 LVGL 资源裁剪优化。

---

## 📦 如何运行
1. **克隆仓库**：`git clone https://github.com/Lelechacc/speed_meter.git`
2. **环境要求**：Keil MDK (Version 5.30+) + ST-Link 仿真器。
3. **固件获取**：可在 [Releases](https://github.com/Lelechacc/speed_meter/releases) 页面直接下载编译好的 `.bin` 固件进行烧录体验。

---

### ⚖️ 许可与版权说明 (License)
1. 本项目由 **Lelechacc** 独立开发并拥有完整版权。
2. 欢迎开发者进行技术交流与学习参考。未经作者授权，禁止将本项目源码、文档或演示视频用于任何商业用途或作为简历造假材料。
3. **联系方式**: [cavalier_@live.com]

---

© 2026 Developed by Lelechacc. | [GitHub Profile](https://github.com/Lelechacc)
