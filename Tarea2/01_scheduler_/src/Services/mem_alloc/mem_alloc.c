#include "mem_alloc.h"

extern uint8_t _heap_mem_start[];
extern uint8_t _heap_mem_end[];

MemHandlerType MemControl = {
    .MemStart = (uint8_t *)&_heap_mem_start,   /* Sets the start of the heap memory */
    .MemEnd = (uint8_t *)&_heap_mem_end,       /* Sets the end of the heap memory */
    .CurrAddr = (uint8_t *)&_heap_mem_start,   /* Initialize the current start address */
    .FreeBytes = 0,                            /* Sets the size of the heap memory */
};

void Mem_Init(void)
{
  uint32_t *pSrc, *pDest;
  /* Clear the heap segment */
	for (pDest = &_heap_mem_start; pDest < &_heap_mem_end;) {
			*pDest++ = 0;
	}
                  
  MemControl.FreeBytes = (uint8_t *)&_heap_mem_end - (uint8_t *)&_heap_mem_start;  /* Sets the size of the heap memory */
}
  
MemReturnType Mem_Alloc(MemSizeType size)
{
    if (size > MemControl.FreeBytes)
    {
        return NULL;
    }

    size_t request_size = size + MEM_ALLOC_ALIGN;
    uint8_t *ptr = (uint8_t *)(malloc(request_size));
    size_t remainder = ((size_t)ptr) % MEM_ALLOC_ALIGN;
    size_t offset = MEM_ALLOC_ALIGN - remainder;
    uint8_t *ret = ptr + (uint8_t)offset;
    *(uint8_t *)(ret - 1) = offset;
    
    MemControl.CurrAddr = ret + request_size;
    MemControl.FreeBytes = MemControl.FreeBytes - request_size;

    return (void *)ret;
}
