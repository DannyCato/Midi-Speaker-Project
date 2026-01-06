#include "circular_queue.h"
#include <stdint.h>

#ifdef QUEUE_TEST
#include "usart.h"
#include "stdarg.h"
#include "stdio.h"

#define TRUE  (1)
#define FALSE (0)

char OUTPUT[256];
int test_number = 0;

queue* active_queue;

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
    test_string_out("\t%s?: ", name);
}

void val_equal(uint32_t a, uint32_t b, uint8_t is_equal) {
    char* result; 
    char* operand;
    if (a == b) {
        result = (is_equal) ? "Good" : "Bad";
        operand = "==";
    } else {
        result = (is_equal) ? "Bad" : "Good";
        operand = "!=";
    }

    test_string_out("\t%s, %d %s %d\n", result, a, operand, b);
}

void ptr_equal(void* addr1, void* addr2, uint8_t is_equal) {
    val_equal((uint32_t) addr1, (uint32_t) addr2, is_equal);
}

void test_record_elements(queue* q, void* origin, queue_content_size content_size) {
    print_test_with_name("Queue Record Elements");
    print_sub_test("Start");
    ptr_equal(q->start, origin, TRUE);
    print_sub_test("Head");
    ptr_equal(q->head, origin, TRUE);
    print_sub_test("Tail");
    ptr_equal(q->tail, origin, TRUE);
    print_sub_test("Max Size");
    val_equal(q->size, DEFAULT_QUEUE_SIZE, TRUE);
    print_sub_test("Number Enqueued");
    val_equal(q->number_enqueued, 0, TRUE);
    print_sub_test("Content Size");
    val_equal(q->content_size, content_size, TRUE);
}

void active_push(uint32_t item) {
    push(active_queue, item);
}

void active_pop(uint32_t* addr) {
    pop(active_queue, addr);
}

void tests() {
    uint32_t from_func = -1;
    print_test_with_name("Pop From Empty Queue");
    active_pop(&from_func);
    val_equal(from_func, -1, TRUE);

    print_test_with_name("Push and Pop [0]");
    active_push(0);
    active_pop(&from_func);
    print_sub_test("Correct Value");
    val_equal(from_func, 0, TRUE);
    print_sub_test("Head and Tail Equal");
    ptr_equal(active_queue->head, active_queue->tail, TRUE);

    print_test_with_name("Push and Pop [1,2]");
    active_push(1);
    active_push(2);
    active_pop(&from_func);
    print_sub_test("Correct First Value");
    val_equal(from_func, 1, TRUE);
    active_pop(&from_func);
    print_sub_test("Correct Second Value");
    val_equal(from_func, 2, TRUE);
    print_sub_test("Head and Tail Equal");
    ptr_equal(active_queue->head, active_queue->tail, TRUE);

    print_test_with_name("Push to end of underlying array [3,4, ... ,DEFAULT_QUEUE_SIZE]");
    for (int i = active_queue->head - active_queue->start; i < DEFAULT_QUEUE_SIZE + 1; i++ ) {
        active_push(i);
    }

}

void test_queue_8() {
    uint8_t eight[DEFAULT_QUEUE_SIZE];
    queue eight_record;
    active_queue = &eight_record;
    init_queue(&eight_record, eight, 10, QUEUE_8);
    test_record_elements(&eight_record, eight, QUEUE_8);

    

}
#endif
