#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "filesystem.h"
#include "heap.h"
#include "sprite.h"
#include "overlay_45_unk_struct_ov66_02234798.h"
#include "overlay_87_scratch_off_card_app.h"

struct GF_2DGfxRawResMan;

struct GF_3DGfxRawResMan;

struct UnkStruct_ov63_0222BEC0_t;

struct UnkStruct_ov66_02234548_t;

struct UnkStruct_ov66_02234798_t;

typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void BillboardLists_Delete(void);

BOOL sub_02023874(BillboardList *list);

typedef struct GF_2DGfxRawResMan ResourceCollection;

typedef struct GF_3DGfxRawResMan TextureResourceManager;





typedef struct {
    Field3dModel unk_00;
    u32 unk_10;
} UnkStruct_ov66_02234958;

struct UnkStruct_ov66_02234548_t {
    ResourceCollection *unk_00;
    ResourceCollection *unk_04;
    TextureResourceManager *unk_08;
    BillboardList *unk_0C;
    UnkStruct_ov66_02234798 *unk_10;
    u32 unk_14;
    UnkStruct_ov66_02234958 unk_18;
    int unk_2C;
};

void ov45_02230A44(UnkStruct_ov66_02234958 *param0);

void ov45_02230638(UnkStruct_ov66_02234548 *param0);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov45_02230A4C(void *);
#else
s32 ov45_02230A4C(void *);
#endif
void ov45_02230700(u8 *arg0);

void ov45_02230638(UnkStruct_ov66_02234548 *param0) {
    {
        ov45_02230A44(&param0->unk_18);
        GF3dGfxRawResMan_FreeAllObjs(param0->unk_08);
        GF2dGfxRawResMan_FreeAllObjs(param0->unk_00);
        GF2dGfxRawResMan_FreeAllObjs(param0->unk_04);
    }

    {
        sub_02023874(param0->unk_0C);
        BillboardLists_Delete();
    }

    GF3dGfxRawResMan_Destroy(param0->unk_08);
    GF2dGfxRawResObj_Destroy(param0->unk_00);
    GF2dGfxRawResObj_Destroy(param0->unk_04);
    Heap_Free(param0->unk_10);
    Heap_Free(param0);
}

void ov45_02230ACC(UnkStruct_ov66_02234798 *param0);

void ov45_02230CB0(UnkStruct_ov66_02234798 *param0);

void ov45_02230DF4(UnkStruct_ov66_02234798 *param0);

void ov45_02230680(UnkStruct_ov66_02234548 *param0);

void ov45_02230680(UnkStruct_ov66_02234548 *param0) {
    int v0;

    for (v0 = 0; v0 < param0->unk_14; v0++) {
        ov45_02230ACC(&param0->unk_10[v0]);
        ov45_02230CB0(&param0->unk_10[v0]);
        ov45_02230DF4(&param0->unk_10[v0]);
    }
}

void BillboardLists_Draw(void);

u32 ov45_02230A58(const UnkStruct_ov66_02234958 *param0);

BOOL ov45_02230AA4(const UnkStruct_ov66_02234798 *param0);

void ov45_02230AC0(UnkStruct_ov66_02234798 *param0);

void ov45_022306B4(UnkStruct_ov66_02234548 *param0);

void ov45_022306B4(UnkStruct_ov66_02234548 *param0) {
    int v0;

    BillboardLists_Draw();

    if (ov45_02230A58(&param0->unk_18) > 0) {
        for (v0 = 0; v0 < param0->unk_14; v0++) {
            if (ov45_02230AA4(&param0->unk_10[v0]) == 1) {
                ov45_02230AC0(&param0->unk_10[v0]);
            }
        }
    }
}

struct CoordinatesS16;

struct Sprite;

void sub_02023910(Sprite *sprite, BOOL draw);

typedef struct BattleFactoryAppCursorSprite {
    u8 numSlots;
    u8 cursorMode;
    u8 currentSlot;
    u8 selectionActive;
    const CoordinatesS16 *positions;
    const u8 *animIDs;
    Sprite *sprite;
    u8 numBallSlots;
} BattleFactoryAppCursor;

void ov45_022306F4(BattleFactoryAppCursor *cursor, BOOL draw);

void ov45_022306F4(BattleFactoryAppCursor *cursor, BOOL draw) {
    sub_02023910(cursor->sprite, draw);
}

void ov45_02230700(u8 *arg0) {
    ov45_02230A4C(arg0 + 0x18);
}
