/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3
 */

#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <npmx_driver.h>

#include "nrf_fuel_gauge.h"

#define LOG_MODULE_NAME fuel_gague
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

K_SEM_DEFINE(init_sem, 0, 1);

static const struct battery_model battery_model = {
#include "battery_model.inc"
};

static npmx_error_t fuel_gauge_init(void)
{
	struct nrf_fuel_gauge_init_parameters parameters = { .model = &battery_model };

	nrf_fuel_gauge_init(&parameters, NULL);

	return NPMX_SUCCESS;
}

static void adc_callback(npmx_instance_t *p_pm, npmx_callback_type_t type, uint8_t mask)
{
	/* Get V_BAT */
	uin16_t vbat_mv;
	if (npmx_adc_meas_get(npmx_adc_get(p_pm, 0), NPMX_ADC_MEAS_VBAT, &vbat_mv) !=
	    NPMX_SUCCESS) {
		LOG_ERR("VBAT measurement failed.")
	}

	// get T_BAT

	// get I_BAT ???

	/* Release first callback semaphore - fuel gauge can be initialized. */
	k_sem_give(&init_sem);
}

void main(void)
{
	const struct device *pmic_dev = DEVICE_DT_GET(DT_NODELABEL(npm_0));

	if (!device_is_ready(pmic_dev)) {
		LOG_ERR("PMIC device is not ready.");
		return;
	}

	LOG_INF("PMIC device OK.");

	/* Get pointer to npmx device. */
	npmx_instance_t *npmx_instance = npmx_driver_instance_get(pmic_dev);

	/* Register callback for ADC events. */
	npmx_core_register_cb(npmx_instance, adc_callback, NPMX_CALLBACK_TYPE_EVENT_ADC);

	/* Enable interrupt for ADC measurement ready. */
	npmx_core_event_interrupt_enable(npmx_instance, NPMX_EVENT_GROUP_ADC,
					 NPMX_EVENT_GROUP_ADC_BAT_READY_MASK);

	/* Set NTC type for ADC measurements. */
	npmx_adc_ntc_set(npmx_adc_get(npmx_instance, 0), NPMX_ADC_NTC_TYPE_10_K);

	/* Enable ADC auto measurements every ~1s (default). */
	npmx_adc_config_t config = { .vbat_auto = true, .vbat_burst = false };

	npmx_adc_config_set(npmx_adc_get(npmx_instance, 0), &config);

	/* Wait for first ADC callback to fire, this is needed for init parameters. */
	if (k_sem_take(&init_sem, K_MSEC(5)) != 0) {
		LOG_ERR("ADC callback did not fire.");
	}

	if (fuel_gauge_init() != NPMX_SUCCESS) {
		LOG_ERR("Fuel gauge initialization faiied.");
	}

	LOG_INF("Fuel gauge OK.");

	while (1) {
		k_sleep(K_FOREVER);
	}
}
