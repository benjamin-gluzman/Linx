CC = gcc

TARGET = linx

CFLAGS = -Wall -Wextra -g -MMD -MP
CFLAGS += $(shell pkg-config --cflags dbus-1)

LDFLAGS = -rdynamic
LDLIBS = $(shell pkg-config --libs dbus-1)

SRC = \
	main.c \
	dbus/linx_dbus.c \
	dbus/types/linx_types.c \
	bluetooth_profiles/linx_map.c \
	bluetooth_profiles/linx_pbap.c \
	bluetooth_manager/linx_object_manager.c \
	bluetooth_manager/linx_adapter.c \
	bluetooth_manager/linx_device.c \

OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

-include $(DEP)

clean:
	rm -f $(OBJ) $(DEP) $(TARGET)