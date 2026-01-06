#include "circular_queue.h"
#include <stdint.h>

#ifndef NULL
    #define NULL ((void*) 0)
#endif

// Prototypes
void _dequeue(queue*, void*);
void _enqueue(queue*, uint32_t);
void _inc_head(queue*);
void _inc_tail(queue*);
void _set_next_addr(queue*, void**);

void pop(queue* q, void* return_item) {
    _dequeue(q, return_item);
}

void push(queue* q, uint32_t put_item) {
    _enqueue(q, put_item);
}

void init_queue(queue* record_keeper, void* first_element, uint32_t size, queue_content_size content_size) {
    record_keeper->start = first_element;
    record_keeper->head = first_element;
    record_keeper->tail = first_element;
    
    record_keeper->content_size = content_size;
    record_keeper->size = size;
    record_keeper->number_enqueued = 0;    
}

void _dequeue(queue* q, void* return_item) {
    if (q->number_enqueued == 0) {
        return;
    }
    switch (q->content_size) {
        case QUEUE_8:
        *(uint8_t*)return_item  = *(uint8_t*) q->tail; 
        case QUEUE_16: 
        *(uint16_t*)return_item = *(uint16_t*)q->tail; 
        case QUEUE_32:
        *(uint32_t*)return_item = *(uint32_t*)q->tail;
    }
    q->number_enqueued -= 1;
    _inc_tail(q);
}

void _enqueue(queue* q, uint32_t new_val) {
    if (q->number_enqueued >= q->size) {
        return;
    }
    switch (q->content_size) {
        case QUEUE_8:
        *(uint8_t*)q->head  = (uint8_t)  new_val; 
        case QUEUE_16: 
        *(uint16_t*)q->head = (uint16_t) new_val;
        case QUEUE_32:
        *(uint32_t*)q->head = (uint32_t) new_val;
    }
    q->number_enqueued += 1;
    _inc_head(q);
}

void _inc_head(queue* q) {
    _set_next_addr(q, &(q->head));
}

void _inc_tail(queue* q) {
    _set_next_addr(q, &(q->tail));
}

void _set_next_addr(queue* q, void** ptr_addr) {
    if (q->start + (q->size * q->content_size) < *ptr_addr + q->content_size) {
        *ptr_addr = q->start;
    }
    *ptr_addr += q->content_size;
}
