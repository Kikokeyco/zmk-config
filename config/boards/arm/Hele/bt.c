/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <bluetooth/services/bas.h>

#include <logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <zmk/ble.h>
#include <zmk/event_manager.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/events/endpoint_selection_changed.h>
#include <zmk/endpoints.h>



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

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);






static int btled(const struct device *dev) {
	bool connected = zmk_ble_active_profile_is_connected();
	bool bonded = !zmk_ble_active_profile_is_open();
	int ret;

	dev = device_get_binding(LED1);
	if (dev == NULL) {
		return -EIO;
	}

	ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return -EIO;
	}	
	if (bonded) {
		if (connected) {
			gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE);
		}
		else {gpio_pin_configure(dev, PIN, GPIO_OUTPUT_INACTIVE);}
	}
	
}		

SYS_INIT(btled, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
ZMK_LISTENER(dev, output_status_listener);
ZMK_SUBSCRIPTION(dev, zmk_usb_conn_state_changed);
ZMK_SUBSCRIPTION(dev, zmk_ble_active_profile_changed);
ZMK_SUBSCRIPTION(dev, zmk_activity_state_changed);