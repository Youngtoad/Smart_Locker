#include <linux/init.h> 
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/fs.h> 
#include <linux/uaccess.h> 
#include <linux/slab.h>
#include <linux/gpio.h>

#include <asm/mach/map.h> 
#include <asm/uaccess.h> 


#include <linux/delay.h>

#define RGB_MAJOR_NUMBER   502
#define RGB_DEV_NAME      "RGB_dev" 

#define GPIO_BASE_ADDRESS   0x3F200000

#define GPFSEL1 0x04
#define GPFSEL2 0x08
#define GPSET0 0x1C
#define GPCLR0 0x28

#define RED_ON 1
#define GREEN_ON 4
#define OFF 3

static void __iomem * gpio_base;
volatile unsigned int * gpfsel1;
volatile unsigned int * gpfsel2;
volatile unsigned int * gpset0;
volatile unsigned int * gpclr0;

int rgb_open(struct inode * inode, struct file * filp)
{

    printk(KERN_ALERT "rgb driver open\n"); 
    
    gpio_base = ioremap(GPIO_BASE_ADDRESS, 0xD000);
    gpfsel1 = (volatile unsigned int *)(gpio_base + GPFSEL1);
    gpfsel2 = (volatile unsigned int *)(gpio_base + GPFSEL2);
    gpset0 = (volatile unsigned int *)(gpio_base + GPSET0);
    gpclr0 = (volatile unsigned int *)(gpio_base + GPCLR0);
	
    *gpfsel1 |= (1 << 18);
    *gpfsel1 |= (1 << 27);
   // *gpfsel2 |= (1 << 3);
    
    *gpclr0 |= (1 << 16);
    *gpclr0 |= (1 << 19);
   // *gpclr0 |= (1 << 21);
    
    return 0;

}

int rgb_release(struct inode * inode, struct file * filp) { 
    printk(KERN_ALERT "rgb driver close\n");
    *gpclr0 |= (1 << 16);
    *gpclr0 |= (1 << 19);
   // *gpclr0 |= (1 << 21);
    iounmap((void *)gpio_base); 
    return 0; 
}

long rgb_ioctl(struct file * flip, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
        case RED_ON:
        printk("RED!!!");
        *gpset0 |= (1 << 16);       //bcm 16
        *gpclr0 |= (1 << 19);
     //   *gpclr0 |= (1 << 21);
        break;
        
        case GREEN_ON:
        printk("GREEN!!!");
        *gpclr0 |= (1 << 16);
        *gpset0 |= (1 << 19);       //bcm 20
     //   *gpclr0 |= (1 << 21);
        break;
        
        case OFF:
        printk("BLUE!!!");
        *gpclr0 |= (1 << 19);
        *gpclr0 |= (1 << 16);
        //*gpclr0 |= (1 << 20);
        
        break;
        
    }
    return 0;
}


static struct file_operations rgb_fops = { 
    .owner = THIS_MODULE, 
    .open = rgb_open, 
    .release = rgb_release,
    .unlocked_ioctl = rgb_ioctl
};

int __init rgb_init (void) { 
    if(register_chrdev(RGB_MAJOR_NUMBER, RGB_DEV_NAME, &rgb_fops) < 0)
        printk(KERN_ALERT "RGB driver initalization failed\n"); 
    else 
        printk(KERN_ALERT "RGB driver initalization succeed\n");
    
    return 0; 
}

void __exit rgb_exit(void){ 
    unregister_chrdev(RGB_MAJOR_NUMBER, RGB_DEV_NAME); 
    printk(KERN_ALERT "RGB driver exit"); 
}

module_init(rgb_init);
module_exit(rgb_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CLE");
MODULE_DESCRIPTION("RGB DEVICE DRIVER");
