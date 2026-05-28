#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include "task2_led_driver.h"

const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(led_driver0));

static int sensor_fetch(const struct shell *sh, size_t argc, char **argv) {
    int ret = sensor_sample_fetch(dev);
    if (ret < 0) {
        shell_error(sh, "Failed to fetch sample from LED driver (%d)", ret);
        return ret;
    }
    shell_print(sh, "Sample fetch called -> LED ON");
    return 0;
}

static int sensor_chan_get(const struct shell *sh, size_t argc, char **argv) {
    //struct sensor_value val;
    int ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, NULL);
    if (ret < 0) {
        shell_error(sh, "Failed to get channel value from LED driver (%d)", ret);
        return ret;
    }
    shell_print(sh, "Channel get called -> LED OFF");
    return 0;
}

static int sensor_info(const struct shell *sh, size_t argc, char **argv) {
    const struct device *dev = shell_device_get_binding("led_driver0");
    if (dev == NULL) {
        shell_error(sh, "Device not found");
        return -ENODEV;
    }
    shell_print(sh, "Device name: %s", dev->name);
    shell_print(sh, "Device is %s", device_is_ready(dev) ? "ready" : "not ready");
    return 0;
}

static int sensor_brightness_set(const struct shell *sh, size_t argc, char **argv) {
    if(argc != 2) {
        shell_error(sh, "Invalid number of arguments. Usage: sensor brightness <value>");
        return -EINVAL;
    }

    char* endptr;
    int brightness = strtol(argv[1], &endptr, 10);
    if(*endptr != '\0' || brightness < 0 || brightness > 100) {
        shell_error(sh, "Invalid brightness value. Must be an integer between 0 and 100.");
        return -EINVAL;
    }

    int ret = led_driver_set_brightness(dev, brightness);
    if (ret < 0) {
        shell_error(sh, "Failed to set LED brightness (%d)", ret);
        return ret;
    }
    shell_print(sh, "LED brightness set to %d", brightness);
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_shell_commands,
    SHELL_CMD(fetch, NULL, "Call sensor_sample_fetch() and turn LED on", sensor_fetch),
    SHELL_CMD(read, NULL, "Call sensor_channel_get() and turn LED off", sensor_chan_get),
    SHELL_CMD(info, NULL, "Get device name and information about ready state", sensor_info),
    SHELL_CMD_ARG(brightness, NULL, "Set LED brightness (0-100)", sensor_brightness_set, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_shell_commands, "LED sensor shell commands", NULL);