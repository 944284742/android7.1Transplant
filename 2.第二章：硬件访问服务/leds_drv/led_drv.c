#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <linux/miscdevice.h>
#include <linux/ioctl.h>
#include <linux/delay.h>
#include <linux/gpio.h>

static const int led1_gpio = (32*0 + 8*1 + 4);
static const int led2_gpio = (32*0 + 8*1 + 0);

static int gec3399_leds_open(struct inode *inode, struct file *file)
{
	return 0;
}

static long gec3399_leds_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	//printk("cmd = %u    arg = %lu\n",cmd,arg);
	switch(cmd)
	{
		case 0:	switch(arg)
				{
					case 0: gpio_set_value(led1_gpio,0); break;
					case 1:	gpio_set_value(led1_gpio,1); break;
				}break;
		
		case 1: switch(arg)
				{
					case 0: gpio_set_value(led2_gpio,0); break;
					case 1:	gpio_set_value(led2_gpio,1); break;			
				}break;
	}
	return 0;
}

static int gec3399_leds_release(struct inode *inode, struct file *file)
{
	return 0;
}

static const struct file_operations gec3399_leds_fops = {
	.owner = THIS_MODULE,
	.open = gec3399_leds_open,
	.unlocked_ioctl = gec3399_leds_ioctl,
	.release = gec3399_leds_release,
}; 

static struct miscdevice gec3399_leds_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &gec3399_leds_fops,
	.name   = "leds_drv",	
}; 

static int __init gec3399_leds_init(void)
{
    int ret;
	ret =  misc_register(&gec3399_leds_misc);   //注册字符设备
	if(ret < 0){
		printk("misc register error\n");
		goto err0;		
	}
	ret = gpio_request(led1_gpio,"led1_gpio");    //申请led1_gpio引脚为GPIO模式
	if(ret < 0){
		printk("gpio_request led1_gpio error\n");
		goto err1;
	}
	
	ret = gpio_request(led2_gpio,"led2_gpio");    //申请led2_gpio引脚为GPIO模式
	if(ret < 0){
		printk("gpio_request  led2_gpio error\n");
		goto err2;
	}
	
	ret = gpio_direction_output(led1_gpio,1);      //初始划LED1为熄灭状态
	if(ret < 0){
		printk("gpio direction output  led1_gpio error\n");
		goto err3;
	}

	ret = gpio_direction_output(led2_gpio,1);      //初始划LED2为熄灭状态
	if(ret < 0){
		printk("gpio direction output  led2_gpio error\n");
		goto err3;
	}

    return 0;
	
err3:
	gpio_free(led2_gpio);
err2:
	gpio_free(led1_gpio);
err1:
	misc_deregister(&gec3399_leds_misc);
err0:
    return ret;

}


static void __exit gec3399_leds_exit(void)
{
	misc_deregister(&gec3399_leds_misc);
}

module_init(gec3399_leds_init);
module_exit(gec3399_leds_exit);

MODULE_LICENSE("GPL");