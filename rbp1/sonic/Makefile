KERNEL_VER := 4.19.75-v7+
KERNEL_DIR = /lib/modules/$(KERNEL_VER)/build

obj-m := hcsr04_dev.o

PWD := $(shell pwd)

all :
	make -C $(KERNEL_DIR) M=$(PWD) modules
	sudo insmod hcsr04_dev.ko
	sudo mknod -m 666 /dev/hcsr04_dev c 505 0
clean :
	make -C $(KERNEL_DIR) M=$(PWD) clean
	sudo rmmod hcsr04_dev.ko
	sudo rm /dev/hcsr04_dev
