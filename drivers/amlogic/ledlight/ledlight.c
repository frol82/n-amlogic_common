#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/leds.h>
#include <linux/gpio.h>
#include <linux/proc_fs.h>
#include <linux/printk.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
static struct early_suspend ledlight_early_suspend;
#endif

#define DRIVER_NAME "ledlight"

#define DRV_NAME	DRIVER_NAME
#define DRV_VERSION	"v3.0.0"

enum ledlight_type {
	LED_UNKNOWN = 0,
	LED_NET = 1,
	LED_POWER = 2,
};

enum led_type {
	LED_RG = 0, /* double color led, Red and Green(R&G) */
	LED_R = 1,  /* single color led, Red(R) */
	LED_G = 2,  /* single color led, Green(G) */
};

enum led_state {
	STATE_UNKNOWN  = 0,
	STATE_OFF = 1,       /* open red led or close red&green led */
	STATE_ON = 2,        /* open green or red&green led */
	STATE_ALWAYS_ON = 3, /* keep green or red&green led always on */
	STATE_ALL_OFF = 4,   /* close all led */
};

enum gpio_level {
	LOW = 0,
	HIGHT = 1,
};

struct led_gpio {
	unsigned int pin;
	enum gpio_level gpio_active;
	enum led_type type;
	struct led_gpio *next;
};

struct ledlight {
	struct proc_dir_entry *leddir;
	struct proc_dir_entry *netdir;
	struct proc_dir_entry *powerdir;
	struct led_gpio *net_gpio;
	enum led_state net_state;
	struct led_gpio *power_gpio;
	enum led_state power_state;
};

static struct ledlight led_data;

static int ledlight_count_gpio(struct led_gpio *gpio_list)
{
	int count = 0;
	struct led_gpio *pre_gpio = NULL;

	pre_gpio = gpio_list;
	while (pre_gpio) {
		count++;
		pre_gpio = pre_gpio->next;
	}

	pr_info("%s, gpio count: %d\n", __func__, count);
	return count;
}

static struct led_gpio *ledlight_find_gpio_by_type(
		struct led_gpio *gpio_list, enum led_type type)
{
	struct led_gpio *pre_gpio = NULL;
	pre_gpio = gpio_list;
	while (pre_gpio) {
		if (pre_gpio->type == type)
			return pre_gpio;
		pre_gpio = pre_gpio->next;
	}
	return NULL;
}

static int ledlight_add_to_list(
		struct led_gpio **gpio_list, struct led_gpio *gpio)
{
	struct led_gpio *pre_gpio = NULL;

	if (*gpio_list == NULL) {
		*gpio_list = gpio;
	} else {
		pre_gpio = *gpio_list;
		while (pre_gpio->next != NULL)
			pre_gpio = pre_gpio->next;
		pre_gpio->next = gpio;
	}
	return 0;
}

static int ledlight_free_list(struct led_gpio **gpio_list)
{
	struct led_gpio *pre_gpio = NULL;
	pre_gpio = *gpio_list;
	while (pre_gpio) {
		*gpio_list = (*gpio_list)->next;
		kfree(pre_gpio);
		pre_gpio = *gpio_list;
	}
	return 0;
}

static int set_led(int pin, enum gpio_level level)
{
	if (pin < 0) {
		pr_err("%s set fail, pin=%d\n", __func__, pin);
		return 0;
	}

	pr_info("%s set pin: %d, level: %d\n", __func__, pin, level);
	gpio_direction_output(pin, level);
	return 0;
}

static int set_leds(enum led_state state, struct led_gpio *gpio_list)
{
	struct led_gpio *gpio_rg = NULL;
	struct led_gpio *gpio_r = NULL;
	struct led_gpio *gpio_g = NULL;

	if (gpio_list == NULL) {
		pr_err("%s gpio list is NULL!\n", __func__);
		return -1;
	}

	gpio_rg = ledlight_find_gpio_by_type(gpio_list, LED_RG);
	gpio_r = ledlight_find_gpio_by_type(gpio_list, LED_R);
	gpio_g = ledlight_find_gpio_by_type(gpio_list, LED_G);

	if (state == STATE_OFF) {
		if (gpio_rg != NULL) {
			set_led(gpio_rg->pin, !gpio_rg->gpio_active);
		} else {
			/* open led led */
			if (gpio_r != NULL)
				set_led(gpio_r->pin, gpio_r->gpio_active);
			if (gpio_g != NULL)
				set_led(gpio_g->pin, !gpio_g->gpio_active);
		}
	} else if ((state == STATE_ON) || (state == STATE_ALWAYS_ON)) {
		if (gpio_rg != NULL) {
			set_led(gpio_rg->pin, gpio_rg->gpio_active);
		} else {
			/* open green led */
			if (gpio_r != NULL)
				set_led(gpio_r->pin, !gpio_r->gpio_active);
			if (gpio_g != NULL)
				set_led(gpio_g->pin, gpio_g->gpio_active);
		}
	} else if (state == STATE_ALL_OFF) {
		if (gpio_rg != NULL) {
			set_led(gpio_rg->pin, !gpio_rg->gpio_active);
		} else {
			/* close led and green led */
			if (gpio_r != NULL)
				set_led(gpio_r->pin, !gpio_r->gpio_active);
			if (gpio_g != NULL)
				set_led(gpio_g->pin, !gpio_g->gpio_active);
		}
	} else {
			pr_err("%s error, state is %d!\n", __func__, state);
	}

	return 0;
}

static ssize_t netled_control(struct file *file, const char __user *buffer,
		size_t size, loff_t *loff)
{
	char buf[128] = {0};
	int ret = 0;

	if (size > 0) {
		memset(buf, 0, sizeof(buf));
		ret = copy_from_user(buf, buffer, size);
		if (ret != 0)
			return -EINVAL;

		if (strncmp(buf, "on", 2) == 0)
			led_data.net_state = STATE_ON;
		else if (strncmp(buf, "always_on", 9) == 0)
			led_data.net_state = STATE_ALWAYS_ON;
		else if (strncmp(buf, "off", 3) == 0)
			led_data.net_state = STATE_OFF;
		else if (strncmp(buf, "all_off", 6) == 0)
			led_data.net_state = STATE_ALL_OFF;

		set_leds(led_data.net_state, led_data.net_gpio);
	}
	return size;
}

static ssize_t powerled_control(struct file *file, const char __user *buffer,
		size_t size, loff_t *loff)
{
	char buf[128] = {0};
	int ret = 0;

	if (size > 0) {
		memset(buf, 0, sizeof(buf));
		ret = copy_from_user(buf, buffer, size);
		if (ret != 0)
			return -EINVAL;

		if (strncmp(buf, "on", 2) == 0)
			led_data.power_state = STATE_ON;
		else if (strncmp(buf, "off", 3) == 0)
			led_data.power_state = STATE_OFF;
		else if (strncmp(buf, "all_off", 6) == 0)
			led_data.power_state = STATE_ALL_OFF;

		set_leds(led_data.power_state, led_data.power_gpio);
	}
	return size;
}

static const struct file_operations netled_proc_fops = {
	.write = netled_control,
};

static const struct file_operations powerled_proc_fops = {
	.write = powerled_control,
};

static void init_led_data(void)
{
	led_data.leddir = NULL;
	led_data.netdir = NULL;
	led_data.powerdir = NULL;
	led_data.net_gpio = NULL;
	led_data.power_gpio = NULL;
	led_data.net_state = STATE_OFF;
	led_data.power_state = STATE_ON;
}

static void ledlight_suspend(struct early_suspend *h)
{
	pr_info("%s!\n", __func__);
	if (led_data.netdir != NULL)
		set_leds(STATE_ALL_OFF, led_data.net_gpio);

	if (led_data.powerdir != NULL)
		set_leds(STATE_OFF, led_data.power_gpio);
}

static void ledlight_resume(struct early_suspend *h)
{
	pr_info("%s!\n", __func__);
	if (led_data.net_state == STATE_ALWAYS_ON) {
		if (led_data.netdir != NULL)
			set_leds(STATE_ALWAYS_ON, led_data.net_gpio);
	}
	if (led_data.powerdir != NULL)
		set_leds(STATE_ON, led_data.power_gpio);
}

static int ledlight_gpio_parse_dt(struct device_node *node, int id)
{
	int ret =  -1;
	unsigned int val;
	const char *led_name = NULL;
	const char *led_type = NULL;
	struct gpio_desc *desc = NULL;
	struct led_gpio *gpio = NULL;
	enum ledlight_type type = LED_UNKNOWN;

	gpio = kzalloc(sizeof(struct led_gpio), GFP_KERNEL);
	if (!gpio) {
		pr_err("%s:%d, no memory for led_gpio\n", __func__, __LINE__);
		ret = -ENOMEM;
		return ret;
	}

	/* parser led name, for example <  led_name = "power_led"  > */
	ret = of_property_read_string(node, "led_name", &led_name);
	if (ret < 0) {
		pr_err("%s:%d, get led_name fail\n", __func__, __LINE__);
		ret = -EINVAL;
		goto exit;
	}
	if (led_name) {
		pr_info("led_name: %s\n", led_name);
		if (strncmp(led_name, "net_led", 7) == 0)
			type = LED_NET;
		else if (strncmp(led_name, "power_led", 9) == 0)
			type = LED_POWER;
		else {
			pr_err("%s:%d, not support this ledlight type(%s)\n",
					__func__, __LINE__, led_name);
			goto exit;
		}
	}

	/* parser led gpio, for example
		<  led_gpio = <&gpio_ao  GPIOAO_2  GPIO_ACTIVE_HIGH>  > */
	desc = of_get_named_gpiod_flags(node, "led_gpio", 0, NULL);
	if (IS_ERR(desc)) {
		pr_err("%s:%d, get led_gpio fail\n", __func__, __LINE__);
		ret = -EINVAL;
		goto exit;
	}
	gpio->pin = desc_to_gpio(desc);
	pr_info("gpio pin: %d\n", gpio->pin);
	gpio_request(gpio->pin, DRV_NAME);

	/* parser led_active, for example <  led_active = <1>  > */
	ret = of_property_read_u32(node, "led_active", &val);
	if (ret) {
		pr_info("faild to get led_active\n");
		/* set default active_low */
		val = 1;
	}
	pr_info("gpio active: %d\n", val);
	if (val == 1)
		gpio->gpio_active = HIGHT;
	else
		gpio->gpio_active = LOW;

	/* parser led_type, for example <  led_type = "R&G"  > */
	ret = of_property_read_string(node, "led_type", &led_type);
	if (ret < 0) {
		pr_err("%s:%d,get led_type fail\n", __func__, __LINE__);
		ret = -EINVAL;
		goto exit;
	}
	if (led_type) {
		pr_info("led_type: %s\n", led_type);
		if (strncmp(led_type, "R&G", 3) == 0)
			gpio->type = LED_RG;
		else if (strncmp(led_type, "R", 1) == 0)
			gpio->type = LED_R;
		else if (strncmp(led_type, "G", 1) == 0)
			gpio->type = LED_G;
		else {
			pr_err("%s:%d, not support this led type(%s)\n",
					__func__, __LINE__, led_type);
			goto exit;
		}
	}

	if (type == LED_NET)
		ledlight_add_to_list(&led_data.net_gpio, gpio);
	else if (type == LED_POWER)
		ledlight_add_to_list(&led_data.power_gpio, gpio);
	else {
		pr_err("%s:%d, not support this ledlight type(%d)\n",
				__func__, __LINE__, type);
		goto exit;
	}
	return 0;
exit:
	kfree(gpio);
	return ret;
}

static int ledlight_item_create(struct platform_device *pdev)
{
	int ret =  -1;
	int index;
	struct device_node *child;
	struct device_node *np = pdev->dev.of_node;

	of_node_get(np);
	index = 0;
	for_each_child_of_node(np, child) {
		ret = ledlight_gpio_parse_dt(child, index);
		if (!ret)
			index++;
	}
	pr_info("ledlight gpio-num is %d\n", index);
	return 0;
}

static int ledlight_probe(struct platform_device *pdev)
{
	pr_info("%s!\n", __func__);
	init_led_data();

	led_data.leddir = proc_mkdir(DRV_NAME, NULL);
	ledlight_item_create(pdev);

	if (ledlight_count_gpio(led_data.net_gpio) > 0) {
		set_leds(led_data.net_state, led_data.net_gpio);
		led_data.netdir = proc_mkdir("netled", led_data.leddir);
		proc_create_data("state", 0666, led_data.netdir,
				&netled_proc_fops, NULL);
	}
	if (ledlight_count_gpio(led_data.power_gpio) > 0) {
		set_leds(led_data.power_state, led_data.power_gpio);
		led_data.powerdir = proc_mkdir("powerled", led_data.leddir);
		proc_create_data("state", 0666, led_data.powerdir,
				&powerled_proc_fops, NULL);
	}

#ifdef CONFIG_HAS_EARLYSUSPEND
	ledlight_early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN;
	ledlight_early_suspend.suspend = ledlight_suspend;
	ledlight_early_suspend.resume = ledlight_resume;
	register_early_suspend(&ledlight_early_suspend);
#endif
	return 0;
}

static void ledlight_shutdown(struct platform_device *pdev)
{

	pr_info("%s!\n", __func__);
	if (led_data.netdir != NULL)
		set_leds(STATE_ALL_OFF, led_data.net_gpio);

	if (led_data.powerdir != NULL)
		set_leds(STATE_OFF, led_data.power_gpio);

}

static int ledlight_remove(struct platform_device *pdev)
{
	pr_info("%s!\n", __func__);
	ledlight_free_list(&led_data.net_gpio);
	ledlight_free_list(&led_data.power_gpio);
	if (led_data.netdir != NULL) {
		remove_proc_entry("state", led_data.netdir);
		if (led_data.leddir != NULL)
			remove_proc_entry("netled", led_data.leddir);
	}
	if (led_data.powerdir != NULL) {
		remove_proc_entry("state", led_data.powerdir);
		if (led_data.leddir != NULL)
			remove_proc_entry("powerled", led_data.leddir);
	}
	remove_proc_entry(DRV_NAME, NULL);
#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&ledlight_early_suspend);
#endif
	return 0;
}

static const struct of_device_id ledlight_dt_match[] = {
	{	.compatible = "amlogic, ledlight", },
	{},
};

static struct platform_driver ledlight_driver = {
	.probe = ledlight_probe,
	.remove = ledlight_remove,
        .shutdown= ledlight_shutdown,
	.driver = {
		.name = DRV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = ledlight_dt_match,
	}
};

static int __init aml_ledlight_init(void)
{
	pr_info("%s!\n", __func__);
	if (platform_driver_register(&ledlight_driver)) {
		pr_err("failed to register ledlight driver\n");
		return -ENODEV;
	}
	return 0;
}

static void __exit aml_ledlight_exit(void)
{
	pr_info("%s!\n", __func__);
	platform_driver_unregister(&ledlight_driver);
}

module_init(aml_ledlight_init);
module_exit(aml_ledlight_exit);

MODULE_DESCRIPTION("Amlogic ledlight driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Amlogic, Inc.");

