#ifndef CONFIG_H
#define CONFIG_H

typedef unsigned char uint8_t;
typedef char int8_t;
typedef short int int16_t;
typedef unsigned short int uint16_t;

#define CAM_STACK_SIZE 1024 /* CAM value cells */
#define CAM_HEAP_SIZE 1024 /* CAM value cells */
#define CAM_CODE_SIZE 1024 /* bytes */
typedef uint16_t cam_mem_ptr_t;

#define CAM_KBD_QUEUE_SIZE 16
#define CAM_LCD_H 2
#define CAM_LCD_W 16

#endif

