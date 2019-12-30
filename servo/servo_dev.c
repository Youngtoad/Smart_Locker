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

#define SERVO_MAJOR_NUMBER   504
#define SERVO_DEV_NAME      "servo_dev" 

#define GPIO_BASE_ADDRESS   0x3F200000

#define GPFSEL1 0x04
#define GPSET0	0x1C
#define GPCLR0	0x28

#define PWM_RANGE 20


#define MOTOR1_RIGHT 0
#define MOTOR1_LEFT 1
#define MOTOR2_RIGHT 4
#define MOTOR2_LEFT 3

#define MOTOR1 18
#define MOTOR2 19

#define RIGHT 0
#define LEFT 1

#define DEGREE90 3

static void __iomem * gpio_base;
volatile unsigned int * gpfsel1;
volatile unsigned int * gpset0;
volatile unsigned int * gpclr0;

void setServo(int cmd, int motor)
{
    int i;
      if(motor == 1)
      {
      
      if(cmd == 0)
      {        
        printk( "PIN%d RIGHT\n", motor);
        for(i = 0; i < DEGREE90 ; i++)
        {
          *gpset0 |= (1 << motor);
          usleep_range(1400, 1400);
          *gpclr0 |= (1 << motor);
          mdelay(PWM_RANGE-1);
        }
      }

      else if(cmd == 1)
      {
        printk( "PIN%d LEFT\n", motor);
        for(i=0; i < DEGREE90; i++)
        {
          *gpset0 |= (1 << motor);
          usleep_range(1600, 1600);
          *gpclr0 |= (1 << motor);
          mdelay(PWM_RANGE-1);
        }
          
       }
    }
    else
    {
        if(cmd == 0)
      {        
        printk( "PIN%d RIGHT\n", motor);
        for(i = 0; i < DEGREE90 + 8; i++)
        {
          *gpset0 |= (1 << motor);
          usleep_range(1300, 1300);
          *gpclr0 |= (1 << motor);
          mdelay(PWM_RANGE-1);
        }
      }
      
      else if(cmd == 1)
      {
        printk( "PIN%d LEFT\n", motor);
        for(i=0; i < DEGREE90 + 6; i++)
        {
          *gpset0 |= (1 << motor);
          usleep_range(1700, 1700);
          *gpclr0 |= (1 << motor);
          mdelay(PWM_RANGE-1);
        }
          
       }
   }
      
}

int servo_open(struct inode * inode, struct file * filp)
{
    printk(KERN_ALERT "servo driver open\n"); 
    gpio_base = ioremap(GPIO_BASE_ADDRESS, 0xD000);
    
    
    gpfsel1 = (volatile unsigned int *)(gpio_base + GPFSEL1);
    gpset0 = (volatile unsigned int *)(gpio_base + GPSET0);
    gpclr0 = (volatile unsigned int *)(gpio_base + GPCLR0);
    
    *gpfsel1 |= (1 << 24);
    *gpfsel1 |= (1 << 27);
    udelay(500);
    
    //setServo(RIGHT, MOTOR2);
  
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
	
        case 4: 
        printk(KERN_ALERT "CMD : %d, MOTOR2 RIGHT\n", cmd);
        setServo(RIGHT, MOTOR2);
        break;
        
        case MOTOR2_LEFT: 
        printk(KERN_ALERT "CMD : %d, MOTOR2 LEFT\n", cmd);
        setServo(LEFT, MOTOR2);
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
