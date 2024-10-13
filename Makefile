# Makefile for ZeroPlayer

# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra -g $(shell pkg-config --cflags glib-2.0 gstreamer-1.0 gupnp-1.6) -I/usr/local/opt/icu4c/include
LDFLAGS = $(shell pkg-config --libs glib-2.0 gstreamer-1.0 gupnp-1.6) -L/usr/local/opt/icu4c/lib -licuuc


# 源文件目录
SRC_DIR = src
INCLUDE_DIR = includes

# 源文件
SOURCES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/cli/*.c)
OBJECTS = $(SOURCES:.c=.o)

# 可执行文件名
TARGET = zeroplayer

# 默认目标
all: $(TARGET)

# 编译规则
%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# 链接规则
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

# 清理规则
clean:
	rm -f $(OBJECTS) $(TARGET)

# 安装规则
install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin

# 卸载规则
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# 伪目标
.PHONY: all clean install uninstall
