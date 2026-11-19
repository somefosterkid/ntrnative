#include "global.h"

#include "constants/heap.h"

#include "save.h"
#include "screen_fade.h"
#include "unk_02033AE0.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "player_data.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct OverlayManager;

struct PlayerProfile;

struct UnkStruct_ov114_0225D678_t;

struct UnkStruct_ov114_0225E854_t;

struct UnkStruct_ov66_0222DFF8_t;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

void *OverlayManager_GetData(OverlayManager *appMan);

typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;

typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;

UnkStruct_ov114_0225D678 *ov90_0225892C(const UnkStruct_ov114_0225C76C *param0, u32 heapID);

void ov90_02258938(UnkStruct_ov114_0225D678 *param0);

BOOL ov90_022589BC(const UnkStruct_ov114_0225D678 *param0);

int ov90_022589CC(const UnkStruct_ov114_0225D678 *param0);

UnkStruct_ov114_0225E854 *ov90_022589F8(const UnkStruct_ov114_0225C76C *param0, const UnkStruct_ov114_0225C9A8 *param1, u32 heapID);

void ov90_02258A04(UnkStruct_ov114_0225E854 *param0);

BOOL ov90_02258AA0(const UnkStruct_ov114_0225E854 *param0);

BOOL ov90_02258AA4(const UnkStruct_ov114_0225E854 *param0);

BOOL ov90_02258B98(const UnkStruct_ov114_0225C76C *param0);

void sub_0200FC20(u16 color);

typedef struct {
    UnkStruct_ov117_022605C0 *unk_00;
    UnkStruct_ov114_0225D678 *unk_04;
    UnkStruct_ov114_0225E854 *unk_08;
} UnkStruct_ov117_02266498;

int ov93_022625BC(OverlayManager *appMan, int *param1);

void OverlayManager_FreeData(OverlayManager *appMan);
int ov93_022626E8(OverlayManager *appMan, int *param1);
extern u32 ov93_022630E4;
void ov93_022626FC(void *param_1);

int ov93_022625BC(OverlayManager *appMan, int *param1) {
    UnkStruct_ov117_02266498 *v0 = OverlayManager_GetData(appMan);

    if (v0->unk_00->unk_3D == 1) {
        switch (v0->unk_00->unk_3E) {
        case 0:
            if (IsPaletteFadeFinished() == 1) {
                sub_0200FB70();
            }

            sub_0200FC20(((0) << 10) | ((0) << 5) | (0));
            v0->unk_00->unk_3E++;
            break;
        case 1:
            if (ov90_02258B98(&v0->unk_00->unk_00) == 1) {
                v0->unk_00->unk_3E++;
            }
            break;
        default:
        case 2:
            if (v0->unk_04 != 0L) {
                ov90_02258938(v0->unk_04);
                v0->unk_04 = 0L;
            }

            if (v0->unk_08 != 0L) {
                ov90_02258A04(v0->unk_08);
                v0->unk_08 = 0L;
            }

            return 1;
        }

        return 0;
    }

    switch (*param1) {
    case 0:
        if (v0->unk_00->unk_31 == 0) {
            *param1 = 1;
        } else {
            *param1 = 3;
        }
        break;
    case 1:
        v0->unk_04 = ov90_0225892C(&v0->unk_00->unk_00, HEAP_ID_117);
        (*param1)++;
        break;
    case 2:
        if (ov90_022589BC(v0->unk_04) == 1) {
            v0->unk_00->unk_34 = ov90_022589CC(v0->unk_04);
            ov90_02258938(v0->unk_04);
            v0->unk_04 = 0L;
            (*param1) = 5;
        }
        break;
    case 3:
        v0->unk_08 = ov90_022589F8(&v0->unk_00->unk_00, &v0->unk_00->unk_10, HEAP_ID_117);
        (*param1)++;
        break;
    case 4:
        if (ov90_02258AA0(v0->unk_08) == 1) {
            v0->unk_00->unk_38 = ov90_02258AA4(v0->unk_08);
            ov90_02258A04(v0->unk_08);
            v0->unk_08 = 0L;
            (*param1) = 5;
        }

        break;
    case 5:
    default:
        return 1;
    }

    return 0;
}

int ov93_022626E8(OverlayManager *appMan, int *param1) {
    UnkStruct_ov117_02266498 *v0 = OverlayManager_GetData(appMan);
    OverlayManager_FreeData(appMan);

    return 1;
}

void ov93_022626FC(void *param_1) {
    sub_0203410C(&ov93_022630E4, 4, param_1);
    return;
}
