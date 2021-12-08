#ifndef __AML_HDMI_GPIO_H__
#define __AML_HDMI_GPIO_H__

#include <linux/delay.h>
#include <linux/of_gpio.h>
#include <linux/io.h>

#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/of.h>
#include <linux/amlogic/aml_gpio_consumer.h>
#include <linux/switch.h>
#include <linux/pinctrl/consumer.h>

#include <linux/time.h>


typedef unsigned char   u_int8;
typedef unsigned short  u_int16;	
typedef unsigned long 	u_int32;



#if 1
#define pr_dbg(args...) printk(KERN_ALERT "HDMI_GPIO: " args)
#else
#define pr_dbg(args...)
#endif

#define pr_error(args...) printk(KERN_ALERT "HDMI_GPIO: " args)

#ifndef CONFIG_OF
#define CONFIG_OF
#endif



#define DEV_NAME          "hdmi_gpio"


typedef struct _tag_hdmi_gpio_dev{
	int data_pin_1;    
	int data_pin_2;
	struct class sysfs;
}HDMI_GPIO_DEV;

#endif
