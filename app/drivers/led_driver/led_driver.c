#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT led_driver

#define LED_NODE DT_ALIAS(led1)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(led_driver, LOG_LEVEL_INF);

static int led_driver_channel_get(const struct device *dev, 
                     enum sensor_channel chan, 
                     struct sensor_value *val) {
        LOG_INF("LED driver channel get called for channel %d", chan);

    int ret = gpio_pin_set_dt(&led, 0);
    if (ret < 0) {
        LOG_ERR("Failed to turn LED off (%d)", ret);
        return ret;
    }
    return 0;                   
}

static int led_driver_sample_fetch(const struct device *dev,
				     enum sensor_channel chan) {

    int ret = gpio_pin_set_dt(&led, 1);
    if (ret < 0) {
        LOG_ERR("Failed to turn LED on (%d)", ret);
        return ret;
    }
    return 0;   
}

static DEVICE_API(sensor, led_driver_api) = {
    .sample_fetch = led_driver_sample_fetch,
    .channel_get = led_driver_channel_get,
};

static int init(const struct device *dev) {
    LOG_INF("LED driver initialized");

    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("LED node not ready");
	    return -ENODEV;
    }

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE) < 0) {
        LOG_ERR("Failed to configure LED pin");
        return -ENODEV;
    }

    return 0;
}

DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, NULL, POST_KERNEL,
              80, &led_driver_api);