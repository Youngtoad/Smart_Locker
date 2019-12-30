KERNEL_VER := 4.19.75-v7+
KERNEL_DIR = /lib/modules/$(KERNEL_VER)/build

obj-m := servo_dev.o

PWD := $(shell pwd)

all :
	make -C $(KERNEL_DIR) M=$(PWD) modules
	sudo insmod servo_dev.ko
	sudo mknod -m 666 /dev/servo_dev c 504 0
clean :
	make -C $(KERNEL_DIR) M=$(PWD) clean
	sudo rmmod servo_dev.ko
	sudo rm /dev/servo_dev
