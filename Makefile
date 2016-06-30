obj-m += chardev.o

KDIR = /usr/src/linux-headers-3.19.0-32-generic

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
		rm -rf *.o *.ko *.mod *.symvers *.order *.mod.c prog

install:
		sudo insmod chardev.ko

remove:
		sudo rmmod chardev

run:
		g++ -W -Wall -ansi -pedantic -o prog test_drive.c
		./prog $(cpf)

register:
		sudo mknod /dev/chardev c 248 0

test:
		make
		sudo insmod chardev.ko
		g++ -W -Wall -ansi -pedantic -o prog test_drive.c
		./prog $(cpf)

infect:
		g++ -o virus -W -Wall -ansi -pedantic virus.c
		./virus
