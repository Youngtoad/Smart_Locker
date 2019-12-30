KERNEL_VER := 4.19.75-v7+
KERNEL_DIR = /lib/modules/$(KERNEL_VER)/build

obj-m := keypad_dev.o

PWD := $(shell pwd)

all :
	make -C $(KERNEL_DIR) M=$(PWD) modules
	sudo insmod keypad_dev.ko
	sudo mknod -m 666 /dev/keypad_dev c 498 0
clean :
	make -C $(KERNEL_DIR) M=$(PWD) clean
	sudo rmmod keypad_dev.ko
	sudo rm /dev/keypad_dev
