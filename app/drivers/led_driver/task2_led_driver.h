#ifndef TASK2_LED_DRIVER_H
#define TASK2_LED_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

int led_driver_set_brightness(const struct device *dev, int brightness);

#ifdef __cplusplus
}
#endif

#endif /* TASK2_LED_DRIVER_H */