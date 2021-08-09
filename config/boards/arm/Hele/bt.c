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

static const struct device *btled;

void btledinit(void) {
	bool connected = zmk_ble_active_profile_is_connected();
	bool bonded = !zmk_ble_active_profile_is_open();
	enum zmk_endpoint selected_endpoint = zmk_endpoints_selected();

	btled = device_get_binding(LED1);
	if (btled == NULL) {
		return;
	}
	gpio_pin_configure(btled, PIN, GPIO_OUTPUT_ACTIVE);
	switch (selected_endpoint) {
	case ZMK_ENDPOINT_USB:
		gpio_pin_set(btled, PIN, false);
		break;
	case ZMK_ENDPOINT_BLE:
		if (bonded == true){
			if (connected == true){
				gpio_pin_set(btled, PIN, (int)1);
			}
			else {
				gpio_pin_set(btled, PIN, (int)0);
			}
		}
		else {
			gpio_pin_set(btled, PIN, (int)0);	
		}	
		break;
	}
}	

SYS_INIT(btledinit, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
ZMK_LISTENER(btled, btledinit);
ZMK_SUBSCRIPTION(btled, zmk_usb_conn_state_changed);
ZMK_SUBSCRIPTION(btled, zmk_ble_active_profile_changed);
ZMK_SUBSCRIPTION(btled, zmk_endpoint_selection_changed);