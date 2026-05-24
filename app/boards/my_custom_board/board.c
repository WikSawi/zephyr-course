#include <zephyr/init.h>
#include <zephyr/kernel.h>

static int board_my_custom_board_init(void)
{
    printk("Board Initialized\n");
    return 0;
}