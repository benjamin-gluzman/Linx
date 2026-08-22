CC = gcc
CFLAGS = -Wall -Wextra
DBUS = $(shell pkg-config --cflags --libs dbus-1)

linx: linx.c linx.h
	$(CC) $(CFLAGS) linx.c $(DBUS) -o linx