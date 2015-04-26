#include <conio.h>
#include "cam_devices.h"

errcode_t init_kbd(cam_kbd_t * kbd)
{
    uint16_t i;
    for (i = 0; i < CAM_KBD_QUEUE_SIZE; i++)
    {
        kbd->queue[i] = '\0';
    }
    kbd->queue_start = kbd->queue_end = kbd->queue;
    return OK;
}

errcode_t sync_kbd(cam_kbd_t * kbd)
{
    errcode_t retval;
    if (kbhit())
    {
        retval = kbd_enqueue(kbd, getch());
        CHECK_OK(retval, "Failed to enqueue keyboard key\n");
    }
    return OK;
}

