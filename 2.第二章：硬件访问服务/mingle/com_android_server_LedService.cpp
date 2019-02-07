#define LOG_TAG "LedService"

#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"

#include <utils/misc.h>
#include <utils/Log.h>
#include <hardware/led_hal.h>

#include <stdio.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <hardware/led_hal.h>

namespace android
{
	static led_device_t* led_device;

	jint ledOpen(JNIEnv *env, jobject cls)
	{
		int err;
		hw_module_t* module;
		hw_device_t* device;

		
		
		
		/*1. hw_get_module*/
		err = hw_get_module("led", (hw_module_t const**)&module);
		ALOGE("native ledOpen");
		if(err == 0){
			/*2. get device:  module->methods->open*/
			err = module->methods->open(module, NULL, &device);
			if (err == 0) {
				led_device = (led_device_t*)device;
				
				/*3. call led_open*/
				return led_device->led_open(led_device);
			} else {
				return -1;
			}
		}
		return -1;		
	}

	void ledClose(JNIEnv *env, jobject cls)
	{
		ALOGE("native ledClose");
	}

	jint ledCtrl(JNIEnv *env, jobject cls, jint which, jint status)
	{
		ALOGE("native ledCtrl : %d, %d", which, status);
		return led_device->led_ctrl(led_device, which, status);
	}

	static const JNINativeMethod method_table[] = {
		{"native_ledOpen", "()I", (void *)ledOpen},
		{"native_ledClose", "()V", (void *)ledClose},
		{"native_ledCtrl", "(II)I", (void *)ledCtrl},
	};

	
	int register_android_server_LedService(JNIEnv *env)
	{
		return jniRegisterNativeMethods(env, "com/android/server/LedService",
				method_table, NELEM(method_table));
	}

};