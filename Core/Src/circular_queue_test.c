#include "circular_queue.h"

#ifdef QUEUE_TEST
#include "usart.h"

void test_char_out(char* c) {
    HAL_UART_Transmit(&huart2, (uint8_t*)c, 1, HAL_MAX_DELAY);
}

void test_string_out(char* c) {
    while (*c) {
        if (*c == '\n') {
            test_char_out("\r");
        }
        test_char_out(c++);
    }
}

void test_queue_8() {
    uint8_t eight[DEFAULT_QUEUE_SIZE];
    queue eight_record;
    init_queue(&eight_record, eight, 10, QUEUE_8);

    uint8_t value = -1;
    uint8_t from_func = value;
    get_next(&eight_record, (uint32_t*)&from_func);
    
}
#endif