#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "unk_02037C94.h"
#include "unk_02091564.h"

struct UnionRoomManager *sub_02059E1C(FieldSystem *);

s32 sub_0205A034(struct UnionRoomManager *, s32 *, s32);

extern s32 sub_02059E88;

struct UnionRoomManager *sub_02059DB0(FieldSystem *fieldSystem);

struct UnionRoomManager *sub_02059DB0(FieldSystem *fieldSystem) {
    struct UnionRoomManager *v0;

    if (fieldSystem == NULL) {
        GF_AssertFail();
    }
    if (fieldSystem->unk80 != NULL) {
        return NULL;
    }
    Heap_CreateAtEnd(HEAP_ID_3, HEAP_ID_31, 0xA80U);
    v0 = sub_02059E1C(fieldSystem);
    if (v0 == NULL) {
        v0 = fieldSystem->unk80;
    }
    sub_02091574(fieldSystem);
    sub_02038C1C(2);
    sub_0205A034(v0, &sub_02059E88, 0x28);
    return v0;
}
