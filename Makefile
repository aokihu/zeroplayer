# Makefile for ZeroPlayer

# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra -g $(shell pkg-config --cflags glib-2.0 gstreamer-1.0 gupnp-1.6 gupnp-av-1.0) -I/usr/local/opt/icu4c/include
LDFLAGS = $(shell pkg-config --libs glib-2.0 gstreamer-1.0 gupnp-1.6 gupnp-av-1.0) -L/usr/local/opt/icu4c/lib -licuuc

# 目录设置
SRC_DIR = src
INCLUDE_DIR = includes
OBJ_DIR = obj
CLI_DIR = $(SRC_DIR)/cli
PLAYBACK_DIR = $(SRC_DIR)/playback
UPNP_DIR = $(SRC_DIR)/upnp

# 源文件
SOURCES = $(wildcard $(SRC_DIR)/*.c) \
          $(wildcard $(CLI_DIR)/*.c) \
          $(wildcard $(PLAYBACK_DIR)/*.c) \
          $(wildcard $(UPNP_DIR)/*.c) \

# 目标文件
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# 可执行文件名
TARGET = zeroplayer

# 默认目标
all: $(TARGET)

# 创建obj目录及其子目录
$(shell mkdir -p $(OBJ_DIR) $(OBJ_DIR)/cli $(OBJ_DIR)/playback $(OBJ_DIR)/upnp)

# 编译规则
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# 链接规则
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

# 清理规则
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# 安装规则
install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin

# 卸载规则
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# 伪目标
.PHONY: all clean install uninstall
