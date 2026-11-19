#include "global.h"

#include "heap.h"
#include "overlay_01_021EFB38.h"
#include "bg_window.h"
#include "overlay_01_unk_struct_ov5_021de6bc.h"

UnkStruct_ov5_021DE6BC *ov01_021F0848(s32);

typedef struct UnkStruct_ov5_021DE928 {
    Window *unk_00;
    UnkStruct_ov5_021DE6BC *unk_04[48];
    u8 unk_C4;
    u8 unk_C5;
    u8 unk_C6;
    u8 unk_C7;
    s8 unk_C8;
    u8 unk_C9;
    u8 unk_CA;
    u8 unk_CB[1];
} UnkStruct_ov5_021DE928;
void ov01_021F0A0C(UnkStruct_ov5_021DE928 *param0, u8 param1, u8 param2, Window *param3, u8 param4);

void *ov01_021F09BC(s32 arg0) {
    s32 v1;
    UnkStruct_ov5_021DE928 *v0;

    v0 = Heap_Alloc(HEAP_ID_FIELD1, sizeof(UnkStruct_ov5_021DE928));
    memset(v0, 0, sizeof(UnkStruct_ov5_021DE928));
    v1 = 0;
    do {
        v0->unk_04[v1] = ov01_021F0848(arg0);
        v1 += 1;
    } while (v1 < 0x30);
    return v0;
}

SysTask *ov01_021F0860(UnkStruct_ov5_021DE6BC *);

void ov01_021F09EC(void *ptr) {
    s32 v0;
    UnkStruct_ov5_021DE928 *v1;

    v0 = 0;
    v1 = ptr;
    do {
        ov01_021F0860(v1->unk_04[v0]);
        v0 += 1;
    } while (v0 < 0x30);
    Heap_Free(ptr);
}

void ov01_021F0A0C(UnkStruct_ov5_021DE928 *param0, u8 param1, u8 param2, Window *param3, u8 param4) {
    param0->unk_00 = param3;
    param0->unk_C9 = param4;
    param0->unk_C6 = param1;
    param0->unk_C7 = param2;
    param0->unk_C4 = 0;
    param0->unk_C5 = 0;
    param0->unk_C8 = 0;
    param0->unk_CA = 1;
}
