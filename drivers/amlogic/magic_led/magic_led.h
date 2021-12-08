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

#ifndef __MAGIC_LED_H
#define __MAGIC_LED_H

#include <linux/leds.h>
#include <linux/workqueue.h>
#include <linux/mutex.h>

#define MAGIC_LED_ID0 0 //blue
#define MAGIC_LED_ID1 1 //bluegreen 
#define MAGIC_LED_ID2 2 //green and black
#define MAGIC_LED_ID3 3 //flash loop
#define MAGIC_LED_ID4 4 //always green
#define MAGIC_LED_ID5 5 //black


struct magic_led_gpio {
	const char *name;
	unsigned int pin1;
	unsigned int pin2;
};

#endif
