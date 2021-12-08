/*
 * drivers/amlogic/led/aml_led_sys.h
 *
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
*/

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/leds.h>
#include <linux/gpio.h>
#include <linux/printk.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "magic_led.h"

#include <linux/sched.h> 
#include <linux/kthread.h>
#include <linux/err.h>

//#include <sys/sysinfo.h>

#include <linux/timer.h>
#include <linux/timex.h>
#include <linux/rtc.h>
#include <linux/delay.h>

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
static struct early_suspend magicled_early_suspend;
#endif

#define NO_THREAD

#define MAGICLED_DEV_NAME		"magicled"
#define MAGICLED_NAME		"magic-led"

#define DEBUG

#ifndef DEBUG
#define INFO(format, arg...)
#define ERR(format,  arg...)
#else
#define INFO(format, arg...) pr_info("%s: " format, __func__ , ## arg)
#define ERR(format,  arg...) pr_err("%s: "  format, __func__ , ## arg)
#endif

struct magic_led_dev {
	struct magic_led_gpio magic_led_io;
	struct led_classdev cdev;
	struct work_struct work_update;
	struct mutex lock;
	struct timer_list timer;
#ifdef CONFIG_HAS_EARLYSUSPEND
	struct early_suspend early_suspend;
#endif

};


int magic_led_id = 0;
EXPORT_SYMBOL(magic_led_id);

int issuspend =0;
EXPORT_SYMBOL(issuspend);

struct magic_led_dev *thread_mled_dev;

static int magic_led_dt_parse(struct platform_device *pdev)
{
	struct device_node *node;
	struct magic_led_dev *mled_dev;
	struct gpio_desc *desc;
	unsigned int default_led_mode = 0;
	int ret;
	
	mled_dev = platform_get_drvdata(pdev);

	mled_dev->magic_led_io.pin1 = 0;
	mled_dev->magic_led_io.pin2 = 0;
	
	node = pdev->dev.of_node;
	if (!node) {
		ERR("failed to find node for %s\n", MAGICLED_DEV_NAME);
		return -ENODEV;
	}

	desc = of_get_named_gpiod_flags(node, "led1_gpio", 0, NULL);
	mled_dev->magic_led_io.pin1 = desc_to_gpio(desc);
	if (mled_dev->magic_led_io.pin1 > 0)
		gpio_request(mled_dev->magic_led_io.pin1, MAGICLED_DEV_NAME);
	else{

		ERR("faild to get led1_gpio\n");
		return -ENODEV;
	}
		
	
	desc = of_get_named_gpiod_flags(node, "led2_gpio", 0, NULL);
	mled_dev->magic_led_io.pin2 = desc_to_gpio(desc);
	if (mled_dev->magic_led_io.pin2 > 0)
		gpio_request(mled_dev->magic_led_io.pin2, MAGICLED_DEV_NAME);
	else{

		ERR("faild to get led2_gpio\n");
		return -ENODEV;
	}

	ret = of_property_read_u32(node, "magic_mode", &default_led_mode);
	if (ret) {
		ERR("faild to get magic_mode,force to 0 \n");
		default_led_mode = 0;
	}
	
	INFO("magic_mode = %u\n", default_led_mode);

	thread_mled_dev = mled_dev;
	
	switch(default_led_mode){
		case MAGIC_LED_ID1:
			gpio_direction_output(mled_dev->magic_led_io.pin1, 1);
			gpio_direction_output(mled_dev->magic_led_io.pin2, 0);
			break;
		case MAGIC_LED_ID2:
			gpio_direction_output(mled_dev->magic_led_io.pin1, 0);
			gpio_direction_output(mled_dev->magic_led_io.pin2, 1);
			break;
		case MAGIC_LED_ID3:
			gpio_direction_output(mled_dev->magic_led_io.pin1, 1);
			gpio_direction_output(mled_dev->magic_led_io.pin2, 0);
			break;
		case MAGIC_LED_ID4:
			gpio_direction_output(mled_dev->magic_led_io.pin1, 0);
			gpio_direction_output(mled_dev->magic_led_io.pin2, 0);
			break;
		case MAGIC_LED_ID5:
		default:
			gpio_direction_output(mled_dev->magic_led_io.pin1, 0);
			gpio_direction_output(mled_dev->magic_led_io.pin2, 1);
			break;
	
	}
	return 0;
}



static const struct of_device_id magic_led_dt_match[] = {
	{
		.compatible = "amlogic, magicled",
	},
	{},
};

static const char MagicLedIDFileName[] = "/data/id.txt";

void magic_led_write_id_to_file(int led_id)
{
	char data[1];
	unsigned int orgfs;
	struct file *fp;

	sprintf(data, "%d", led_id);

	orgfs = get_fs();
	/* Set segment descriptor associated to kernel space */
	set_fs(KERNEL_DS);

	fp = filp_open(MagicLedIDFileName, O_RDWR | O_CREAT, 0777);
	if (IS_ERR(fp)) {
		printk("filp_open %s error!!.\n", MagicLedIDFileName);
	} else {
		printk("filp_open %s SUCCESS!!.\n",MagicLedIDFileName);
		fp->f_op->write(fp, data, 1, &fp->f_pos);
		filp_close(fp, NULL);
	}

	set_fs(orgfs);
}

int get_magic_id(void)
{
		unsigned int orgfs;
		char data[1];
		struct file *fp;
		int ret,magic_id=0;
		orgfs = get_fs();
		/* Set segment descriptor associated to kernel space */
		set_fs(KERNEL_DS);
	
		fp = filp_open(MagicLedIDFileName, O_RDWR , 0);
		printk(KERN_INFO "%s,try file open !\n", __func__);
		if (IS_ERR(fp)) {
			printk(KERN_INFO "%s:Sorry,file open ERROR !\n", __func__);
			set_fs(orgfs);
			return 0;
		} else {
			printk(" get_magic_id ->filp_open %s SUCCESS!!.\n", MagicLedIDFileName);
			fp->f_op->read(fp, data, 1, &fp->f_pos);
			printk("get_magic_id result %s\n", data);

			ret = sscanf(data, "%d", &magic_id);
			printk("get_magic_id ->read size=%d,magic id =%d\n",ret,magic_id);			
			filp_close(fp, NULL);
			set_fs(orgfs);
			return magic_id;
		}
		
}

#ifndef NO_THREAD
static struct task_struct *magic_led_task = NULL; 

int magic_led_thread(void *data)
{

	struct timespec uptime;
	unsigned long boottime =0;
	int pre_id=0;
	int id3_number =0;
	printk(" magic_led_thread \n");

	do{

		get_monotonic_boottime(&uptime);
		boottime = (unsigned long)uptime.tv_sec;
	}
	while(boottime <= 16);


	magic_led_id = get_magic_id();	
	if (magic_led_id == 0)
		magic_led_id = 1;
	
	while(1)  
	{
			if(kthread_should_stop()) 
			{
				printk(" magic_led_thread stop \n");
				break;
			}
			
				//printk("id =%d,pre id=%d\n",magic_led_id,pre_id);
				if ((magic_led_id != pre_id)||(magic_led_id == MAGIC_LED_ID3) ){

						if (magic_led_id != pre_id){
							pre_id = magic_led_id;
							magic_led_write_id_to_file(magic_led_id);
						}
						
						switch(magic_led_id){
							case MAGIC_LED_ID1://only bluegreen
								//printk("id1 action\n");
								gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 1);
								gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 1);
								break;
							case MAGIC_LED_ID2://green 0.5s and black
							//printk("id2 action\n");
								gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 1);
								gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 0);
								msleep(500);
								gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 0);
								gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 0);
								break;
							case MAGIC_LED_ID3://flash loop : blue 3s -> bluegreen 3s -> green 3s -> blue 3s...
							//printk("id3 action\n");
								if (id3_number ==0){
									gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 0);
									gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 1);
								}else if (id3_number ==1){
									gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 1);
									gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 1);
								}else if (id3_number ==2){
									gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 1);
									gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 0);
								}
								msleep(2000);
								break;
							case MAGIC_LED_ID4:// only green 
							//printk("id4 action\n");
								gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 1);
								gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 0);
								break;
							case MAGIC_LED_ID5://only black
							//printk("id5 action\n");
								gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 0);
								gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 0);
								break;
							default://only blue
								gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 0);
								gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 1);
								break;
					
						}
						
						if (magic_led_id == MAGIC_LED_ID3){

							id3_number++;
								if (id3_number > 2){
									id3_number  = 0;
								}
						}else{
							id3_number = 0;
						}
						
						
			}
			msleep(1000);
	}
	return 0 ;
}

void magic_led_thread_init(void)
 {
	int err;

	printk("\n ####  ir_led_init  ##### \n");

	//return;
	
	magic_led_task = kthread_create(magic_led_thread, NULL, "magic_led_task"); 
	if(IS_ERR(magic_led_task)){  
		printk(" \n  #### Unable to start magic led thread./n");  
		err = PTR_ERR(magic_led_task);  
		magic_led_task = NULL;  
		return ;//err;  
	}
	wake_up_process(magic_led_task);
	return ;//0;
 }

#endif

#ifdef NO_THREAD
int magic_led_count=0,id3_number=0,pre_id=0;
static void magic_led_work(struct magic_led_dev *kp)
{
	struct timespec uptime;
	unsigned long boottime =0;

	//printk(" magic_led_work \n");


	get_monotonic_boottime(&uptime);
	boottime = (unsigned long)uptime.tv_sec;

	if (issuspend)
		return;
	
	if (boottime < 16)//if boot time more than 20s(uboot 4s+kernel 16s),start magic led 
		return;

	if (magic_led_id == 0){// do magic led process/3 seconds

		magic_led_id = get_magic_id();	
		if (magic_led_id == 0)
			magic_led_id = 1;
	}
	//}else{
	  {
			
			
			if (magic_led_id == MAGIC_LED_ID3){

				
				if (magic_led_id != pre_id){
					pre_id = magic_led_id;
					magic_led_write_id_to_file(magic_led_id);

					gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 0);
					gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 1);
				}
				
				if (magic_led_count != 5){// do magic led process/3 seconds 

			
					magic_led_count++;
				}else{
					

					
					if (id3_number > 2){
							id3_number  = 0;
					}
					
					if (id3_number == 0){
						//printk(" magic led ID3-0 action\n");
						gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 0);
						gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 1);
						
						
					}else if (id3_number == 1){
						//printk(" magic led ID3-1 action\n");
						gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 1);
						gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 1);
						
									
					}else if (id3_number == 2){
						//printk(" magic led ID3-2 action\n");
						gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 1);
						gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 0);
						
					}
					id3_number++;
					magic_led_count = 0;
				}
			}else{
				id3_number = 0;
				magic_led_count = 0;
				if (magic_led_id != pre_id){
					
					switch(magic_led_id){
							case MAGIC_LED_ID1://only bluegreen
								//printk("id1 action\n");
								gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 1);
								gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 1);
								break;
							case MAGIC_LED_ID2://green 0.5s and black
								//printk("id2 action\n");
								gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 1);
								gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 0);
								msleep(500);
								gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 0);
								gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 0);
								break;
							case MAGIC_LED_ID4:// only green 
								//printk("id4 action\n");
								gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 1);
								gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 0);
								break;
							case MAGIC_LED_ID5://only black
								//printk("id5 action\n");
								gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 0);
								gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 0);
								break;
							default://only blue
								gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 0);
								gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 1);
								break;
					
						}
					pre_id = magic_led_id;
					magic_led_write_id_to_file(magic_led_id);
				}
			}
	}
}

static void update_work_func(struct work_struct *work)
{
	struct magic_led_dev *kp = container_of(work, struct magic_led_dev, work_update);
	magic_led_work(kp);
	//printk("magic led update work\n");
}

static void magic_led_timer_sr(unsigned long data)
{
	struct magic_led_dev *kp = (struct magic_led_dev *)data;
	schedule_work(&(kp->work_update));
	mod_timer(&kp->timer, jiffies+msecs_to_jiffies(500));
}

#endif

static void magic_led_resume(struct early_suspend *h)
{
	
	#ifdef NO_THREAD
		issuspend = 0 ;
		magic_led_id = get_magic_id();	
		if (magic_led_id == 0)
			magic_led_id = 1;
		
		pre_id = 0;
		id3_number = 0;
	#endif
	INFO("magic_led_resume\n");
	//return 0;
}

static void magic_led_suspend(struct early_suspend *h)
{
	gpio_direction_output(thread_mled_dev->magic_led_io.pin1, 0);
	gpio_direction_output(thread_mled_dev->magic_led_io.pin2, 0);

	#ifdef NO_THREAD
		issuspend = 1;
	#endif
	
	INFO("magic_led_suspend done\n");
	//return 0;
}

static void magic_led_set(struct magic_led_dev *ldev,
				int onoff)
{
	if(onoff){
               gpio_direction_output(ldev ->magic_led_io.pin2, 1);
		}
	else
		{
		gpio_direction_output(ldev ->magic_led_io.pin2, 0);
		}
	
	gpio_direction_output(ldev ->magic_led_io.pin1, 0);
}

static int magic_led_probe(struct platform_device *pdev)
{
	struct magic_led_dev *ldev;
	int ret;

	ldev = kzalloc(sizeof(struct magic_led_dev), GFP_KERNEL);
	if (!ldev) {
		ERR("kzalloc error\n");
		return -ENOMEM;
	}

	/* set driver data */
	platform_set_drvdata(pdev, ldev);

	/* parse dt param */
	ret = magic_led_dt_parse(pdev);
	if (ret)
		return ret;

	/* register led class device */
	ldev->cdev.name = MAGICLED_NAME;
	
	mutex_init(&ldev->lock);
	ret = led_classdev_register(&pdev->dev, &ldev->cdev);
	if (ret < 0) {
		kfree(ldev);
		return ret;
	}

#ifndef NO_THREAD
	magic_led_thread_init();// magic led thread 
#else
	INIT_WORK(&(ldev->work_update), update_work_func);
	setup_timer(&ldev->timer, magic_led_timer_sr, (unsigned long)ldev);
	mod_timer(&ldev->timer, jiffies+msecs_to_jiffies(500));
#endif

#ifdef CONFIG_HAS_EARLYSUSPEND
	ldev->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN;
	ldev->early_suspend.suspend = magic_led_suspend;
	ldev->early_suspend.resume = magic_led_resume;
	register_early_suspend(&ldev->early_suspend);
#endif
	


	INFO("module probed ok\n");

	return 0;
}


static int __exit magic_led_remove(struct platform_device *pdev)
{
	struct magic_led_dev *ldev = platform_get_drvdata(pdev);

	led_classdev_unregister(&ldev->cdev);
	gpio_free(ldev->magic_led_io.pin1);
	gpio_free(ldev->magic_led_io.pin2);
	platform_set_drvdata(pdev, NULL);
	#ifdef NO_THREAD
	del_timer_sync(&ldev->timer);
	cancel_work_sync(&ldev->work_update);
	#endif
	kfree(ldev);
	
#ifdef CONFIG_HAS_EARLYSUSPEND
		unregister_early_suspend(&magicled_early_suspend);
#endif
	return 0;
}

static void magic_led_shutdown(struct platform_device *pdev)
{
	struct magic_led_dev *ldev = platform_get_drvdata(pdev);
	/* set led off*/
         magic_led_set(ldev,0);
}

static struct platform_driver magic_led_driver = {
	.driver = {
		.name = MAGICLED_DEV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = magic_led_dt_match,
	},
	.probe = magic_led_probe,
	.remove = __exit_p(magic_led_remove),
	.shutdown = magic_led_shutdown,

	
};


static int __init magic_led_init(void)
{
	INFO("module init\n");
	if (platform_driver_register(&magic_led_driver)) {
		ERR("failed to register driver\n");
		return -ENODEV;
	}

	return 0;
}


static void __exit magic_led_exit(void)
{
	INFO("module exit\n");
	platform_driver_unregister(&magic_led_driver);
}

void power_magicled_control(int onoff)
{
   unsigned int on = !!onoff;
   printk("power_magicled_control  onoff=%d\n",on);		
   magic_led_set(thread_mled_dev,on);

}
EXPORT_SYMBOL(power_magicled_control);

module_init(magic_led_init);
module_exit(magic_led_exit);

MODULE_DESCRIPTION("magic led driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("echo, Inc.");

