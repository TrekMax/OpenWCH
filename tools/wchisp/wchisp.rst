WCHISP
======

wchisp - WCH ISP Tool in Rust

简介
----

wchisp 是一个用 Rust 编写的 WCH 系列芯片 ISP（In-System Programming）工具，支持通过串口/USB 对 WCH 微控制器进行固件烧录与管理。

特性
----

- 支持多种 WCH 芯片型号
- 通过串口或 USB 接口进行编程
- 命令行界面，易于集成到自动化流程
- 跨平台支持（Windows、Linux、macOS）
- 高效稳定的传输协议实现


安装
----

```shell
cargo install --git https://github.com/ch32-rs/wchisp
```
