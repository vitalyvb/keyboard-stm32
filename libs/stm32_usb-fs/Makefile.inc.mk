OBJS+= usb_core.o \
	usb_init.o \
	usb_int.o \
	usb_mem.o \
	usb_regs.o \
	usb_sil.o

usb_core.o: $(TOPDIR)libs/stm32_usb-fs/usb_core.c
	$(CC) -c $(CFLAGS) -o $@ $<
usb_init.o: $(TOPDIR)libs/stm32_usb-fs/usb_init.c
	$(CC) -c $(CFLAGS) -o $@ $<
usb_int.o: $(TOPDIR)libs/stm32_usb-fs/usb_int.c
	$(CC) -c $(CFLAGS) -o $@ $<
usb_mem.o: $(TOPDIR)libs/stm32_usb-fs/usb_mem.c
	$(CC) -c $(CFLAGS) -o $@ $<
usb_regs.o: $(TOPDIR)libs/stm32_usb-fs/usb_regs.c
	$(CC) -c $(CFLAGS) -o $@ $<
usb_sil.o: $(TOPDIR)libs/stm32_usb-fs/usb_sil.c
	$(CC) -c $(CFLAGS) -o $@ $<
