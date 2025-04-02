#ifndef RUNNING
#define RUNNING

#ifndef ARR_SIZE // preprocessor step because size of arr will not change on execution
#define ARR_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

char get_char();

void running() ;

#endif
