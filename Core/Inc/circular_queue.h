#ifndef __CIRC_QUEUE_H__
#define __CIRC_QUEUE_H__

#include <stdint.h>

#define DEFAULT_QUEUE_SIZE (16)

#define QUEUE_TEST // enables test suite in circular queue.c for when I need to test functionality

// Queue has no idea of the size of an item at runtime, so this is what it uses to determine how many bytes per element
typedef enum {
    QUEUE_8 = 1,
    QUEUE_16 = 2,
    QUEUE_32 = 4
} queue_content_size;

// queue record definiton
typedef struct {
    void* start;
    void* head;
    void* tail;
    uint32_t number_enqueued;
    uint32_t size;
    queue_content_size content_size;
} queue;

void get_next(queue* q, uint32_t* return_item);
void put_next(queue* q, uint32_t put_item);
void init_queue(queue* record_keeper, void* first_element, uint32_t size, queue_content_size content_size);

#endif
