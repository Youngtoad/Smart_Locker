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

#include <linux/module.h> 
#include <linux/fs.h> 
#include <linux/uaccess.h> 
#include <linux/slab.h> 

#include <asm/mach/map.h> 
#include <asm/uaccess.h> 

#define KEYPAD_MAJOR_NUMBER   498
#define KEYPAD_DEV_NAME      "keypad_dev" 

#define GPIO_BASE_ADDRESS   0x3F200000

#define GPFSEL1             0x04
#define GPFSEL2             0x08
#define GPLEV0              0x34
#define GPSET0               0x1C
#define GPCLR0               0x28

#define ROWS 4
#define COLS 3

static void __iomem * gpio_base;


volatile unsigned int * gpfsel0;
volatile unsigned int * gpfsel1;
volatile unsigned int * gpfsel2; 
volatile unsigned int * gplev0; 
volatile unsigned int * gpset0; 
volatile unsigned int * gpclr0; 

int rows[4] = {2, 12, 4, 27};
int cols[4] = {20, 11, 9, 10};
char pad[4][4] = {
    {'1', '2', '3', 'S'},
    {'4', '5', '6', 'G'},
    {'7', '8', '9', 'L'},
    {'E', '0', 'E', 'P'}
};
char ninput = '\0';

void clearcol(void);
void clearrow(void);

int keypad_open(struct inode * inode, struct file * filp){
    printk(KERN_ALERT "keypad driver open\n"); 

    gpio_base = ioremap(GPIO_BASE_ADDRESS, 0x60);
    gpfsel0 = (volatile unsigned int *)(gpio_base); 
    gpfsel1 = (volatile unsigned int *)(gpio_base + GPFSEL1);
    gpfsel2 = (volatile unsigned int *)(gpio_base + GPFSEL2);
    gplev0 = (volatile unsigned int *)(gpio_base + GPLEV0);
    gpset0 = (volatile unsigned int *)(gpio_base + GPSET0);
    gpclr0 = (volatile unsigned int *)(gpio_base + GPCLR0);
    
    
   /* *gpfsel2 &= (0 << 0);
    *gpfsel1 &= (0 << 3);
    *gpfsel0 &= (0 << 27);
    *gpfsel1 &= (0 << 0); */
    
    
    *gpfsel2 |= (1 << 21);
    *gpfsel0 |= (1 << 12);
    *gpfsel1 |= (1 << 6);
    *gpfsel0 |= (1 << 6);
    
    clearrow();
    clearcol();
    
    return 0; 
}

int keypad_release(struct inode * inode, struct file * filp) { 
    printk(KERN_ALERT "keypad driver close\n"); 
    iounmap((void *)gpio_base); 
    return 0; 
}

void clearrow(void)
{
    *gpclr0 |= (1 << rows[0]);
    *gpclr0 |= (1 << rows[1]);
    *gpclr0 |= (1 << rows[2]);
    *gpclr0 |= (1 << rows[3]);
    return ;
}

void clearcol(void)
{
    *gpclr0 |= (1 << cols[0]);
    *gpclr0 |= (1 << cols[1]);
    *gpclr0 |= (1 << cols[2]);
    *gpclr0 |= (1 << cols[3]);
    return ;
}

ssize_t keypad_read(struct file * flip, char * buf, size_t count, loff_t * f_pos)
{
    int i;
    int n;
    
    for(i = 0; i < 4; i++)
    {
        mdelay(10);
        *gpset0 |= (1 << rows[i]);
        for(n = 0; n < 4; n++)
        {
            mdelay(2);
            if((*gplev0 & (1 << cols[n])) != 0)
            {
                printk("ROW : %d, COL : %d, value : %c", i+1, n+1, pad[i][n]);
                clearrow();
                sprintf(buf, "%c", pad[i][n]);
                return count;
                
                
            }
        }
        mdelay(20);
        sprintf(buf, "%c", ninput);
        clearrow();
    }
    printk("No Input");
    return count; 
}

static struct file_operations keypad_fops = { 
    .owner = THIS_MODULE, 
    .open = keypad_open, 
    .release = keypad_release, 
    .read = keypad_read
}; 

int __init keypad_init (void) { 
    if(register_chrdev(KEYPAD_MAJOR_NUMBER, KEYPAD_DEV_NAME, &keypad_fops) < 0)
        printk(KERN_ALERT "keypad driver initalization faibutton\n"); 
    else 
        printk(KERN_ALERT "keypad driver initalization succeed\n");
    
    return 0; 
}

void __exit keypad_exit(void){ 
    unregister_chrdev(KEYPAD_MAJOR_NUMBER, KEYPAD_DEV_NAME); 
    printk(KERN_ALERT "button driver exit"); 
}

module_init(keypad_init); 
module_exit(keypad_exit);  

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yoon JoonSUng"); 
MODULE_DESCRIPTION("4x3 keypad"); 
