/*
 * 
 *
 * Copyright (C) 2017 Fdhisi, Inc.
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
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/errno.h>
#include <linux/string.h>

#include <linux/ioctl.h>
#include <linux/device.h>

#include <linux/errno.h>
#include <linux/mutex.h>


#include <linux/fs.h>

#include <linux/fcntl.h>
#include <linux/poll.h>

#include <linux/sched.h>
#include "aml_hdmi_gpio.h"

static HDMI_GPIO_DEV *pdata = NULL;



static ssize_t hdmi_audio_status_show(struct class *dev,
                 struct class_attribute *attr, char *buf){

	if((gpio_get_value(pdata->data_pin_1))&(gpio_get_value(pdata->data_pin_2)))
		return sprintf(buf, "%s\n", "hdmi_audio_on");
	else			 
	   return sprintf(buf, "%s\n", "hdmi_audio_off");
}

static ssize_t hdmi_audio_status_store(struct class *dev,
                 struct class_attribute *attr,
                 const char *buf, size_t count){
    switch (buf[0]) {
	case '2':
		gpio_direction_output(pdata->data_pin_1, 1);
		gpio_direction_output(pdata->data_pin_2, 0);
		break;
	case '1': 
		gpio_direction_output(pdata->data_pin_1, 0);
		gpio_direction_output(pdata->data_pin_2, 1);
		
		break;
	case '0':	
		gpio_direction_output(pdata->data_pin_1, 0);
		gpio_direction_output(pdata->data_pin_2, 0);
		break;
	case '3':
	default:
		 gpio_direction_output(pdata->data_pin_1, 1);
		 gpio_direction_output(pdata->data_pin_2, 1);
		break;
	}

	return count;
}


static struct class_attribute hdmi_gpio_class_attrs[] = {
	__ATTR(audio_status, S_IRUGO|S_IWUSR|S_IWGRP, hdmi_audio_status_show, hdmi_audio_status_store),
	__ATTR_NULL
};






static int hdmi_gpio_driver_remove(struct platform_device *pdev)
{
	class_unregister(&pdata->sysfs);
#ifdef CONFIG_OF
	gpio_free(pdata->data_pin_1);
   	gpio_free(pdata->data_pin_2);	
	kfree(pdata);
#endif
    return 0;
}

static int hdmi_gpio_driver_suspend(struct platform_device *dev, pm_message_t state)
{
    gpio_free(pdata->data_pin_1);
   	gpio_free(pdata->data_pin_2);
	return 0;
}

static int hdmi_gpio_driver_resume(struct platform_device *dev)
{
	int ret = -1;
    pr_dbg("hdmi_gpio_driver_resume");
	ret = gpio_request(pdata->data_pin_1, DEV_NAME);
	if(ret){
		pr_error("---%s----can not request pin %d\n",__func__,pdata->data_pin_1);
	}	
	ret = gpio_request(pdata->data_pin_2, DEV_NAME);
	if(ret){
		pr_error("---%s----can not request pin %d\n",__func__,pdata->data_pin_2);
	}	
    
    return 0;
}



static int hdmi_gpio_driver_probe(struct platform_device *pdev)
{
	int state=-EINVAL;

	char buf[32];
	int ret;
	const char *str;

	struct gpio_desc *desc;

	pr_dbg("==%s==\n", __func__);

    if (!pdev->dev.of_node) {
				pr_error("hdmi_gpio_driver: pdev->dev.of_node == NULL!\n");
				state = -EINVAL;
				goto get_hdmi_gpio_node_fail;
	}

	pdata = kzalloc(sizeof(*pdata), GFP_KERNEL);
    if (!pdata) {
        pr_error("platform data is required!\n");
        state = -EINVAL;
        goto get_hdmi_gpio_node_fail;
    }
	snprintf(buf, sizeof(buf), "data_pin_1");
	ret = of_property_read_string(pdev->dev.of_node, buf, &str);
	if(!ret){
		desc = of_get_named_gpiod_flags(pdev->dev.of_node, buf, 0, NULL);		
		pdata->data_pin_1 = desc_to_gpio(desc);
		pr_dbg("%s: %s\n", buf, str);
	}else{
		pr_error("cannot find resource \"%s\"\n", buf);
	}
	snprintf(buf, sizeof(buf), "data_pin_2");
	ret = of_property_read_string(pdev->dev.of_node, buf, &str);
	if(!ret){
		desc = of_get_named_gpiod_flags(pdev->dev.of_node, buf, 0, NULL);		
		pdata->data_pin_2 = desc_to_gpio(desc);
		pr_dbg("%s: %s\n", buf, str);
	}else{
		pr_error("cannot find resource \"%s\"\n", buf);
	}

    ret = gpio_request(pdata->data_pin_1, DEV_NAME);
	if(ret){
		pr_error("---%s----can not request pin %d\n",__func__,pdata->data_pin_1);
		goto get_hdmi_gpio_mem_fail ;
	}	
	ret = gpio_request(pdata->data_pin_2, DEV_NAME);
	if(ret){
		pr_error("---%s----can not request pin %d\n",__func__,pdata->data_pin_2);
		goto get_hdmi_gpio_mem_fail ;
	}	
	pdata->sysfs.name = "hdmi_gpio";
	pdata->sysfs.class_attrs = hdmi_gpio_class_attrs;
	ret= class_register(&pdata->sysfs);
	if(ret < 0)
		goto error_sysfs; 
		
	platform_set_drvdata(pdev, pdata);
	
	
	
	
    return 0;
	
    get_hdmi_gpio_mem_fail:
			kfree(pdata);
    get_hdmi_gpio_node_fail:
    error_sysfs:	
    return state;
}


#ifdef CONFIG_OF
static const struct of_device_id hdmi_gpio_dt_match[]={
	 {	.compatible = "amlogic, hdmi_gpio",
	},
	{},
};
#else
#define hdmi_gpio_dt_match NULL
#endif




static struct platform_driver hdmi_gpio_driver = {
    .probe      = hdmi_gpio_driver_probe,
    .remove     = hdmi_gpio_driver_remove,
    .suspend    = hdmi_gpio_driver_suspend,
    .resume     = hdmi_gpio_driver_resume,
    .driver     = {
        .name   = DEV_NAME,
		.owner	= THIS_MODULE,
        .of_match_table = hdmi_gpio_dt_match,
    },
};

static int __init hdmi_gpio_driver_init(void)
{
    pr_dbg( "hdmi gpio Driver init.\n");
    return platform_driver_register(&hdmi_gpio_driver);
}

static void __exit hdmi_gpio_driver_exit(void)
{
    pr_dbg("hdmi gpio Driver exit.\n");
    platform_driver_unregister(&hdmi_gpio_driver);
}

module_init(hdmi_gpio_driver_init);
module_exit(hdmi_gpio_driver_exit);

MODULE_AUTHOR("fdhisi");
MODULE_DESCRIPTION("hdmi_gpio Driver");
MODULE_LICENSE("GPL");










