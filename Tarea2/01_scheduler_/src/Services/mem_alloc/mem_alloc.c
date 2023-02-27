#include "mem_alloc.h"

MemHandlerType MemControl = {
    .MemStart = (uint8_t *)&_heap_mem_start,                              /* Sets the start of the heap memory */
    .MemEnd = (uint8_t *)&_heap_mem_end,                                  /* Sets the end of the heap memory */
    .CurrAddr = (uint8_t *)&_heap_mem_start,                              /* Initialize the current start address */
    .FreeBytes = (uint8_t *)&_heap_mem_end - (uint8_t *)&_heap_mem_start  /* Sets the size of the heap memory */
};

MemReturnType Mem_Alloc(MemSizeType size)
{
    if (size > MemControl.FreeBytes)
    {
        return NULL;
    }

    size_t request_size = size + MEM_ALLOC_ALIGN;
    uint8_t *ptr = (uint8_t *)(malloc(request_size));
    size_t remaider = ((size_t)ptr) % MEM_ALLOC_ALIGN;
    size_t offset = MEM_ALLOC_ALIGN - remainder;
    uint8_t *ret = ptr + (uint8_t)offset;
    *(uint8_t *)(ret - 1) = offset;

    return (void *)ret;
}