#include "cam.h"

errcode_t load_program(cam_t * cam, const char * filename)
{
    (void)filename; /* Suppress compiler warning */
    /* kbdtest.bin */
    cam->code[0] = 0x02;
    cam->code[1] = 0x00;
    cam->code[2] = 0x00;
    cam->code[3] = 0x07;
    cam->code[4] = 0x16;
    cam->code[5] = 0x10;
    cam->code[6] = 0x00;
    cam->code[7] = 0x03;
    cam->code[8] = 0x07;
    cam->code[9] = 0x17;
    cam->code[10] = 0x13;
    cam->code[11] = 0x07;
    cam->code[12] = 0x02;
    cam->code[13] = 0xFF;
    cam->code[14] = 0xD0;
    cam->code[15] = 0x13;
    cam->code[16] = 0x19;
    cam->code[17] = 0x15;
    cam->code[18] = 0x01;
    cam->code[19] = 0x12;
    cam->code[20] = 0x00;
    cam->code[21] = 0x00;
    /* lcd.bin */
    /*cam->code[0] = 0x1A;
    cam->code[1] = 0x15;
    cam->code[2] = 0x02;
    cam->code[3] = 0x03;
    cam->code[4] = 0x07;
    cam->code[5] = 0x02;
    cam->code[6] = 0x00;
    cam->code[7] = 0x21;
    cam->code[8] = 0x04;
    cam->code[9] = 0x07;
    cam->code[10] = 0x02;
    cam->code[11] = 0x00;
    cam->code[12] = 0x6F;
    cam->code[13] = 0x04;
    cam->code[14] = 0x07;
    cam->code[15] = 0x02;
    cam->code[16] = 0x00;
    cam->code[17] = 0x6C;
    cam->code[18] = 0x04;
    cam->code[19] = 0x07;
    cam->code[20] = 0x02;
    cam->code[21] = 0x00;
    cam->code[22] = 0x6C;
    cam->code[23] = 0x04;
    cam->code[24] = 0x07;
    cam->code[25] = 0x02;
    cam->code[26] = 0x00;
    cam->code[27] = 0x65;
    cam->code[28] = 0x04;
    cam->code[29] = 0x07;
    cam->code[30] = 0x02;
    cam->code[31] = 0x00;
    cam->code[32] = 0x48;
    cam->code[33] = 0x04;
    cam->code[34] = 0x07;
    cam->code[35] = 0x02;
    cam->code[36] = 0x00;
    cam->code[37] = 0x00;
    cam->code[38] = 0x08;
    cam->code[39] = 0x07;
    cam->code[40] = 0x02;
    cam->code[41] = 0x00;
    cam->code[42] = 0x05;
    cam->code[43] = 0x08;
    cam->code[44] = 0x1B;
    cam->code[45] = 0x15;
    cam->code[46] = 0x02;
    cam->code[47] = 0x00;*/
    return OK;
}

