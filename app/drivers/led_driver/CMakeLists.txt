if(CONFIG_LED_DRIVER)
    target_sources(app PRIVATE led_driver.c)
    target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()