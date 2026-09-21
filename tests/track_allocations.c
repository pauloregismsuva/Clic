/* Optional GNU/LLVM linker wrappers: count allocations made by our objects,
 * including the cleanup paths of partially constructed rows. */
#include <assert.h>
#include <stddef.h>

void *__real_malloc(size_t size);
void *__real_calloc(size_t count, size_t size);
void __real_free(void *pointer);

size_t Test_outstandingAllocations = 0;

void *__wrap_malloc(size_t size) {
    void *pointer = __real_malloc(size);
    if (pointer) Test_outstandingAllocations++;
    return pointer;
}

void *__wrap_calloc(size_t count, size_t size) {
    void *pointer = __real_calloc(count, size);
    if (pointer) Test_outstandingAllocations++;
    return pointer;
}

void __wrap_free(void *pointer) {
    if (pointer) {
        assert(Test_outstandingAllocations > 0);
        Test_outstandingAllocations--;
    }
    __real_free(pointer);
}
