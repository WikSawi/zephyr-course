if(CONFIG_LED_DRIVER)
    target_sources(app PRIVATE led_driver.c)
endif()