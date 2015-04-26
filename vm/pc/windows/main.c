#include "cam.h"

int main(int argc, char ** argv)
{
    errcode_t retval;
    static cam_t cam;

    if (argc != 2)
    {
        FAIL(BAD_ARGUMENT, "No program file specified\n");
    }
    retval = init_cam(&cam);
    CHECK_OK(retval, "Failed to init CAM\n");
    retval = load_program(&cam, argv[1]);
    CHECK_OK(retval, "Failed to load program\n");
    while (!cam.is_halted)
    {
        retval = step(&cam);
        CHECK_OK(retval, "Failed to execute instruction\n");
        retval = sync_kbd(&cam.keyboard);
        CHECK_OK(retval, "Failed to sync keyboard\n");
        if (cam.LED_row.is_dirty)
        {
            retval = sync_led_row(&cam.LED_row);
            CHECK_OK(retval, "Failed to sync LED row\n");
        }
        if (cam.LCD.is_dirty)
        {
            retval = sync_lcd(&cam.LCD);
            CHECK_OK(retval, "Failed to sync LCD\n");
        }
    }
    return OK;
}

