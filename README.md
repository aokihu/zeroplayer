# ZeroPlayer

这是一个基于 GUPnP、GLib 和 GStreamer 的 UPnP 音频渲染器项目，旨在为 UPnP 控制点（如智能手机或其他媒体控制软件）提供 HTTP 音频流的播放能力。此项目使用 `playbin` 作为 GStreamer 的核心播放组件，实现了包括播放、暂停、停止和跳转（seek）在内的多种功能。

## 目录

- [ZeroPlayer](#zeroplayer)
  - [目录](#目录)
  - [项目概述](#项目概述)
  - [功能特性](#功能特性)
  - [DLNA 必须实现的用户功能](#dlna-必须实现的用户功能)
  - [项目目录结构](#项目目录结构)
  - [安装和配置](#安装和配置)
    - [Mac 环境配置](#mac-环境配置)
    - [Linux 环境配置](#linux-环境配置)
  - [使用说明](#使用说明)
  - [命令行参数](#命令行参数)
  - [用户操作](#用户操作)
  - [模块详细说明](#模块详细说明)
    - [UPnP 服务模块](#upnp-服务模块)
    - [GStreamer 播放模块](#gstreamer-播放模块)
    - [GLib 主循环模块](#glib-主循环模块)
    - [命令行参数模块](#命令行参数模块)
    - [应用层级上下文结构模块](#应用层级上下文结构模块)
  - [模块代码示例](#模块代码示例)
    - [main.c 示例](#mainc-示例)
    - [UPnP 服务回调实现](#upnp-服务回调实现)
  - [Seek 功能实现](#seek-功能实现)
  - [测试与调试](#测试与调试)
    - [单元测试与集成测试](#单元测试与集成测试)
    - [调试工具](#调试工具)
  - [参考资料](#参考资料)
  - [贡献指南](#贡献指南)

## 项目概述

本项目旨在实现一个兼容 DLNA/UPnP 标准的音频渲染器设备。通过 GUPnP 框架处理 UPnP 设备的注册与控制命令，使用 GStreamer 处理媒体流的播放和控制，并借助 GLib 提供的主循环机制进行异步处理。

## 功能特性

- **UPnP 设备注册与控制**：通过 GUPnP 实现设备发现、注册和控制。
- **HTTP 音频流播放**：使用 GStreamer 的 `playbin` 组件处理 HTTP 音频流，实现播放、暂停、停止等功能。
- **音频跳转（Seek）功能**：支持对正在播放的媒体进行精确跳转。
- **事件处理**：处理 UPnP 控制点发送的各种控制命令，包括音量控制和状态查询。

## DLNA 必须实现的用户功能

在 DLNA 音频渲染器中，必须实现以下用户功能：

1. **播放控制**：允许用户通过控制点应用（如智能手机或 PC）来远程启动音频播放。
2. **暂停和停止**：提供对播放内容进行暂停和停止的操作。
3. **音量控制**：允许用户调整渲染器的音量，包括静音功能。
4. **跳转（Seek）功能**：支持用户在音频中进行快进或后退，直接跳转到指定时间。
5. **播放状态报告**：提供当前播放状态的信息（例如播放中、暂停、停止）以及进度（播放时间）。
6. **设备发现和注册**：确保渲染器可以被 DLNA 控制点设备发现，并在网络中注册。

这些功能确保用户能够以直观的方式与音频渲染器交互，实现对播放内容的全方位控制。

## 项目目录结构

```
zeroplayer/
│
├── src/                            # 源代码目录
│   ├── main.c                      # 主入口文件，包含主循环和初始化逻辑
│   ├── core/                       # 核心功能模块
│   │   ├── upnp/                   # UPnP 相关模块
│   │       ├── upnp_service.c      # UPnP 服务模块，处理设备注册和控制命令
│   │       ├── upnp_control.c      # UPnP 控制模块，处理控制点命令
│   ├── playback/                   # 播放控制模块
│       ├── gstreamer_player.c      # GStreamer 播放模块，管理媒体播放和跳转
│   ├── utils/                      # 工具函数和通用模块
│       ├── utils.c                 # 工具函数模块，包含通用的辅助功能
│   └── cli/                        # 命令行参数解析模块
│       ├── cli_parser.c            # 命令行参数解析实现
│
├── includes/                       # 头文件目录
│   ├── upnp_service.h              # UPnP 服务模块头文件
│   ├── upnp_control.h              # UPnP 控制模块头文件
│   ├── gstreamer_player.h          # GStreamer 播放模块头文件
│   ├── utils.h                     # 工具函数模块头文件
│   ├── cli_parser.h                # 命令行参数解析模块头文件
│
├── tests/                          # 测试代码目录
│   ├── unit/                       # 单元测试代码
│   ├── integration/                # 集成测试代码
│
└── docs/                           # 文档目录
    ├── README.md                   # 项目说明文档
    └── guide.md                    # 使用指南
```

## 安装和配置

### Mac 环境配置

1. 安装必要依赖：
   ```bash
   brew install gupnp gstreamer gst-plugins-base gst-plugins-good glib
   ```
2. 克隆项目并编译：
   ```bash
   git clone https://github.com/yourusername/zeroplayer.git
   cd zeroplayer
   make
   ```

### Linux 环境配置

1. 安装必要依赖：
   ```bash
   sudo apt-get install libgupnp-1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good libglib2.0-dev
   ```
2. 克隆项目并编译：
   ```bash
   git clone https://github.com/yourusername/zeroplayer.git
   cd zeroplayer
   make
   ```

## 使用说明

运行 ZeroPlayer：

```bash
./zeroplayer
```

## 命令行参数

- `-d` 或 `--debug`：启用调试模式，输出详细日志。
- `-c <path>` 或 `--config <path>`：指定配置文件路径。
- `-h` 或 `--help`：显示帮助信息。
- `--output-alsa-device` : 设置ALSA设备
- `-n` 或 `--name` : 设置设备显示的名称
- `-v` 或 `--version` : 打印软件当前版本信息

## 用户操作

通过 UPnP 控制点（例如智能手机上的控制应用程序）可以对 ZeroPlayer 进行操作，例如播放、暂停、停止和跳转到指定时间。

## 模块详细说明

### UPnP 服务模块

负责处理 UPnP 设备的发现和注册，响应来自控制点的控制命令。主要使用 GUPnP 库实现。

### GStreamer 播放模块

基于 GStreamer 的 `playbin` 组件实现，处理音频流的播放、暂停、停止和跳转。

### GLib 主循环模块

使用 GLib 的主循环机制管理整个应用的事件处理，确保 UPnP 消息和 GStreamer 状态变更能够及时响应。

### 命令行参数模块

用于解析用户通过命令行传入的参数，例如启用调试模式或指定配置文件路径。

### 应用层级上下文结构模块

定义了应用的上下文结构，用于保存全局状态，包括 GUPnP 设备对象、GStreamer 播放器对象和主循环对象。

## 模块代码示例

### main.c 示例

主入口文件，负责初始化应用上下文、启动 GLib 主循环并注册 UPnP 设备。

```c
int main(int argc, char *argv[]) {
    // 初始化应用上下文
    AppContext *app_context = init_app_context(argc, argv);
    if (app_context == NULL) {
        return -1;
    }

    // 启动主循环
    g_main_loop_run(app_context->main_loop);

    // 释放资源
    cleanup_app_context(app_context);
    return 0;
}
```

### UPnP 服务回调实现

实现了对控制点命令的回调处理，包括播放、暂停、停止和跳转请求。

```c
void on_play_request(GUPnPService *service, gpointer user_data) {
    AppContext *app_context = (AppContext *)user_data;
    gstreamer_player_play(app_context->player);
}
```

## Seek 功能实现

通过调用 GStreamer 的 `gst_element_seek` 函数实现对媒体播放位置的跳转。实现过程包括：

1. **解析控制点发送的跳转时间**：如 "00:05:30" 转换为纳秒。
2. **执行跳转操作**：调用 `gst_element_seek` 进行跳转。

示例代码：

```c
gboolean seek_to_position(GstElement *playbin, gint64 position_ns) {
    gboolean seek_success;
    seek_success = gst_element_seek(playbin,
                                    1.0, // rate
                                    GST_FORMAT_TIME,
                                    GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE,
                                    GST_SEEK_TYPE_SET, position_ns,
                                    GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
    if (!seek_success) {
        g_printerr("Seek failed!\n");
    }
    return seek_success;
}
```

## 测试与调试

### 单元测试与集成测试

- **单元测试**: 位于 `tests/unit/` 目录下，例如 `test_playback.c`、`test_upnp.c` 和 `test_cli_parser.c`，分别测试播放功能、UPnP 服务和命令行参数解析。
- **集成测试**: 位于 `tests/integration/` 目录下，例如 `test_full_flow.c`，测试完整的媒体播放流程。

运行测试的命令示例：

```bash
make test
./tests/unit/test_playback
./tests/integration/test_full_flow
```

### 调试工具

为了方便调试和排查问题，可以设置 GStreamer 的调试级别以获取更多日志信息。

使用以下命令设置 GStreamer 的调试日志级别：

```bash
export GST_DEBUG=playbin:4
```

这样可以输出有关 `playbin` 元素的详细日志，帮助开发者了解播放过程中的行为和遇到的问题。

另外，也可以使用以下工具来进行调试：

- **GDB**: 用于调试程序的崩溃或异常。
  ```bash
  gdb ./zeroplayer
  ```
- **Valgrind**: 用于检查内存泄漏和内存访问错误。
  ```bash
  valgrind ./zeroplayer
  ```
- **System Log**: 通过查看系统日志 (`/var/log/syslog` 或 `journalctl`) 获取与网络和系统相关的问题。

## 参考资料

- [GUPnP Documentation](https://gnome.pages.gitlab.gnome.org/gupnp/docs/)
- [GStreamer Playbin3 Documentation](https://gstreamer.freedesktop.org/documentation/playback/playbin3.html?gi-language=c)
- [DLNA/UPnP Specifications](https://spirespark.com/dlna/guidelines)
- [Implementing UPnP devices](https://gnome.pages.gitlab.gnome.org/gupnp/docs/server-tutorial.html)
- [Interacting with remote UPnP devices](https://gnome.pages.gitlab.gnome.org/gupnp/docs/client-tutorial.html)

## 贡献指南

如果您有兴趣为本项目做出贡献，请提交 PR 或联系项目维护者。欢迎任何关于代码改进或新功能的建议！
