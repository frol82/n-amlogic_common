/*
 * linux/drivers/input/vfd/vfd.c
 *
 * VFD Driver
 *
 * Copyright (C) 2011 Amlogic Corporation
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * author :   tiejun_peng
 *
 *
 * - by leon@2016-04-18 add surpport of FD628 for Amlogic905 MBox platform
 */

 /*
 * !!caution:
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/types.h>
#include <linux/input.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/mutex.h>
#include <linux/errno.h>
#include <asm/irq.h>
#include <asm/io.h>
/*
#include <mach/am_regs.h>
#include <mach/pinmux.h>
*/
#include <linux/major.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include <linux/gpio.h>
#include <linux/amlogic/aml_gpio_consumer.h>
#include <linux/of_gpio.h>
#include <linux/amlogic/iomap.h>
#include <linux/input/vfd.h>

#include "vfd_comm.h"

#if 0
/**
 * by leon@2016.04.18
 * GPIOY_13 => CLK
 * GPIOY_14 => DAT
 * GPIOY_15 => STB
 // 				 	OEN 		OUT 			IN
 // GPIOY_13~0  	bit[16:0]	bit[16:0]		bit[16:0]
 */
#define FD628_AML_GPIO_CLK 			(13)
#define FD628_AML_GPIO_DAT			(14)
#define FD628_AML_GPIO_STB			(15)
#define FD628_AML_GPIO_OEN 			P_PREG_PAD_GPIO1_EN_N
#define FD628_AML_GPIO_I 			P_PREG_PAD_GPIO1_I
#define FD628_AML_GPIO_O 			P_PREG_PAD_GPIO1_O
#endif

#define VFD_DEV_NAME 	"vfd_keypad"

struct aml_vfd_gpio {
	unsigned int pin;
	unsigned int active_low;
	unsigned int state;
};

struct vfd {
	struct input_dev *input;
	struct timer_list timer;
	char config_name[20];
	struct class *config_class;
	struct device *config_dev;
	int config_major;
	unsigned int cur_keycode;
	unsigned int debug_enable;
	char set_led_value[12];
	char cur_led_value[12];
	int key_num;

	struct vfd_key *key;
	char greenLed[2];

#ifdef CONFIG_VFD_FD628SW
	/* 时钟信号接口 */
	struct aml_vfd_gpio fd628_clk;
	/* 传输控制接口 */
	struct aml_vfd_gpio fd628_stb;
	/* 数据读写接口 */
	struct aml_vfd_gpio fd628_dat;
#endif
};

type_vfd_printk vfd_input_dbg;

static struct vfd *gp_vfd=NULL;

static DEFINE_MUTEX(led_set_mutex);

#ifdef CONFIG_VFD_SM1628
extern void MDrv_TM1623_StandBy(void);
extern void MDrv_TM1623_Write_Adr_Data(unsigned char addr, unsigned char value);
#endif
#ifdef CONFIG_VFD_FD650
extern void MDrv_FrontPnl_Suspend(void);
#endif

unsigned char powerstate = 0;

int vfd_printk(const char *fmt, ...)
{
    va_list args;
    int r;

    if (gp_vfd->debug_enable==0)  return 0;
    va_start(args, fmt);
    r = vprintk(fmt, args);
    va_end(args);
    return r;
}

/*****************************************************************
**
** func : hardware init
**       in this function will do pin configuration and and initialize for hardware
**
********************************************************************/

static int set_led_string( char *string)
{
		sprintf(gp_vfd->set_led_value, "%.11s", string);
		gp_vfd->set_led_value[11] = '\0';
		return 0;
}

static ssize_t vfd_key_get(struct device *dev,
                   struct device_attribute *attr, char *buf)
{
    return sprintf(buf, "%u\n", gp_vfd->cur_keycode);
}

static ssize_t vfd_led_show(struct device *dev,
                   struct device_attribute *attr, char *buf)
{
    return sprintf(buf, "%s\n", gp_vfd->cur_led_value);
}

//control var by sysfs .
static ssize_t vfd_led_store(struct device *dev, struct device_attribute *attr,
                    const char *buf, size_t count)
{
		char value[12];
		if (sscanf(buf, "%11s", value) != 1)
			return -EINVAL;
		vfd_printk("function[%s]line%d :vfd led display: %s \n",__FUNCTION__,__LINE__, value);
		mutex_lock(&led_set_mutex);
		set_led_string(value);
		mutex_unlock(&led_set_mutex);

    return strlen(buf);
}

static ssize_t vfd_greenled_show(struct device *dev,
                   struct device_attribute *attr, char *buf)
{
    return sprintf(buf, "%s\n", gp_vfd->greenLed);
}
int greenLedFlag=0;
//control var by sysfs .
static ssize_t vfd_greenled_store(struct device *dev, struct device_attribute *attr,
                    const char *buf, size_t count)
{
	char *endp;
	char value[2];
	greenLedFlag = simple_strtol(buf, &endp, 0);
	mutex_lock(&led_set_mutex);

	#if 1
	if (sscanf(buf, "%1s", value) != 1)
		return -1 ;

	sprintf(gp_vfd->greenLed, "%.1s", value);
	gp_vfd->greenLed[1] = '\0';
	#else

	gp_vfd->greenLed[0] = buf[0];
	gp_vfd->greenLed[1] = '\0';
	#endif
	Led_Show_lockflg(greenLedFlag);
	mutex_unlock(&led_set_mutex);

	return strlen(buf);
}
static DEVICE_ATTR(key, S_IRUGO | S_IWUSR, vfd_key_get, NULL);
static DEVICE_ATTR(led, S_IRUGO | S_IWUSR, vfd_led_show, vfd_led_store);
static DEVICE_ATTR(greenled, S_IRUGO | S_IWUSR, vfd_greenled_show, vfd_greenled_store);

static struct vfd_key vfd_key[] = {
        {KEY_POWER,         "power",    0x04},
		{KEY_VOLUMEDOWN,        "volumn-up",    0x0d},
		{KEY_VOLUMEUP,      "volumn-down",      0x02},
		{KEY_MENU,          "menu",     0x0b},
		{KEY_UP,            "up",       0x09},
		{KEY_DOWN,          "down",     0x0c},
		{KEY_ENTER,         "enter",    0x01},
};
/*******************************************************************
**Hw code
********************************************************************/
static int vfd_stb_pin_set_value(int value)
{
#ifdef CONFIG_VFD_FD628SW
	if(gp_vfd){
		gpio_direction_output(gp_vfd->fd628_stb.pin , value>0 ? 1 : 0);
		gpio_set_value(gp_vfd->fd628_stb.pin , value>0 ? 1 : 0);
	}
#endif
	return 0;
}

/**
 * 设置clk gpio值为0或者1
 * @method vfd_clock_pin_set_value
 * @param  value                   0/1
 * @return                         invalid
 */
static int vfd_clock_pin_set_value(int value)
{
#ifdef CONFIG_VFD_FD628SW
	if(gp_vfd){
		gpio_direction_output(gp_vfd->fd628_clk.pin , value>0 ? 1 : 0);
		gpio_set_value(gp_vfd->fd628_clk.pin , value>0 ? 1 : 0);
	}
#endif
    return 0;
}

/**
 * 设置dat输出值为0或者1
 * @method vfd_do_pin_set_value
 * @param  value                0/1
 * @return                      invalid
 */
static int vfd_do_pin_set_value(int value)
{
#ifdef CONFIG_VFD_FD628SW
	if(gp_vfd){
		gpio_direction_output(gp_vfd->fd628_dat.pin , value>0 ? 1 : 0);
		gpio_set_value(gp_vfd->fd628_dat.pin , value>0 ? 1 : 0);
	}
#endif
    return 0;
}

/**
 * 获取dat输入值
 * @method vfd_di_pin_get_value
 * @return                      invalid
 */
static int vfd_di_pin_get_value(void)
{
	int ret = 0;
#ifdef CONFIG_VFD_FD628SW
	if(gp_vfd){
		gpio_direction_input(gp_vfd->fd628_dat.pin);
		ret = gpio_get_value(gp_vfd->fd628_dat.pin);
	}
#endif
	return ret;
}

/**
 * 设置stb为输入/输出
 * @method vfd_set_stb_gpio
 * @param  value            1输出，0输入
 * @return                  [description]
 */
static int vfd_set_stb_gpio(int value)
{
#ifdef CONFIG_VFD_FD628SW
	if(gp_vfd){
		if (value) {
			gpio_direction_output(gp_vfd->fd628_stb.pin , value>0 ? 1 : 0);
		} else {
			gpio_direction_input(gp_vfd->fd628_stb.pin);
		}
	}
#endif
	return 0;
}

/**
 * 设置dat为输入/输出
 * @method vfd_set_dio_gpio
 * @param  value            1输出，0输入
 * @return                  [description]
 */
static int vfd_set_dio_gpio(int value)
{
#ifdef CONFIG_VFD_FD628SW
	if(gp_vfd){
		if (value) {
			gpio_direction_output(gp_vfd->fd628_dat.pin , value>0 ? 1 : 0);
		} else {
			gpio_direction_input(gp_vfd->fd628_dat.pin);
		}
	}
#endif
	return 0;
}

/**
 * 设置clk为输入/输出
 * @method vfd_set_clk_gpio
 * @param  value            1输出，0输入
 * @return                  [description]
 */
static int vfd_set_clk_gpio(int value)
{
#ifdef CONFIG_VFD_FD628SW
	if(gp_vfd){
		if (value) {
			gpio_direction_output(gp_vfd->fd628_clk.pin , value>0 ? 1 : 0);
		} else {
			gpio_direction_input(gp_vfd->fd628_clk.pin);
		}
	}
#endif
	return 0;
}

/*******************************************************************
**stand code
********************************************************************/
static struct vfd_platform_data vfd_pdata = {
        .key = &vfd_key[0],
        .key_num = ARRAY_SIZE(vfd_key),

		//vfd_set_stb_pin_value = pvfd_platform_data->set_stb_pin_value;
		//vfd_set_clock_pin_value = pvfd_platform_data->set_clock_pin_value;
		//vfd_set_do_pin_value = pvfd_platform_data->set_do_pin_value;
		//vfd_get_di_pin_value = pvfd_platform_data->get_di_pin_value;
		//vfd_set_stb_gpio = pvfd_platform_data->set_stb_gpio;
		//vfd_set_dio_gpio = pvfd_platform_data->set_dio_gpio;
		//vfd_set_clk_gpio = pvfd_platform_data->set_clk_gpio;

        .set_stb_pin_value = vfd_stb_pin_set_value,
        .set_clock_pin_value = vfd_clock_pin_set_value,
        .set_do_pin_value = vfd_do_pin_set_value,
        .get_di_pin_value = vfd_di_pin_get_value,
		.set_stb_gpio = vfd_set_stb_gpio,
		.set_dio_gpio = vfd_set_dio_gpio,
		.set_clk_gpio = vfd_set_clk_gpio,
};

static int vfd_search_key(struct vfd *vfd)
{
		struct vfd_key *key = vfd->key;
		int value,j;

		value = get_vfd_key_value();
		if (value <= 0) {
			return 0;
		}
		vfd_printk("function<%s> line <%d> get VFD key value : [%d]  \n",__FUNCTION__,__LINE__,value);
		for (j=0; j<vfd->key_num; j++) {
			if ((value == key->value)) {
				return key->code;
			}
			key++;
		}
	return 0;
}

static void vfd_work(struct vfd *vfd)
{
		int code = vfd_search_key(vfd);

		if (vfd->cur_keycode) {
			if (!code) {
				vfd_printk("vfd report key code: [%d] released.\n", vfd->cur_keycode);
				input_report_key(vfd->input, vfd->cur_keycode, 0);
				input_sync(vfd->input);
				vfd->cur_keycode = 0;
			}
			else {
			// detect another key while pressed
			}
		}
		else {
			if (code) {
				vfd->cur_keycode = code;
				vfd_printk("vfd report key code: [%d] pressed.\n", vfd->cur_keycode);
				input_report_key(vfd->input, vfd->cur_keycode, 1);
				input_sync(vfd->input);
			}
		}
}
int cnt =0;
void vfd_timer_sr(unsigned long data)
{
		struct vfd *vfd_data=(struct vfd *)data;
#ifndef CONFIG_VFD_NO_KEY_INPUT
		vfd_work(vfd_data);
#endif
		if(strcmp(gp_vfd->cur_led_value,gp_vfd->set_led_value)) {
				vfd_printk("function[%s] line %d current LED value :%s ,set LED value :%s \n",
					__FUNCTION__,__LINE__,gp_vfd->cur_led_value,gp_vfd->set_led_value);
				strcpy(gp_vfd->cur_led_value,gp_vfd->set_led_value);//vfd->cur_ledcode = vfd->set_ledcode;
				set_vfd_led_value(gp_vfd->cur_led_value);
				cnt =0;
			}else{
					cnt++;
					if(cnt==300){
						set_vfd_led_value(gp_vfd->cur_led_value);
						//printk("vfd set value again after: %d\n", cnt);//1 min
						cnt=0;
					}
			}
		mod_timer(&vfd_data->timer,jiffies+msecs_to_jiffies(200));
}

static int vfd_config_open(struct inode *inode, struct file *file)
{
    file->private_data = gp_vfd;
    return 0;
}

static int vfd_config_release(struct inode *inode, struct file *file)
{
    file->private_data=NULL;
    return 0;
}

static const struct file_operations vfd_fops = {
    .owner      = THIS_MODULE,
    .open       = vfd_config_open,
   // .ioctl      = NULL,
    .unlocked_ioctl =NULL,
    .release    = vfd_config_release,
};

static int  register_vfd_dev(struct vfd  *vfd)
{
    int ret=0;
    strcpy(vfd->config_name,"aml_vfd");
    ret=register_chrdev(0,vfd->config_name,&vfd_fops);
    if(ret <=0)
    {
        printk("register char dev vfd error\r\n");
        return  ret ;
    }
    vfd->config_major=ret;
    printk("vfd config major:%d\r\n",ret);
    vfd->config_class=class_create(THIS_MODULE,vfd->config_name);
    vfd->config_dev=device_create(vfd->config_class,NULL,MKDEV(vfd->config_major,0),NULL,vfd->config_name);
    return ret;
}

static int vfd_dt_parse(struct platform_device *pdev)
{
	struct device_node *node;
	struct vfd *vfd;


	vfd = platform_get_drvdata(pdev);
	node = pdev->dev.of_node;
	if (!node) {
		dev_err(&pdev->dev, "failed to find node for %s\n", VFD_DEV_NAME);
		return -ENODEV;
	}

#ifdef CONFIG_VFD_FD628SW
	struct gpio_desc *clk_desc = NULL;
	struct gpio_desc *dat_desc = NULL;
	struct gpio_desc *stb_desc = NULL;

	/*
	fd628_gpio_clk = <&gpio       GPIOY_13       GPIO_ACTIVE_HIGH>;
	fd628_gpio_dat = <&gpio       GPIOY_14       GPIO_ACTIVE_HIGH>;
	fd628_gpio_stb = <&gpio       GPIOY_15       GPIO_ACTIVE_HIGH>;
	*/

	clk_desc = of_get_named_gpiod_flags(node, "fd628_gpio_clk", 0, NULL);
	vfd->fd628_clk.pin = desc_to_gpio(clk_desc);
	gpio_request(vfd->fd628_clk.pin, VFD_DEV_NAME);

	dat_desc = of_get_named_gpiod_flags(node, "fd628_gpio_dat", 0, NULL);
	vfd->fd628_dat.pin = desc_to_gpio(dat_desc);
	gpio_request(vfd->fd628_dat.pin, VFD_DEV_NAME);

	stb_desc = of_get_named_gpiod_flags(node, "fd628_gpio_stb", 0, NULL);
	vfd->fd628_stb.pin = desc_to_gpio(stb_desc);
	gpio_request(vfd->fd628_stb.pin, VFD_DEV_NAME);

	printk(KERN_INFO "VFD Driver request fd628_gpio_clk/ \n");
#endif
	return 0;
}



static int __init vfd_probe(struct platform_device *pdev)
{
    struct vfd *vfd;
    struct input_dev *input_dev;
    int ret;
	int i;
    struct vfd_key *temp_key;
    struct vfd_platform_data *pdata = &vfd_pdata;

    if (!pdata) {
        dev_err(&pdev->dev, "platform data is required!\n");
        return -EINVAL;
    }

    if(hardware_init(pdata)) {
		dev_err(&pdev->dev, "hardware initial failed!\n");
		return -EINVAL;
    }

	dev_notice(&pdev->dev, "vfd driver probe");
    vfd = kzalloc(sizeof(struct vfd), GFP_KERNEL);
    input_dev = input_allocate_device();
    if (!vfd || !input_dev)
        goto err1;

	memset(vfd, 0, sizeof(struct vfd));

    gp_vfd = vfd;
	/* FIXME: 关闭调试开关 */
    vfd->debug_enable = 1;
    vfd_input_dbg = vfd_printk;
	platform_set_drvdata(pdev, vfd);

	/* parse dt param */
	ret = vfd_dt_parse(pdev);
	if (ret){
		dev_err(&pdev->dev, "vfd dt parse failed");
		return ret;
	}
	vfd->input = input_dev;
	vfd->cur_keycode = 0;
	strcpy(vfd->set_led_value, "boot");
	strcpy(vfd->cur_led_value, "");

	setup_timer(&vfd->timer, vfd_timer_sr, (unsigned long)vfd) ;
	mod_timer(&vfd->timer, jiffies+msecs_to_jiffies(100));

	/* setup input device */
    set_bit(EV_KEY, input_dev->evbit);
    set_bit(EV_REP, input_dev->evbit);

    vfd->key = pdata->key;
    vfd->key_num = pdata->key_num;
    temp_key = pdata->key;
	for (i=0; i<vfd->key_num; i++) {
		set_bit(temp_key->code, input_dev->keybit);
		printk(KERN_INFO "%s vfd key(%d) registed.\n", temp_key->name, temp_key->code);
        temp_key++;
    }
	ret = device_create_file(&pdev->dev, &dev_attr_key);
	if (ret < 0)
        goto err1;

	ret = device_create_file(&pdev->dev, &dev_attr_greenled);
	if (ret < 0)
		goto err1;

	ret = device_create_file(&pdev->dev, &dev_attr_led);
	if (ret < 0)
        goto err1;
    vfd_input_dbg("device_create_file completed \r\n");

    input_dev->name = VFD_DEV_NAME;
    input_dev->phys = "vfd_keypad/input0";
    input_dev->dev.parent = &pdev->dev;

    input_dev->id.bustype = BUS_ISA;
    input_dev->id.vendor = 0x0001;
    input_dev->id.product = 0x0001;
    input_dev->id.version = 0x0100;

    input_dev->rep[REP_DELAY]=0xffffffff;
    input_dev->rep[REP_PERIOD]=0xffffffff;

    input_dev->keycodesize = sizeof(unsigned short);
    input_dev->keycodemax = 0x1ff;

    ret = input_register_device(vfd->input);
    if (ret < 0) {
        dev_err(&pdev->dev, "Unable to register vfdkeypad input device\n");
        goto err2;
    }

    dev_notice(&pdev->dev, "vfd input_register_device completed \r\n");

    register_vfd_dev(gp_vfd);

	printk(KERN_INFO "VFD Driver probed\n");

    return 0;
/*
err3:
    input_unregister_device(vfd->input);
    input_dev = NULL;
*/
err2:
    device_remove_file(&pdev->dev, &dev_attr_key);
	device_remove_file(&pdev->dev, &dev_attr_greenled);
    device_remove_file(&pdev->dev, &dev_attr_led);
err1:
    kfree(vfd);
    input_free_device(input_dev);

    return -EINVAL;
}

static int vfd_remove(struct platform_device *pdev)
{
	struct vfd *vfd = platform_get_drvdata(pdev);

#ifdef CONFIG_VFD_FD628SW
	if(vfd->fd628_clk.pin)
		gpio_free(vfd->fd628_clk.pin);
	if(vfd->fd628_dat.pin)
		gpio_free(vfd->fd628_dat.pin);
	if(vfd->fd628_stb.pin)
		gpio_free(vfd->fd628_stb.pin);
#endif

	/* unregister everything */
	input_unregister_device(vfd->input);
	device_remove_file(&pdev->dev, &dev_attr_key);
	device_remove_file(&pdev->dev, &dev_attr_greenled);
    device_remove_file(&pdev->dev, &dev_attr_led);

	input_free_device(vfd->input);
	unregister_chrdev(vfd->config_major,vfd->config_name);
	if(vfd->config_class)
	{
		if(vfd->config_dev)
			device_destroy(vfd->config_class,MKDEV(vfd->config_major,0));
		class_destroy(vfd->config_class);
	}
	kfree(vfd);
	gp_vfd = NULL ;
	return 0;

}
static int vfd_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef CONFIG_VFD_SM1628
	MDrv_TM1623_Write_Adr_Data(13, 0x01);
	MDrv_TM1623_StandBy();
#endif
#ifdef CONFIG_VFD_FD650
	MDrv_FrontPnl_Suspend();
#endif
	powerstate = 1;
	return 0;
}
static int vfd_resume(struct platform_device *pdev)
{
#ifdef CONFIG_VFD_SM1628
	MDrv_TM1623_Write_Adr_Data(13, 0x00);
#endif
	powerstate = 0;
	return 0;
}

static const struct of_device_id vfd_dt_match[]={
	{
		.compatible     = "amlogic,aml_vfd",
	},
	{},
};
static struct platform_driver vfd_driver = {
    .probe      = vfd_probe,
    .remove     = vfd_remove,
    .suspend    = vfd_suspend,
    .resume     = vfd_resume,
    .driver     = {
	.name   = "m1-vfd.0",
	.of_match_table = vfd_dt_match,
	},
};

static int __init vfd_init(void)
{
    printk(KERN_INFO "VFD Driver init\n");

    return platform_driver_register(&vfd_driver);
}

static void __exit vfd_exit(void)
{
    printk(KERN_INFO "VFD driver exit \n");
    platform_driver_unregister(&vfd_driver);
}

module_init(vfd_init);
module_exit(vfd_exit);

MODULE_AUTHOR("tiejun_peng && leon_e");
MODULE_DESCRIPTION("Amlogic VFD Driver");
MODULE_LICENSE("GPL");
