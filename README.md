# PCIDriver
基于WinDriver的PCI驱动程序及测试软件
## 开发工具
* VS2005
* WinDriver v10.00
## 介绍
* PCIDriver工程：对驱动相关功能的实现进行封装，生成dll，用户通过接口函数对PCI设备进行操作
* PCIDriverTest工程：PCI驱动测试软件，对PCIDriver工程生成的dll进行测试
## 实现功能
支持中断收发、寄存器读写与DMA传输等操作
