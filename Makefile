CC=gcc
CFLAGS=-Wall
DEBUGCFLAGS=-Wall -g

all:
	$(CC) $(CFLAGS) -o split_dracut_ramfs split_dracut_ramfs.c

debug:
	$(CC) $(DEBUGCFLAGS) -o split_dracut_ramfs split_dracut_ramfs.c

clean:
	rm split_dracut_ramfs

