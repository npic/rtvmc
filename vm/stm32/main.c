/* TODO: get rid of carriage return plague */

#include <stdlib.h>
#include "vm.h"

vm_t vm;

int main(void)
{
    errcode_t retval;
    /*
    char uart_out_char;
    */

    retval = init_vm(&vm);
    CHECK_OK(retval, "Failed to init vm\n");
    retval = load_program(&vm, NULL);
    CHECK_OK(retval, "Failed to load program\n");
    retval = reschedule(&vm);
    CHECK_OK(retval, "Failed to do initial scheduling\n");
    while (!vm.is_halted)
    {
        /* TODO IO here? */
        /*
        retval = dequeue(&vm.uart.out, &uart_out_char);
        CHECK_OK(retval, "Failed to dequeue from UART out\n");
        if (retval == OK)
        {
            if (uart_out_char)
            {
                BSP_LED_On(LED2);
            }
            else
            {
                BSP_LED_Off(LED2);
            }
        }
        */
        vm.PP = vm.NPP;
        retval = step(&vm);
        CHECK_OK(retval, "Failed to execute instruction\n");
    }

    while (1)
    {    
    }
}
