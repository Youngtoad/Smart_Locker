KERNEL_VER := 4.19.75-v7+
KERNEL_DIR = /lib/modules/$(KERNEL_VER)/build

obj-m := magnetic_dev.o 

PWD := $(shell pwd) 

all : 
	make -C $(KERNEL_DIR) M=$(PWD) modules
	sudo insmod magnetic_dev.ko
	sudo mknod -m 666 /dev/magnetic_dev c 510 0   
clean : 
	make -C $(KERNEL_DIR) M=$(PWD) clean
	sudo rmmod magnetic_dev.ko
	sudo rm /dev/magnetic_dev
