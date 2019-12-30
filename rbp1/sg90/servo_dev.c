#include <linux/init.h> 
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/fs.h> 
#include <linux/uaccess.h> 
#include <linux/slab.h>

#include <asm/mach/map.h> 
#include <asm/uaccess.h> 

#include <linux/delay.h>
#include <linux/timer.h>

#define SERVO_MAJOR_NUMBER   508
#define SERVO_DEV_NAME      "servo_dev" 

#define GPIO_BASE_ADDRESS   0x3F200000

#define GPFSEL2 0x08
#define GPSET0   0x1C
#define GPCLR0   0x28

#define PWM_RANGE 20


#define MOTOR1_RIGHT 0
#define MOTOR1_LEFT 1

#define MOTOR1 25

#define RIGHT 0
#define LEFT 1

#define DEGREE90 7

static void __iomem * gpio_base;
volatile unsigned int * gpfsel2;
volatile unsigned int * gpset0;
volatile unsigned int * gpclr0;

void setServo(int cmd, int motor)
{
    int i;

  if(cmd == 0)
  {        
    printk( "PIN%d RIGHT\n", motor);
    for(i = 0; i < DEGREE90 + 2; i++)
    {
      *gpset0 |= (1 << motor);
      usleep_range(1000, 1000);
      *gpclr0 |= (1 << motor);
      mdelay(20);
    }
  }

  else if(cmd == 1)
  {
    printk( "PIN%d LEFT\n", motor);
    for(i=0; i < DEGREE90; i++)
    {
      *gpset0 |= (1 << motor);
      usleep_range(2000, 2000);
      *gpclr0 |= (1 << motor);
      mdelay(PWM_RANGE);
    }
      
   }
   
}

int servo_open(struct inode * inode, struct file * filp)
{
    printk(KERN_ALERT "servo driver open\n"); 
    gpio_base = ioremap(GPIO_BASE_ADDRESS, 0xFF);
    
    
    gpfsel2 = (volatile unsigned int *)(gpio_base + GPFSEL2);
    gpset0 = (volatile unsigned int *)(gpio_base + GPSET0);
    gpclr0 = (volatile unsigned int *)(gpio_base + GPCLR0);
    
    *gpfsel2 |= (1 << 18);
    udelay(500);
    
   // setServo(LEFT, MOTOR1);
  
    return 0;
    

}

int servo_release(struct inode * inode, struct file * filp) { 
    printk(KERN_ALERT "servo driver close\n");
    iounmap((void *)gpio_base); 
    return 0; 
}

long servo_ioctl(struct file * filp, unsigned int cmd, unsigned long arg)
{ 
    switch (cmd){ 
        case MOTOR1_RIGHT: 
        printk(KERN_ALERT "CMD : %d, MOTOR1 RIGHT\n", cmd);
        setServo(RIGHT, MOTOR1);
        break; 
    
        case MOTOR1_LEFT: 
        printk(KERN_ALERT "CMD : %d, MOTOR1 LEFT\n", cmd);
        setServo(LEFT, MOTOR1);
        break;
   
   default :
        printk(KERN_ALERT "ioctl : command error\n");
    }
    
    return 0; 
}

static struct file_operations servo_fops = { 
    .owner = THIS_MODULE, 
    .open = servo_open, 
    .release = servo_release,
    .unlocked_ioctl = servo_ioctl,
};

int __init servo_init (void) { 
    if(register_chrdev(SERVO_MAJOR_NUMBER, SERVO_DEV_NAME, &servo_fops) < 0)
        printk(KERN_ALERT "SERVO driver initalization failed\n"); 
    else 
        printk(KERN_ALERT "SERVO driver initalization succeed\n");
    
    return 0; 
}

void __exit servo_exit(void){ 
    unregister_chrdev(SERVO_MAJOR_NUMBER, SERVO_DEV_NAME); 
    printk(KERN_ALERT "SERVO driver exit"); 
}

module_init(servo_init);
module_exit(servo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CLE");
MODULE_DESCRIPTION("SERVO MOTOR FS5106R DEVICE DRIVER");
