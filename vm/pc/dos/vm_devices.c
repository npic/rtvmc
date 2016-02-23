#include <stdlib.h>
#include <sys/farptr.h> /* _farpeekb(), _farpokeb() */
#include <go32.h>
#include <dpmi.h>
#include <crt0.h>
#include <dos.h>
#include "vm.h"
#include "vm_devices.h"

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2+1)
#define PIC_EOI 0x20

#define TIMER_INDEX_PORT 0x70
#define TIMER_DATA_PORT 0x71
#define TIMER_VECT 0x70
#define TIMER_IRQ 0x08

typedef struct vector_t vector_t;
struct vector_t {
    _go32_dpmi_seginfo old_v, new_v;
};

extern vm_t vm;
vector_t old_timer_vector;
int _crt0_startup_flags = _CRT0_FLAG_LOCK_MEMORY;

/*
errcode_t init_uart(vm_uart_t * uart)
{
    uint16_t i;
    for (i = 0; i < VM_UART_QUEUE_SIZE; i++)
    {
        uart->in.queue[i] = '\0';
        uart->out.queue[i] = '\0';
    }
    uart->in.queue_start = uart->in.queue_end = uart->in.queue;
    uart->out.queue_start = uart->out.queue_end = uart->out.queue;
    return OK;
}
*/

errcode_t gpio_get(uint32_t pin, uint8_t * result)
{
    /*TODO*/
    if(pin == 5) {
        *result = (uint8_t)_farpeekb(_dos_ds, 0xB8000);
    }
    return OK;
}

errcode_t gpio_set(uint32_t pin, uint8_t value)
{
    /*TODO*/
    if(pin == 5) {
        _farpokeb(_dos_ds, 0xB8000, (value ? '#' : ' '));
    }
    return OK;
}

static void save_vector(vector_t *vec, uint8_t num) {
    _go32_dpmi_get_protected_mode_interrupt_vector(num, &vec->old_v);
}

static void install_handler(vector_t *vec, uint8_t num, void (*fn)(void)) {
    vec->new_v.pm_selector = _my_cs();
    vec->new_v.pm_offset = (unsigned long)fn;
    _go32_dpmi_allocate_iret_wrapper(&vec->new_v);
    _go32_dpmi_set_protected_mode_interrupt_vector(num, &vec->new_v);
}

static void restore_vector(vector_t *vec, uint8_t num) {
    _go32_dpmi_set_protected_mode_interrupt_vector(num, &vec->old_v);
    _go32_dpmi_free_iret_wrapper(&vec->new_v);
}

void restore_timer_vector(void) {
    restore_vector(&old_timer_vector, TIMER_VECT);
}

static uint8_t read_rtc(uint8_t reg) {
    outportb(TIMER_INDEX_PORT, reg | (1<<7));
    return inportb(TIMER_DATA_PORT);
}

static void write_rtc(uint8_t reg, uint8_t val) {
    outportb(TIMER_INDEX_PORT, reg | (1<<7));
    outportb(TIMER_DATA_PORT, val);
}

static void init_rtc(void) {
    write_rtc(0xB, read_rtc(0xB) | 0x40);
    read_rtc(0xC);
}

/* Not used */
/* static void irq_set_mask(uint8_t irq_line) { */
/*     uint16_t port; */
/*     uint8_t value; */

/*     if(irq_line < 8) { */
/*         port = PIC1_DATA; */
/*     } else { */
/*         port = PIC2_DATA; */
/*         irq_line -= 8; */
/*     } */
/*     value = inportb(port) | (1 << irq_line); */
/*     outportb(port, value); */
/* } */

static void irq_clear_mask(uint8_t irq_line) {
    uint16_t port;
    uint8_t value;

    if(irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    value = inportb(port) & ~(1 << irq_line);
    outportb(port, value);
}

static void timer_int(void)
{
    vm.time++;
    vm.proc_table[vm.PP].current_observed_time++;
    reschedule(&vm);
    read_rtc(0xC);
    outportb(PIC2_COMMAND, PIC_EOI);
    outportb(PIC1_COMMAND, PIC_EOI);
}

errcode_t init_hw()
{
    init_rtc();
    save_vector(&old_timer_vector, TIMER_VECT);
    install_handler(&old_timer_vector, TIMER_VECT, timer_int);
    irq_clear_mask(TIMER_IRQ);
    atexit(restore_timer_vector);
    return OK;
}
