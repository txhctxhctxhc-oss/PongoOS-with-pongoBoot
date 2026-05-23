/* 
 * pongoOS - https://checkra.in
 * 
 * Copyright (C) 2019-2021 checkra1n team
 *
 * This file is part of pongoOS.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */
#include <pongo.h>
#include "input.h"

extern int iprintf(const char* fmt, ...);

static uint32_t button_irqs[8];
static uint32_t button_irq_count;

static input_key_t gpio_irq_to_input(uint32_t irq)
{
    for (uint32_t i = 0; i < button_irq_count; i++) {
        if (button_irqs[i] != irq) continue;
        switch (i) {
            case 0: return INPUT_VOLUP;
            case 1: return INPUT_VOLDOWN;
            case 2: return INPUT_HOME;
            case 3: return INPUT_SIDE;
            default: return INPUT_POWER;
        }
    }
    return INPUT_NONE;
}

void gpio_main() {
    while(1) {
        input_key_t key = gpio_irq_to_input(task_current()->irq_type);
        if (key != INPUT_NONE) {
            pongoboot_input_enqueue(key);
        } else {
            iprintf("gpio irq %x\n", task_current()->irq_type);
        }
        task_exit_irq();
    }
}
struct task gpio_task = {.name = "gpio"};

uint64_t gGpioBase;
void gpio_early_init() {
    gGpioBase = dt_get_u32_prop("gpio", "reg");
    gGpioBase += gIOBase;
}

void gpio_init() {
    uint32_t len = 0;
    dt_node_t* buttons = dt_find(gDeviceTree, "buttons");
    if (!buttons) return;
    uint32_t* interrupts = dt_prop(buttons, "interrupts", &len);
    if (!interrupts) return;
    
    for (int i=0; i<len/4; i++) {
        if (button_irq_count < (sizeof(button_irqs) / sizeof(button_irqs[0]))) {
            button_irqs[button_irq_count++] = interrupts[i];
        }
        task_register_irq(&gpio_task, gpio_main, interrupts[i]);
    }
}
