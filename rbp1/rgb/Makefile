KERNEL_VER := 4.19.75-v7+
KERNEL_DIR = /lib/modules/$(KERNEL_VER)/build

obj-m := rgb_dev.o

PWD := $(shell pwd)

all :
	make -C $(KERNEL_DIR) M=$(PWD) modules
	sudo insmod rgb_dev.ko
	sudo mknod -m 666 /dev/rgb_dev c 502 0
clean :
	make -C $(KERNEL_DIR) M=$(PWD) clean
	sudo rmmod rgb_dev.ko
	sudo rm /dev/rgb_dev
