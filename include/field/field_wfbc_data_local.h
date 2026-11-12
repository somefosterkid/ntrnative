// The functions of overlay 20's field_wfbc_data_local.c that overlay 21's field_wfbc.c and its neighbours call
#ifndef BWPORT_FIELD_FIELD_WFBC_DATA_LOCAL_H
#define BWPORT_FIELD_FIELD_WFBC_DATA_LOCAL_H

#include "types.h"
#include "field/city_state.h"
#include "struct_decls.h"

s32 func_ov020_02186278(CityState *state, s32 param_2);
void func_ov020_02186748(CityResident *resident);
void func_ov020_02186764(CityResident *resident);
BOOL func_ov020_02186770(CityResident *resident);
u8 func_ov020_021867b0(const CityResident *resident);
void func_ov020_02186818(CityResidentDataHandle *handle, u32 id);
// The table of two bytes a resident of the city's file
u8 *func_ov020_02186830(CityResidentDataHandle *handle, s32 city, HeapID heapId);
void func_ov020_0218683c(void *table);
u8 *func_ov020_02186844(u8 *table, s32 index);
// The city's residents as zone NPCs, in a new heap block (NULL when there are none)
ZoneNPC *func_ov020_02186538(CityState *state, s32 param_2, u16 param_3);
// How many of the IDs (func_020127e0's) are set
s32 func_ov020_02186854(u16 *ids);
ZoneNPC *func_ov020_021868b8(u16 *param_1, s32 param_2, s32 param_3, u16 param_4);

#endif // BWPORT_FIELD_FIELD_WFBC_DATA_LOCAL_H
