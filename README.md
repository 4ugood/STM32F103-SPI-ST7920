# 基于 STM32F103 的点亮驱动器为 ST7920 的 LCD 屏幕

MCU 型号是 STM32F103RET6，采用模拟 SPI 串行驱动 LCD 的例程。

### 硬件

![屏幕截图 2023-04-10 104427](https://user-images.githubusercontent.com/117444566/230815118-dd6dc7d4-6919-4b52-8fff-63e70c79354f.png)

![屏幕截图 2023-04-10 103500](https://user-images.githubusercontent.com/117444566/230814294-314a2044-6fe2-44a5-b0c4-34da7ed229cd.png)

### 注意

- 如果出现中文乱码，请修改文件编码为 GB18030
- 工程使用 IDE 为 ARM Keil 5.14
- 只能用于 驱动器为 ST7920 的 LCD 液晶屏
- 原理图中 MCU 是 APM32E103RET6，与 STM32F103RET6 通用，唯一区别是主频不同
