#compiler
#CC = /opt/buildroot-gcc342/bin/mipsel-linux-uclibc-gcc

#flags
#CFLAGS = -Wall -ansi -g
CFLAGS = -g 
CFLAGS += -I/usr/src/linux-$(shell uname -r)/include/generated/uapi -I/usr/src/linux-$(shell uname -r)/include/uapi #add include header directories for kernel 3.10.36
CFLAGS += -I/usr/src/linux-$(shell uname -r)/include
#CFLAGS = -g -I/usr/src/linux-2.6.36.4/include

#objects
OBJS = SONiX_UVC_TestAP.o sonix_xu_ctrls.o v4l2uvc.o nalu.o cap_desc_parser.o cap_desc.o

#install path
INSTALL_PATH = ./

all: SONiX_UVC_TestAP

SONiX_UVC_TestAP: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ -lpthread

SONiX_UVC_TestAP.o: SONiX_UVC_TestAP.c sonix_xu_ctrls.h
	$(CC) $(CFLAGS) -c -o $@ $<

sonix_xu_ctrls.o: sonix_xu_ctrls.c sonix_xu_ctrls.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f *.o *.ko .*.cmd .*.flags *.mod.c


