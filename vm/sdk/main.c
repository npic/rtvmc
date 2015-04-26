#include <stdlib.h>
#include "cam.h"

#define CHECK_OK_SDK(val) \
    if((val) != OK)       \
    {                     \
        break;            \
    }

cam_t __xdata cam;

void main(void)
{
    errcode_t retval;

    do
    {
        retval = init_cam(&cam);
        CHECK_OK_SDK(retval);
        retval = load_program(&cam, NULL);
        CHECK_OK_SDK(retval);
        while (!cam.is_halted)
        {
            retval = step(&cam);
            CHECK_OK_SDK(retval);
            retval = sync_kbd(&cam.keyboard);
            CHECK_OK_SDK(retval);
            if (cam.LED_row.is_dirty)
            {
                retval = sync_led_row(&cam.LED_row);
                CHECK_OK_SDK(retval);
            }
            if (cam.LCD.is_dirty)
            {
                retval = sync_lcd(&cam.LCD);
                CHECK_OK_SDK(retval);
            }
        }
    } while (0);
    while (1) { }
}

