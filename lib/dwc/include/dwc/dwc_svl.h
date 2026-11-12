#ifndef DWC_SVL_H_
#define DWC_SVL_H_

#include "types.h"

BOOL DWC_Svl_Init(void *(*alloc)(int, u32), void (*free)(int, void *, int));
BOOL DWC_Svl_GetTokenAsync(const char *svl, void *result);
u32 DWC_Svl_Process(void);
void DWC_Svl_Cleanup(void);

#endif
