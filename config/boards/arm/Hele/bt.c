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
#include <syscalls/led.h>
#include <zmk/ble.h>


/* The devicetree node identifier for the "led1" alias. */
#define LED1_NODE DT_ALIAS(led1)


#if DT_NODE_HAS_STATUS(LED1_NODE, okay)
#define LED1 DT_GPIO_LABEL(LED1_NODE, gpios)
#define PIN DT_GPIO_PIN(LED1_NODE, gpios)
#define FLAGS DT_GPIO_FLAGS(LED1_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led devicetree alias is not defined"
#define LED1	""
#define PIN	0
#define FLAGS	0
#endif



static int pwr_led_init(const struct device *dev) {
	bool zmk_ble_active_profile_is_connected ();

	dev = device_get_binding(LED1);
	if (dev == NULL) {
		return -EIO;
	}

	__ASSERT_NO_MSG(device_is_ready(dev));

	while (true) {
		gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE);
		gpio_pin_set(dev, PIN, (int)zmk_ble_active_profile_is_connected);
		if (zmk_ble_active_profile_is_connected = false) {
			/* Release resource to release device clock */
			gpio_pin_configure(dev, PIN, GPIO_DISCONNECTED);
		}
		if (zmk_ble_active_profile_is_connected = true) {
			/* Release resource to release device clock */
			gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE);
		}
		
SYS_INIT(pwr_led_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
