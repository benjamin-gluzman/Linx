CC = gcc
CFLAGS = -Wall -Wextra -g -rdynamic
DBUS = $(shell pkg-config --cflags --libs dbus-1)

C_FILES = linx.c dbus_service.c bt_manager.c
H_FILES = linx.h shared.h dbus_service.h bt_manager.h

linx: $(C_FILES) $(H_FILES)
	$(CC) $(CFLAGS) $(C_FILES) $(DBUS) -o linx