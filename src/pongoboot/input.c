#include "input.h"
#include <pongo.h>
#include <stdio.h>

extern int iprintf(const char* fmt, ...);

/* Input queue - simple circular buffer */
static input_key_t input_queue[16] = {0};
static int input_queue_head = 0;
static int input_queue_tail = 0;

void pongoboot_input_init(void)
{
    iprintf("[PongoBoot] Initializing input handler\n");
    pongoboot_input_flush();
}

int pongoboot_input_available(void)
{
    return input_queue_head != input_queue_tail;
}

input_key_t pongoboot_input_get(void)
{
    /* Blocking read - wait for input */
    while (!pongoboot_input_available()) {
        /* TODO: Yield or sleep */
        asm("nop");
    }
    return pongoboot_input_get_nonblocking();
}

input_key_t pongoboot_input_get_nonblocking(void)
{
    pongoboot_input_poll_platform();

    if (!pongoboot_input_available()) {
        return INPUT_NONE;
    }
    
    input_key_t key = input_queue[input_queue_tail];
    input_queue_tail = (input_queue_tail + 1) % 16;
    
    return key;
}

void pongoboot_input_flush(void)
{
    input_queue_head = 0;
    input_queue_tail = 0;
}

/* Called by hardware interrupt handlers to queue input */
void pongoboot_input_enqueue(input_key_t key)
{
    int new_head = (input_queue_head + 1) % 16;
    if (new_head != input_queue_tail) {
        input_queue[input_queue_head] = key;
        input_queue_head = new_head;
    }
}

static input_key_t pongoboot_key_from_char(char c)
{
    switch (c) {
        case 'w':
        case 'W':
        case 'k':
        case 'K':
        case '+':
            return INPUT_VOLUP;
        case 's':
        case 'S':
        case 'j':
        case 'J':
        case '-':
            return INPUT_VOLDOWN;
        case '\r':
        case '\n':
        case 'b':
        case 'B':
            return INPUT_HOME;
        case 0x1b:
        case 0x7f:
        case 'q':
        case 'Q':
            return INPUT_SIDE;
        case 'p':
        case 'P':
            return INPUT_POWER;
        default:
            return INPUT_NONE;
    }
}

void pongoboot_input_poll_platform(void)
{
    extern lock stdin_lock;
    extern char stdin_buf[512];
    extern uint32_t bufoff;

    lock_take(&stdin_lock);
    while (bufoff) {
        char c = stdin_buf[0];
        bufoff--;
        if (bufoff) {
            memmove(stdin_buf, stdin_buf + 1, bufoff);
        }
        lock_release(&stdin_lock);

        input_key_t key = pongoboot_key_from_char(c);
        if (key != INPUT_NONE) {
            pongoboot_input_enqueue(key);
        }

        lock_take(&stdin_lock);
    }
    lock_release(&stdin_lock);
}
