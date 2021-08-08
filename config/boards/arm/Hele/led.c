/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/led.h>


/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)

#if DT_NODE_HAS_STATUS(LED_NODE, okay)
#define LED0 DT_GPIO_LABEL(LED_NODE, gpios)
#define PIN DT_GPIO_PIN(LED_NODE, gpios)
#define FLAGS DT_GPIO_FLAGS(LED_NODE, gpios)

#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led devicetree alias is not defined"
#define LED0	""
#define PIN	0
#define FLAGS	0
#endif


void led(void)
{
	const struct device *dev;
	bool led_is_on = true;
	int ret;
	

	dev = device_get_binding(LED0);
	if (dev == NULL) {
		return;
	}	

	ret = gpio_pin_configure (dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
	if (ret < 0) {
			return;
	}

	while (1) {
		gpio_pin_set(dev, PIN, (int)led_is_on);
		led_is_on = !led_is_on;
		if (led_is_on == false) {
			/* Release resource to release device clock */
			gpio_pin_configure(dev, PIN, GPIO_DISCONNECTED);
		}
		k_msleep(SLEEP_TIME_MS);
		if (led_is_on == true) {
			/* Release resource to release device clock */
			gpio_pin_configure(dev, PIN, GPIO_DISCONNECTED);
		}
		led_is_on = !led_is_on;
	}
}
SYS_INIT(led, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
