# ZeroPlayer

这是一个基于 GUPnP、GLib 和 GStreamer 的 UPnP 音频渲染器项目，旨在为 UPnP 控制点（如智能手机或其他媒体控制软件）提供 HTTP 音频流的播放能力。此项目使用 `playbin` 作为GStreamer 的核心播放组件，实现了包括播放、暂停、停止和跳转（seek）在内的多种功能。

## 目录

- [项目概述](#项目概述)
- [功能特性](#功能特性)
- [项目目录结构](#项目目录结构)
- [安装和配置](#安装和配置)
  - [Mac 环境配置](#mac-环境配置)
  - [Linux 环境配置](#linux-环境配置)
- [使用说明](#使用说明)
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
│   ├── cli_parser.h                # 命令行参数解析头文件
│   ├── app.h                       # 应用层级上下文结构头文件
│
├── docs/
│   └── README.md                   # 项目说明文档
│
├── scripts/
│   ├── setup.sh                    # 环境安装脚本
│   └── build.sh                    # 项目构建脚本
│
├── tests/                          # 测试相关文件
│   ├── unit/                       # 单元测试模块
│   │   ├── test_playback.c         # 播放功能测试代码
│   │   ├── test_upnp.c             # UPnP 功能测试代码
│   │   ├── test_cli_parser.c       # 命令行参数解析测试代码
│   └── integration/                # 集成测试模块
│       ├── test_full_flow.c        # 全流程测试代码
│
└── Makefile                        # 项目构建脚本
```

## 安装和配置

### Mac 环境配置

在 macOS 平台上，可以使用 Homebrew 来安装所需的依赖库。首先，确保已安装 Homebrew，然后执行以下命令：

```bash
# 更新 Homebrew
brew update

# 安装必要的依赖库
brew install gupnp glib gstreamer gst-plugins-base gst-plugins-good gst-plugins-bad gst-plugins-ugly
```

### Linux 环境配置

在安装之前，请确保您的系统中安装了必要的开发库和工具，可以使用以下命令进行安装：

```bash
sudo apt-get update
sudo apt-get install libgupnp-1.0-dev libglib2.0-dev gstreamer1.0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly
```

### 项目构建

使用 `Makefile` 来构建项目：

```bash
make
```

### 运行程序

编译完成后，运行生成的可执行文件：

```bash
./zeroplay
```

## 使用说明

1. 启动程序后，ZeroPlayer 会注册到局域网内的 UPnP 控制点。
2. 可以使用支持 UPnP 的控制点软件（如 VLC、Foobar2000、BubbleUPnP 等）对设备进行控制。
3. 控制点可以发送播放、暂停、停止、跳转和音量调节等命令。

## 用户操作

DLNA 音频渲染器需要实现以下用户操作：

1. **播放**：开始播放指定的音频内容。
2. **暂停**：暂停当前播放的音频。
3. **停止**：停止音频播放，并将播放位置重置为起始位置。
4. **跳转（Seek）**：跳转到音频中的特定位置。例如，可以跳转到音频的某个时间点（如 "00:05:30"）。
5. **音量控制**：调整音频的输出音量，包括设置音量大小和静音操作。
6. **状态查询**：查询当前播放状态，包括当前播放位置、播放进度、总时长等。

## 模块详细说明

### UPnP 服务模块

- **文件**: `src/core/upnp/upnp_service.c`
- **功能**: 处理 UPnP 设备的注册、控制命令的解析和响应。
- **实现细节**: 通过 GUPnP 实现 AVTransport 和 RenderingControl 服务，使用回调函数响应控制点的操作请求。

### GStreamer 播放模块

- **文件**: `src/playback/gstreamer_player.c`
- **功能**: 管理媒体播放，支持播放、暂停、停止和跳转等功能，支持设置 ALSA 音频输出设备。
- **实现细节**: 使用 `playbin` 作为 GStreamer 管道核心，通过 `gst_element_seek` 实现跳转（seek）功能，增加了通过命令行参数设置输出设备的功能。

### GLib 主循环模块

- **功能**: 使用 GLib 提供的主循环管理整个项目的事件处理，使得网络事件和媒体播放控制都能异步处理。

### 命令行参数模块

- **文件**: `src/cli/cli_parser.c`
- **功能**: 解析命令行参数，包括音频输出设备的配置。
- **实现细节**: 使用 GLib 的 `GOptionContext` 来处理命令行参数，支持设置 `--output-alsa-device` 参数以指定 ALSA 音频输出设备。

### 应用层级上下文结构模块

- **文件**: `includes/app.h`
- **功能**: 提供应用层级的上下文结构体，用于在回调函数之间传递状态和配置信息。
- **实现细节**: 该模块定义了一个全局的上下文结构体，包含 GUPnP 上下文、GStreamer 播放器和其他共享状态，以便在不同模块中共享信息。

#### 应用层级上下文结构体示例代码

```c
#ifndef APP_H
#define APP_H

#include <gupnp.h>
#include <gst/gst.h>

typedef struct {
    GUPnPContext *upnp_context;
    GstElement *playbin;
    gchar *output_device;
} AppContext;

#endif // APP_H
```

## 模块代码示例

### main.c 示例

以下是 `main.c` 的示例代码，展示如何初始化主循环并调用核心模块：

```c
#include <glib.h>
#include <gupnp.h>
#include "upnp_service.h"
#include "gstreamer_player.h"
#include "cli_parser.h"
#include "app.h"

int main(int argc, char *argv[]) {
    // 创建应用上下文
    AppContext app_context = {0};

    // 初始化 GLib 主循环
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    app_context.upnp_context = gupnp_context_new(NULL, 0, NULL);
    if (!app_context.upnp_context) {
        g_printerr("Failed to create GUPnP context\n");
        return -1;
    }

    // 解析命令行参数
    parse_command_line_arguments(argc, argv, &app_context.output_device);

    // 设置输出设备
    if (app_context.output_device) {
        set_audio_output_device(app_context.output_device);
    }

    // 初始化 UPnP 服务
    if (!init_upnp_service(app_context.upnp_context)) {
        g_printerr("Failed to initialize UPnP service\n");
        return -1;
    }

    // 启动播放模块
    if (!init_gstreamer_player()) {
        g_printerr("Failed to initialize GStreamer player\n");
        return -1;
    }

    // 运行主循环
    g_main_loop_run(loop);

    // 清理资源
    g_main_loop_unref(loop);
    g_object_unref(app_context.upnp_context);

    return 0;
}
```

### UPnP 服务回调实现

以下是 `upnp_service.c` 中的回调函数示例，展示如何响应播放命令：

```c
#include "upnp_service.h"
#include "gstreamer_player.h"
#include "app.h"

void on_play_command(GUPnPService *service, GUPnPServiceAction *action, gpointer user_data) {
    AppContext *app_context = (AppContext *)user_data;
    // 调用 GStreamer 播放模块中的播放函数
    if (start_playback(app_context->playbin)) {
        gupnp_service_action_return(action);
    } else {
        gupnp_service_action_return_error(action, 501, "Playback failed");
    }
}
```

## Seek 功能实现

`playbin` 组件支持媒体跳转（seek）功能。在项目中，通过 `gst_element_seek` 函数实现对媒体播放位置的跳转。实现过程包括：

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
  gdb ./zeroplay
  ```
- **Valgrind**: 用于检查内存泄漏和内存访问错误。
  ```bash
  valgrind ./zeroplay
  ```
- **System Log**: 通过查看系统日志 (`/var/log/syslog` 或 `journalctl`) 获取与网络和系统相关的问题。

## 参考资料

- [GUPnP Documentation](https://gnome.pages.gitlab.gnome.org/gupnp/docs/)
- [GStreamer Playbin3 Documentation](https://gstreamer.freedesktop.org/documentation/playback/playbin3.html?gi-language=c)
- [DLNA/UPnP Specifications](https://spirespark.com/dlna/guidelines)

## 贡献指南

如果您有兴趣为本项目做出贡献，请提交 PR 或联系项目维护者。欢迎任何关于代码改进或新功能的建议！
