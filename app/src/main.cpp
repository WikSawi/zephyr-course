#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(led_driver0));

int main(void)
{
    int ret = 0;
    if (!device_is_ready(driver)) {
        LOG_ERR("LED driver device not ready");
        return -ENODEV;
    }
    struct sensor_value val;

    while (1) {
        ret = sensor_sample_fetch(driver);
        if (ret < 0) {
            LOG_ERR("Failed to fetch sample from LED driver (%d)", ret);
            return ret;
        }
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

        ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
        if (ret < 0) {
            LOG_ERR("Failed to get channel (%d)", ret);
            return ret;
        }

        LOG_INF("LED driver channel get returned value: %d", ret);

        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}
