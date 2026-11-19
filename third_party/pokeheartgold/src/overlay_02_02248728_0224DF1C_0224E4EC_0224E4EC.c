#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "overlay_02.h"
#include "player_avatar.h"
#include "save.h"
#include "unk_0205FD20.h"

s32 ov02_0224E31C(s32, s32);

BOOL ov02_0224E828(FieldSystem *, s16, s16, u8, fx32, s16 *, s16 *, u8);

BOOL ov02_0224EB48(FieldSystem *, s16, s16, u8, fx32, s16 *, s16 *, u8);

BOOL ov02_0224EE4C(FieldSystem *, s16, s16, u8, fx32, s16 *, s16 *, u8);

const fx32 sub_02054774(const FieldSystem *, fx32, fx32, fx32, u8 *);

SafariDecorationArgs *SafariDecoration_CreateArgs(FieldSystem *fieldSystem, enum HeapID heapID) {
    u8 uVar1;
    SafariDecorationArgs *dest;
    SaveData *pSVar2;
    s32 sVar3;
    u32 uVar4;
    s16 iVar5;
    u8 direction;
    u32 uVar6;
    s32 iVar7;
    s16 iVar8;
    u8 bVar9;
    s16 auStack_26[1];
    u8 acStack_28[2];
    s16 auStack_24[2];
    VecFx32 VStack_20;

    dest = Heap_AllocAtEnd(heapID, sizeof(SafariDecorationArgs));
    MI_CpuFill8(dest, 0, sizeof(SafariDecorationArgs));
    pSVar2 = FieldSystem_GetSaveData(fieldSystem);
    dest->saveData = pSVar2;
    dest->menuInputStatePtr = &fieldSystem->menuInputState;
    dest->unk20 = (u8 *)fieldSystem + 0x111;
    sVar3 = PlayerAvatar_GetState(fieldSystem->playerAvatar);
    bVar9 = sVar3 == 2;
    dest->side = bVar9;
    uVar4 = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    direction = uVar4;
    uVar4 = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
    iVar5 = uVar4;
    uVar6 = PlayerAvatar_GetZCoord(fieldSystem->playerAvatar);
    iVar8 = uVar6;
    PlayerAvatar_CopyPositionVector(fieldSystem->playerAvatar, &VStack_20);
    uVar1 = ov02_0224E31C(iVar5, iVar8);
    dest->areaIndex = uVar1;
    VStack_20.y = sub_02054774(fieldSystem, VStack_20.y, VStack_20.x, VStack_20.z, acStack_28);
    if (acStack_28[0] != 1) {
        return dest;
    }
    dest->unk1A = (VStack_20.y >> 0xc);
    iVar7 = GetDeltaXByFacingDirection(direction);
    dest->targets[0].x = iVar5 + iVar7;
    iVar7 = GetDeltaYByFacingDirection(direction);
    dest->targets[0].y = iVar8 + iVar7;
    dest->targets[0].valid = 1;
    iVar7 = ov02_0224E828(fieldSystem, iVar5, iVar8, direction, VStack_20.y, auStack_24, auStack_26, bVar9);
    if (iVar7 != 0) {
        dest->targets[1].x = auStack_24[0];
        dest->targets[1].y = auStack_26[0];
        dest->targets[1].valid = 1;
    }
    iVar7 = ov02_0224EB48(fieldSystem, iVar5, iVar8, direction, VStack_20.y, auStack_24, auStack_26, bVar9);
    if (iVar7 != 0) {
        dest->targets[2].x = auStack_24[0];
        dest->targets[2].y = auStack_26[0];
        dest->targets[2].valid = 1;
    }
    iVar7 = ov02_0224EE4C(fieldSystem, iVar5, iVar8, direction, VStack_20.y, auStack_24, auStack_26, bVar9);
    if (iVar7 != 0) {
        dest->targets[3].x = auStack_24[0];
        dest->targets[3].y = auStack_26[0];
        dest->targets[3].valid = 1;
    }
    return dest;
}
