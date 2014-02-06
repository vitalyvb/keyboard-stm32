TOPDIR=../
include $(TOPDIR)config.mk

all: names_key.c names_kp.c names_key.h names_kp.h

names_key.c: keys.txt
	./mk_kdata.sh keys.txt names_key.c keynames

names_kp.c: keys-keypad.txt
	./mk_kdata.sh keys-keypad.txt names_kp.c keynames_kp

names_key.h: keys.txt
	./mk_hdr.sh keys.txt names_key.h

names_kp.h: keys-keypad-hdr.txt
	./mk_hdr.sh keys-keypad-hdr.txt names_kp.h

clean:

flash:
