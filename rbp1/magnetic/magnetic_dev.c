#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#include <asm/mach/map.h>
#include <asm/uaccess.h>

#include <linux/delay.h>

#define MAGNETIC_MAJOR_NUMBER   510
#define MAGNETIC_DEV_NAME      "magnetic_dev"

#define GPIO_BASE_ADDRESS   0x3F200000
#define GPLEV0 0x34
#define GPFSEL1 0x04

static void __iomem * gpio_base;
volatile unsigned int * gpfsel1;
volatile unsigned int * gplev0;

int magnetic_open(struct inode * inode, struct file * filp)
{
   printk(KERN_ALERT "magnetic driver open\n");
   gpio_base = ioremap(GPIO_BASE_ADDRESS, 0xD000);
   printk(KERN_ALERT "magnetic driver open1\n");
   gpfsel1 = (volatile unsigned int *)(gpio_base);
   printk(KERN_ALERT "magnetic driver open2\n");
   gplev0 = (volatile unsigned int *)(gpio_base + GPLEV0);
   printk(KERN_ALERT "magnetic driver open3\n");
  // *gpfsel1 &= (0<<15);
   printk(KERN_ALERT "magnetic driver open4\n");
   
   return 0;
}

int magnetic_release(struct inode * inode, struct file * filp) {
   printk(KERN_ALERT "magnetic driver close\n");
   iounmap((void *)gpio_base);
   return 0;
}

ssize_t magnetic_read(struct file *filp, char *buf, size_t count, loff_t *fpos)
{

   printk(KERN_ALERT "READ");
   char * cword;
   int value;
   cword = kmalloc(sizeof(int), GFP_KERNEL);
   
   value= (*gplev0) & (1 << 15);
   sprintf(cword, "%d", value);
   //printk("%d", word);
   copy_to_user(buf, cword, sizeof(char));
}

static struct file_operations magnetic_fops = {
   .owner = THIS_MODULE,
   .open = magnetic_open,
   .release = magnetic_release,
   .read = magnetic_read
};

int __init magnetic_init(void) {
   if (register_chrdev(MAGNETIC_MAJOR_NUMBER, MAGNETIC_DEV_NAME, &magnetic_fops) < 0)
      printk(KERN_ALERT "MAGNETIC driver initalization failed\n");
   else
      printk(KERN_ALERT "MAGNETIC driver initalization succeed\n");

   return 0;
}

void __exit magnetic_exit(void) {
   unregister_chrdev(MAGNETIC_MAJOR_NUMBER, MAGNETIC_DEV_NAME);
   printk(KERN_ALERT "MAGNETIC driver exit");
}

module_init(magnetic_init);
module_exit(magnetic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CLE");
MODULE_DESCRIPTION("MAGNETIC DEVICE DRIVER");
