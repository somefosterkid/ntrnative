#include "global.h"
struct GTSApplicationState;

#include "unk_0205B3DC.h"
#include "heap.h"
#include "overlay_70_gtsapplication_state.h"

extern const u16 ov70_02245CFC[7];

void ov70_022412C8(NNSG2dCharacterData *charData, NNSG2dPaletteData *plttData, int slotIndex, int appearance, int gender);

void ov70_02241308(struct GTSApplicationState *arg0);

void ov70_022412C8(NNSG2dCharacterData *charData, NNSG2dPaletteData *plttData, int slotIndex, int appearance, int gender) {
    int appearanceIdx;
    u8 *charRawData, *plttRawData;

    appearanceIdx = SpriteToUnionRoomAvatarIdx(gender, appearance);
    charRawData = (u8 *)charData->pRawData;
    plttRawData = (u8 *)plttData->pRawData;

    GXS_LoadOBJ(&charRawData[((4 * 4) * 3) * appearanceIdx * 0x20], ov70_02245CFC[slotIndex], (4 * 4) * 0x20);
    GXS_LoadOBJPltt(&plttRawData[appearanceIdx * 32], (slotIndex + 2) * 32, 32);
}

void ov70_02241308(GTSApplicationState *arg0) {
    if (arg0->hasAvatarFinishedMoving != 0) {
        Heap_Free(arg0->avatarPaletteHeapPtr);
        Heap_Free(arg0->avatarCharDataHeapPtr);
    }
}
