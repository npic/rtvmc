#include "cam_devices.h"
#include "aduc812.h"
#include "lcd.h"
#include "max.h"

static void write_max(uint8_t regnum, uint8_t val)
{
    uint8_t oldDPP = DPP;

    DPP = MAXBASE;
    *(uint8_t __xdata *)regnum = val;
    DPP = oldDPP;
}

static uint8_t read_max(uint8_t regnum) 
{ 
    uint8_t oldDPP = DPP; 
    uint8_t val = 0; 
    DPP = MAXBASE; 
    val = *(uint8_t __xdata *)regnum; 
    DPP = oldDPP; 
    return val; 
}

static void strobe(char c)
{
    uint16_t i;

    write_max(C_IND, c | 0x1);
    write_max(C_IND, c & 0xFE);
    for (i = 0; i < 300; i++) continue;
}

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

static errcode_t scan_kbd(char * ch)
{
    static char __xdata KBTable[] = "147*2580369#ABCD";
    uint8_t row, col, rownum, colnum;
    uint16_t i;

    for (colnum = 0; colnum < 4; colnum++)
    {
        col = 0x1 << colnum;
        write_max(KB, ~col);

        for (rownum = 0; rownum < 4; rownum++)
        {
            row = read_max(KB) & (0x10 << rownum);
            if (!row)
            {       
                for (i = 0; i < 20000; i++) continue;
                row = read_max(KB) & (0x10 << rownum);
                if (!row)
                {
                    *ch = (KBTable[(colnum<<2) + rownum]);
                    return OK;
                }
            }
        }
    }
    return OK_NEGATIVE;
}

errcode_t sync_kbd(cam_kbd_t * kbd)
{
    errcode_t retval;
    char c;

    if (scan_kbd(&c) == OK)
    {
        retval = kbd_enqueue(kbd, c);
        CHECK_OK(retval, "Failed to enqueue keyboard key\n");
    }
    return OK;
}

errcode_t sync_led_row(cam_led_row_t * led_row)
{
    write_max(SV, led_row->state);
    led_row->is_dirty = 0;
    return OK;
}

errcode_t init_lcd(cam_lcd_t * lcd)
{
    errcode_t retval;
    uint16_t i;

    retval = lcd_clear(lcd);
    CHECK_OK(retval, "Failed to clear LCD\n");

    for (i = 0; i < 4000; i++) continue; 
    write_max(DATA_IND, FUNCTION_SET|EIGHT_BITS);
    strobe(0x8);

    for (i = 0; i < 1500; i++) continue; 
    write_max(DATA_IND, FUNCTION_SET|EIGHT_BITS);
    strobe(0x8);

    for (i = 0; i < 50; i++) continue;
    write_max(DATA_IND, FUNCTION_SET|EIGHT_BITS);
    strobe(0x8);

    write_max(DATA_IND, FUNCTION_SET|EIGHT_BITS|TWO_LINE);
    strobe(0x8);

    write_max(DATA_IND, DISPLAY_CTRL);
    strobe(0x8);

    write_max(DATA_IND, CLEAR); 
    strobe(0x8);

    write_max(DATA_IND, ENTRY_MODE|INCR);
    strobe(0x8);

    write_max(DATA_IND, DISPLAY_CTRL|DISPLAY_ON);
    strobe(0x8);

    retval = sync_lcd(lcd);
    CHECK_OK(retval, "Failed to sync LCD\n");
    return OK;
}

errcode_t sync_lcd(cam_lcd_t * lcd)
{
    uint8_t x, y;
    /* clear lcd */
    write_max(DATA_IND, CLEAR);
    strobe(0x8);
    /* write lcd state */
    x = y = 0;
    while (y < CAM_LCD_H)
    {
        /* goto X Y */
        write_max(DATA_IND, RAM_DD | (x + ((y) ? 0x40 : 0)));
        strobe(0x8);
        /* write char */
        write_max(DATA_IND, lcd->state[y][x]);
        strobe(0xC);
        x++;
        if (x > CAM_LCD_W - 1)
        {
            x = 0;
            y++;
        }
    }
    lcd->is_dirty = 0;
    return OK;
}

