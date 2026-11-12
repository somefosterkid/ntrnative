// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_adapter.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTL_ADAPTER_H
#define BWPORT_BATTLE_BTL_ADAPTER_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Sets up the adapters, sharing one per client unless commMode is set
void func_ov093_021eea30(u8 commMode);
BtlAdapter *func_ov093_021eea58(void *netHandle, u8 clientId, BOOL flag, HeapID heapId);
void func_ov093_021eead0(BtlAdapter *adapter);
void func_ov093_021eeb00(BtlAdapter *adapter, u32 cmd, const void *data, u32 size);
BOOL func_ov093_021eeb2c(BtlAdapter *adapter);
// The client's reply to the last command, and its size if size isn't NULL
void *func_ov093_021eeb64(BtlAdapter *adapter, u32 *size);
void func_ov093_021eeb70(BtlAdapter *adapter);
void func_ov093_021eebdc(BtlAdapter *adapter);
u8 func_ov093_021eebe8(BtlAdapter *adapter);
// The data the adapter received, and its size
u16 func_ov093_021eec20(BtlAdapter *adapter, const void **data);
BOOL func_ov093_021eec4c(BtlAdapter *adapter, const void *data, u32 size);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov093_021eea54(void);
void func_ov093_021eeae0(BtlAdapter *adapter);
void func_ov093_021eeae8(void);
void func_ov093_021eeafc(void);

#endif // BWPORT_BATTLE_BTL_ADAPTER_H
