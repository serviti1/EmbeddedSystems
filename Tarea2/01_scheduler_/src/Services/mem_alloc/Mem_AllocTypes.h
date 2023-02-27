
/*******************************************************************************/
/**
\file       app_tasks.h
\brief      Memory allocation
\author     Jesus Cinco
\author     Snaider Bautista
\version    0.1
\date       26/Feb/2023
*/

#ifndef __MEM_ALLOC_TYPES
#define __MEM_ALLOC_TYPES

/** Core modules */
#include "compiler.h"

typedef void *MemReturnType;

typedef uint16_t MemSizeType;

typedef struct MemHandlerType
{
    uint8_t *MemStart;
    uint8_t *MemEnd;
    uint8_t *CurrAddr;
    uint32_t FreeBytes;
} MemHandlerType;

#endif