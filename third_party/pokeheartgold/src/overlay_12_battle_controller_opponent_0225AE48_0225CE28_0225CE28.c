#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_12_trainer_encounter_data.h"
#include "sys_task.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "sprite.h"

struct Background;

struct BattleSystem;

struct BgConfig;

struct ManagedSprite;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SysTask;

struct SysTaskQueue;

struct Terrain;

struct TrainerEncounterData;

typedef struct BattleSystem BattleSystem;

enum BgOffsetUpdateOp {
    BG_OFFSET_UPDATE_SET_X = 0,
    BG_OFFSET_UPDATE_ADD_X,
    BG_OFFSET_UPDATE_SUB_X,
    BG_OFFSET_UPDATE_SET_Y,
    BG_OFFSET_UPDATE_ADD_Y,
    BG_OFFSET_UPDATE_SUB_Y,
};

typedef struct Background {
    void *tilemapBuffer;
    u32 bufferSize;
    u32 baseTile;
    int hOffset;
    int vOffset;
    u8 mode;
    u8 size;
    u8 colorMode;
    u8 tileSize;
    u16 rotation;
    u16 unk22;
    fx32 xScale;
    fx32 yScale;
    int centerX;
    int centerY;
} Background;

typedef struct BgConfig {
    enum HeapID heapID;
    u16 scrollScheduled;
    u16 bufferTransferScheduled;
    struct Background bgs[8];
} BgConfig;

void BgSetPosTextAndCommit(BgConfig *bgConfig, u8 bgLayer, int bgOffsetUpdateOp, int val);

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

u32 ManagedSprite_GetNumFrames(ManagedSprite *managedSprite);

void ManagedSprite_SetAnim(ManagedSprite *managedSprite, u32 animID);

BOOL ManagedSprite_IsAnimated(ManagedSprite *managedSprite);

void ManagedSprite_SetAnimationFrame(ManagedSprite *managedSprite, u16 frame);

void ManagedSprite_SetPositionXY(ManagedSprite *managedSprite, s16 x, s16 y);

void ManagedSprite_GetPositionXY(ManagedSprite *managedSprite, s16 *outX, s16 *outY);

void ManagedSprite_OffsetPositionXY(ManagedSprite *managedSprite, s16 x, s16 y);


void SysTask_Destroy(SysTask *task);

void BattleController_EmitClearCommand(BattleSystem *battleSys, int battler, int command);

void Heap_Free(void *ptr);

typedef struct Terrain {
    ManagedSprite *managedSprite;
    BattleSystem *battleSys;
    u8 side;
    u8 terrainType;
    s16 unused_0A;
    s16 unused_0C;
} Terrain;

BgConfig *BattleSystem_GetBgConfig(BattleSystem *battleSys);

u32 BattleSystem_GetBattleType(BattleSystem *battleSys);

void ov12_0225CE28(SysTask *task, void *data);

void Pokepic_SetAttr(void *pokepic, int attr, int value);

void Pokepic_Delete(void *);

void ov12_0225CE28(SysTask *task, void *data) {
    TrainerEncounterData *trainerEncounterData = data;
    s16 x1, y1, x2, y2;
    u32 battleType = BattleSystem_GetBattleType(trainerEncounterData->battleSys);

    if (trainerEncounterData->enterFrameCount < 33 && trainerEncounterData->state >= 2
        && (trainerEncounterData->battlerType == 0 || trainerEncounterData->battlerType == 2)) {
        BgSetPosTextAndCommit(BattleSystem_GetBgConfig(trainerEncounterData->battleSys), GF_BG_LYR_MAIN_3, BG_OFFSET_UPDATE_ADD_X, 4);
        trainerEncounterData->enterFrameCount++;
    }

    switch (trainerEncounterData->state) {
    case 0:
        if (ManagedSprite_GetNumFrames(trainerEncounterData->managedSprite) > 2) {
            ManagedSprite_SetAnimationFrame(trainerEncounterData->managedSprite, 0);
            ManagedSprite_SetAnim(trainerEncounterData->managedSprite, 2);
        }

        trainerEncounterData->delay = 28;
        trainerEncounterData->state = 1;
    case 1:
        if (--trainerEncounterData->delay) {
            break;
        }

        trainerEncounterData->state = 2;
    case 2:
        ManagedSprite_GetPositionXY(trainerEncounterData->terrain->managedSprite, &x1, &y1);
        ManagedSprite_GetPositionXY(trainerEncounterData->managedSprite, &x2, &y2);

        if (trainerEncounterData->face == 2) {
            if (trainerEncounterData->battlerType == 1 || trainerEncounterData->battlerType == 3) {
                if (x1 < (24 * 8)) {
                    ManagedSprite_OffsetPositionXY(trainerEncounterData->terrain->managedSprite, 8, 0);
                } else {
                    ManagedSprite_SetPositionXY(trainerEncounterData->terrain->managedSprite, 24 * 8, 8 * 11);
                }
            }

            ManagedSprite_GetPositionXY(trainerEncounterData->terrain->managedSprite, &x1, &y1);

            if (trainerEncounterData->battlerType == 1
                || (trainerEncounterData->battlerType == 3 && (battleType == ((1 << 1) | (1 << 0)) || battleType == ((1 << 7) | ((1 << 1) | (1 << 0))) || battleType == (((1 << 2) | (1 << 0)) | (1 << 1))))) {
                ManagedSprite_SetPositionXY(trainerEncounterData->managedSprite, x1, y2);
            } else if (trainerEncounterData->battlerType == 3) {
                x1 = x2 - x1;
                x1 -= 24;

                ManagedSprite_OffsetPositionXY(trainerEncounterData->managedSprite, -x1, 0);
            } else if (trainerEncounterData->battlerType == 5) {
                x1 = x1 - x2;
                x1 -= 16;

                ManagedSprite_OffsetPositionXY(trainerEncounterData->managedSprite, x1, 0);
            }

            ManagedSprite_GetPositionXY(trainerEncounterData->managedSprite, &x2, &y2);

            if (x2 >= trainerEncounterData->targetX) {
                ManagedSprite_SetPositionXY(trainerEncounterData->managedSprite, trainerEncounterData->targetX, y2);

                if (trainerEncounterData->battlerType == 1 || trainerEncounterData->battlerType == 3) {
                    ManagedSprite_SetPositionXY(trainerEncounterData->terrain->managedSprite, 24 * 8, 8 * 11);
                }

                if (ManagedSprite_GetNumFrames(trainerEncounterData->managedSprite) > 1) {
                    ManagedSprite_SetAnimationFrame(trainerEncounterData->managedSprite, 0);
                    ManagedSprite_SetAnim(trainerEncounterData->managedSprite, 1);
                    trainerEncounterData->ballFlashStarted = 0;
                    trainerEncounterData->state = 3;
                } else {
                    trainerEncounterData->state = 4;
                }
            }
        } else {
            if (trainerEncounterData->battlerType == 0 || trainerEncounterData->battlerType == 2) {
                if (x1 > 64) {
                    ManagedSprite_OffsetPositionXY(trainerEncounterData->terrain->managedSprite, -8, 0);
                } else {
                    ManagedSprite_SetPositionXY(trainerEncounterData->terrain->managedSprite, 64, 128 + 8);
                }
            }

            ManagedSprite_GetPositionXY(trainerEncounterData->terrain->managedSprite, &x1, &y1);

            if (trainerEncounterData->battlerType == 0
                || (trainerEncounterData->battlerType == 2
                    && (battleType == ((1 << 1) | (1 << 0)) || battleType == ((1 << 7) | ((1 << 1) | (1 << 0))) || battleType == ((1 << 4) | ((1 << 1) | (1 << 0))) || battleType == ((1 << 2) | ((1 << 1) | (1 << 0)))))) {
                ManagedSprite_SetPositionXY(trainerEncounterData->managedSprite, x1, y2);
            } else if (trainerEncounterData->battlerType == 2) {
                x1 = x1 - x2;
                x1 -= 24;

                ManagedSprite_OffsetPositionXY(trainerEncounterData->managedSprite, x1, 0);
            } else if (trainerEncounterData->battlerType == 4) {
                x1 = x2 - x1;
                x1 -= 16;

                ManagedSprite_OffsetPositionXY(trainerEncounterData->managedSprite, -x1, 0);
            }

            ManagedSprite_GetPositionXY(trainerEncounterData->managedSprite, &x2, &y2);

            if (trainerEncounterData->pokepic != NULL) {
                Pokepic_SetAttr(trainerEncounterData->pokepic, 0, x2);
            }

            if (x2 <= trainerEncounterData->targetX) {
                if (trainerEncounterData->pokepic != NULL) {
                    Pokepic_Delete(trainerEncounterData->pokepic);
                }
                ManagedSprite_SetPositionXY(trainerEncounterData->managedSprite, trainerEncounterData->targetX, y2);

                if (trainerEncounterData->battlerType == 0 || trainerEncounterData->battlerType == 2) {
                    ManagedSprite_SetPositionXY(trainerEncounterData->terrain->managedSprite, 64, 128 + 8);
                }

                trainerEncounterData->state = 4;
            }
        }
        break;
    case 3:
        if (ManagedSprite_IsAnimated(trainerEncounterData->managedSprite) == 1) {
            if (ov12_02261DC8(trainerEncounterData, trainerEncounterData->managedSprite) == 0) {
                break;
            }
        }

        trainerEncounterData->state = 4;
    case 4:
        BattleController_EmitClearCommand(trainerEncounterData->battleSys, trainerEncounterData->battler, trainerEncounterData->command);
        Heap_Free(trainerEncounterData);
        SysTask_Destroy(task);
        break;
    }
}
