#ifndef PONGOBOOT_INPUT_H
#define PONGOBOOT_INPUT_H

#include <stdint.h>

/* Input Key Types */
typedef enum {
    INPUT_VOLUP     = 0x01,         /* Volume up button */
    INPUT_VOLDOWN   = 0x02,         /* Volume down button */
    INPUT_HOME      = 0x04,         /* Home button / Enter */
    INPUT_SIDE      = 0x08,         /* Side button / Escape */
    INPUT_POWER     = 0x10,         /* Power button */
    INPUT_NONE      = 0x00,
} input_key_t;

/* Input Handler Functions */
void pongoboot_input_init(void);
int pongoboot_input_available(void);
input_key_t pongoboot_input_get(void);
input_key_t pongoboot_input_get_nonblocking(void);
void pongoboot_input_flush(void);
void pongoboot_input_enqueue(input_key_t key);
void pongoboot_input_poll_platform(void);

#endif /* PONGOBOOT_INPUT_H */
