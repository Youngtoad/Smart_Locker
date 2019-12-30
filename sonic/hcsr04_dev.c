#include <linux/version.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/interrupt.h> 
#include <linux/types.h>

#define HCSR04_MAJOR_NUMBER 505
#define HCSR04_DEV_NAME "hcsr04_dev" 

#define GPIO_BASE_ADDRESS   0x3F200000

#define GPFSEL1 0x04
#define GPFSEL2 0x08
#define GPSET0   0x1C
#define GPCLR0   0x28
#define GPLEV0  0x34

#define HCSR04_ECHO   21
#define HCSR04_TRIGGER   6

static void __iomem * gpio_base;
volatile unsigned int * gpfsel0;
volatile unsigned int * gpfsel1;
volatile unsigned int * gpfsel2;
volatile unsigned int * gpset0;
volatile unsigned int * gpclr0;
volatile unsigned int * gplev0;

static ktime_t echo_start;
static ktime_t echo_end;

int hcsr04_open(struct inode * inode, struct file * filp)
{
   
   printk(KERN_ALERT "HCSR04 driver open\n"); 
   
   gpio_base = ioremap(GPIO_BASE_ADDRESS, 0xFF);
   gpfsel0 = (volatile unsigned int *)(gpio_base);
   gpfsel1 = (volatile unsigned int *)(gpio_base + GPFSEL1);
   gpfsel2 = (volatile unsigned int *)(gpio_base + GPFSEL2);
    gpset0 = (volatile unsigned int *)(gpio_base + GPSET0);
    gpclr0 = (volatile unsigned int *)(gpio_base + GPCLR0);
    gplev0 = (volatile unsigned int *)(gpio_base + GPLEV0);
    
    
   //*gpfsel2 &= (0 << 3);
   *gpfsel0 |= (1 << 18);
    
   return 0;

}

int hcsr04_release(struct inode * inode, struct file * filp) { 
   printk(KERN_ALERT "hcrs04 driver close\n");
   iounmap((void *)gpio_base);
   return 0; 
}

ssize_t hcsr04_read(struct file *filp, char *buf, size_t count, loff_t *fpos) 
{
   printk("sensor to ready");
   *gpclr0 |= (1 << 12);
	mdelay(300);
   
   printk("Set trig 1");
   *gpset0 |= (1 << 12);
   udelay(10);
   *gpclr0 |= (1 << 12);
   printk("Set trig 0");
   
   while((*gplev0 & (0x01 << 21)) == 0)
   {
      echo_start = ktime_get();
   }
   while((*gplev0 & (0x01 << 21)) != 0)
   {
      echo_end = ktime_get();
   }
   
   printk("time : %lld", ktime_to_us(ktime_sub(echo_end, echo_start)));
   sprintf(buf, "%lld\n", ktime_to_us(ktime_sub(echo_end,echo_start)));;
   
   return count;
}


static struct file_operations hcsr04_fops = { 
   .owner = THIS_MODULE, 
   .open = hcsr04_open, 
   .release = hcsr04_release,
   .read = hcsr04_read
};

static int hcsr04_init(void)
{   
   printk(KERN_INFO "HC-SR04 driver init.\n");
   if(register_chrdev(HCSR04_MAJOR_NUMBER, HCSR04_DEV_NAME, &hcsr04_fops) < 0)
      printk(KERN_ALERT "SERVO driver initalization failed\n"); 
   else 
      printk(KERN_ALERT "SERVO driver initalization succeed\n");
   return 0;

}
 
static void hcsr04_exit(void)
{
   unregister_chrdev(HCSR04_MAJOR_NUMBER, HCSR04_DEV_NAME); 
   printk(KERN_ALERT "HCSR04 driver exit");
    
}
 
module_init(hcsr04_init);
module_exit(hcsr04_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JoonSung Yoon");
MODULE_DESCRIPTION("HC-SR04 DEVICE DRIVER");
