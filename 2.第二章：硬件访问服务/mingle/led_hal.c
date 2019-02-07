/*1.实现一个名为HMI的hw_module_t结构体*/

/*2.实现一个open函数，他返回一个led_device_t结构体*/

/*3.实现led_device_t结构体*/

//参考hardware/libhardware/modules/vivrator/vivrator.c
#define LOG_TAG "LedHal"

#include <hardware/hardware.h>

#include <cutils/log.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>

#include <utils/Log.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <hardware/led_hal.h>



static int fd;

static int led_close(struct hw_device_t* device)
{
	close(fd);
	return 0;
}

static int led_open(struct led_device_t* dev)
{
	fd = open("/dev/leds_drv", O_RDWR);
	ALOGE("ledOpen : %d", fd);
	if (fd >= 0)
		return 0;
	else
		return -1;
}static int led_ctrl(struct led_device_t* dev, int which, int status)
{
	int ret = ioctl(fd, which, status);
	ALOGE("ledCtrl : %d, %d, %d", which, status, ret);
	return ret;
}

struct led_device_t led_dev = {
	.common = {
		.tag = HARDWARE_DEVICE_TAG,
		.close = led_close, 
	},
	.led_open = led_open,
	.led_ctrl = led_ctrl,
};


static int led_device_open(const hw_module_t* module, const char* id,
hw_device_t** device) 
{
	*device = &led_dev;
	return 0;
}


static struct hw_module_methods_t led_module_methods = {
	/*该函数，com_android_server_LedService.cpp文件中调用
	hw_get_module时会被调用，其主要目的是返回我们自定义的
	结构体，供JNI操作*/
    .open = led_device_open, 
};

//使用HAL_MODULE_INFO_SYM定义一个结构体（HMI）
struct hw_module_t HAL_MODULE_INFO_SYM = {
	.tag = HARDWARE_MODULE_TAG,
    .id = "led",
    .methods = &led_module_methods,
};