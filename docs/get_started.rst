
入门指南
====

本指南用于快速搭建开发环境，并通过示例工程完成编译、烧录与日志查看。

1. 环境搭建与配置
----

1.1 工具链
----

根据目标芯片选择对应的编译工具链，确保 `gcc`、`ld` 等命令可在终端中直接使用。

- 建议使用官方或经过验证的 WCH 工具链版本
- 配置 `PATH` 环境变量
- 验证 `arm-none-eabi-gcc`（或对应工具链）可用

1.2 Python 环境
----

SDK 的部分脚本与工具依赖 Python 运行环境。

- 建议使用 Python 3.8 及以上
- 使用 `venv` 或 `conda` 创建隔离环境
- 安装依赖：`pip install -r requirements.txt`

```shell
➜  OpenWCH git:(main) python3 -m venv .venv

# 激活环境变量 & 安装依赖
➜  OpenWCH git:(main) ✗ . ./.venv/bin/activate
(.venv) ➜  OpenWCH git:(main) ✗ pip install -r requirements.txt
```

1.3 烧录工具
----

支持 `wlin` 与 `wchisp` 两种烧录方式，请根据硬件与系统环境选择。

- wlin
	- 适用于 Linux 环境
	- 配置设备权限（如 udev 规则）

- wchisp
	- 适用于 Windows 环境
	- 需安装驱动与配置软件路径

1. 示例工程编译、烧录与日志查看
----

2.1 选择示例工程
----

在 `examples/` 中选择目标芯片与外设示例工程。

2.2 配置与编译
----

推荐使用 out-of-source 构建方式。

- 创建构建目录
- 配置目标芯片与示例工程
- 执行 CMake 生成与编译

2.3 烧录
----

使用 `wlin` 或 `wchisp` 将固件烧录到目标板。

- 选择串口或调试器设备
- 设置波特率与下载地址
- 执行烧录并确认成功

2.4 查看日志
----

通过串口工具查看运行日志。

- 选择正确的串口设备
- 设置与工程一致的波特率
- 观察启动信息与运行日志

常见问题
----

- 工具链无法识别
- 烧录失败或端口不可用
- 串口无日志输出

下一步
----

请继续阅读其他文档章节，了解组件、Kconfig 与系统 API 使用方式。
