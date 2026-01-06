#include "circular_queue.h"
#include <stdint.h>

#ifdef QUEUE_TEST
#include "usart.h"
#include "stdarg.h"
#include "stdio.h"

char OUTPUT[256];
int test_number = 0;

void test_char_out(char* c) {
    HAL_UART_Transmit(&huart2, (uint8_t*)c, 1, HAL_MAX_DELAY);
}

void test_string_out(char* format, ...) {
    va_list aptr ;
    char* out = OUTPUT;

    va_start(aptr, format) ;
    vsprintf(out, format, aptr) ;
    va_end(aptr) ;

    while (*out) {
        if (*out == '\n') {
            test_char_out("\r");
        }
        test_char_out(out++);
    }
}

void print_test_number() {
    test_string_out("TEST %d: ", test_number++);
}

void print_test_with_name(char* name) {
    print_test_number();
    test_string_out("%s\n", name);
}

void print_sub_test(char* name) {
    test_string_out("\t%s: ", name);
}

void val_equals(uint32_t a, uint32_t b) {
    if (a == b) {
        test_string_out("\tGood\n");
    } else {
        test_string_out("\tBad\n");
    }
}

void ptr_equals(void* addr1, void* addr2) {
    val_equals((uint32_t) addr1, (uint32_t) addr2);
}

void test_queue_8() {
    uint8_t eight[DEFAULT_QUEUE_SIZE];
    queue eight_record;
    init_queue(&eight_record, eight, 10, QUEUE_8);

    print_test_with_name("Queue Elements");
    print_sub_test("Start");
    ptr_equals(eight_record.start, eight);
    print_sub_test("Head");
    ptr_equals(eight_record.head, eight);
    print_sub_test("Tail");
    ptr_equals(eight_record.tail, eight);
    print_sub_test("Max Size");
    val_equals(eight_record.size, DEFAULT_QUEUE_SIZE);
    print_sub_test("Number Enqueued");
    val_equals(eight_record.number_enqueued, 0);
    print_sub_test("Content Size");
    val_equals(eight_record.content_size, QUEUE_8);

    uint8_t value = -1;
    uint8_t from_func = value;
    print_test_with_name("Pop From Empty Queue");
    pop(&eight_record, (uint32_t*)&from_func);
    val_equals(value, from_func);

    print_test_with_name("Push and Pop -1");
    push(&eight_record, value);
    pop(&eight_record, (uint32_t*)&from_func);
    

}
#endif