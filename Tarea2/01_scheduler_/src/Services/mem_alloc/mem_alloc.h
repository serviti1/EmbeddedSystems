/*******************************************************************************/
/**
\file       app_tasks.h
\brief      Memory allocation
\author     Jesus Cinco
\author     Snaider Bautista
\version    0.1
\date       26/Feb/2023
*/

#ifndef __MEM_ALLOC
#define __MEM_ALLOC

#include "Mem_AllocTypes.h"

#define MEM_ALLOC_ALIGN 4

void Mem_Init(void);
MemReturnType Mem_Alloc(MemSizeType size);

#endif