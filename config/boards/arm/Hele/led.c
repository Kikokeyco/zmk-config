/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>


/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)

#if DT_NODE_HAS_STATUS(LED_NODE, okay)
#define LED DT_GPIO_LABEL(LED_NODE, gpios)
#define PIN DT_GPIO_PIN(LED_NODE, gpios)
#define FLAGS DT_GPIO_FLAGS(LED_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led devicetree alias is not defined"
#define LED0	""
#define PIN	0
#define FLAGS	0
#endif


static int pwr_led_init(const struct device *dev)
{
	int ret;
	bool led_is_on = true;

	dev = device_get_binding(LED);
	if (dev == NULL) {
		return -EIO;
	}	

	ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
	if (ret < 0) {
		return -EIO;
	}

	while (1) {
		gpio_pin_set(dev, PIN, (int)led_is_on);
		led_is_on = !led_is_on;
		k_msleep(SLEEP_TIME_MS);
		
	}
}
SYS_INIT(pwr_led_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
