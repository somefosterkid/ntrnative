#include "global.h"

#include "unk_02095EAC.h"
#include "overlay_manager.h"

struct BgConfig;

struct Options;

struct SpriteResource;

struct String;

BOOL sub_02037030(int cmd, const void *data, int size);

u16 sub_0203769C(void);

BOOL IsPaletteFadeFinished(void);

typedef u32 TextColor;

u16 sub_02033250(void);

typedef struct {
    int (*unk_00)(UnkStruct_02095EAC *, int);
    int unk_04;
} UnkStruct_ov58_021D3180;

int ov58_021D1CF4(UnkStruct_02095EAC *param0, int param1);

int ov58_021D1DFC(UnkStruct_02095EAC *param0, int param1);

int ov37_021E69DC(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6B40(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6B64(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6BFC(UnkStruct_02095EAC *param0, int param1);

int ov58_021D20A8(UnkStruct_02095EAC *param0, int param1);

int ov58_021D20C8(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6C84(UnkStruct_02095EAC *param0, int param1);

int ov58_021D2130(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6D14(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6DD0(UnkStruct_02095EAC *param0, int param1);

int ov58_021D2270(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6E2C(UnkStruct_02095EAC *param0, int param1);

int ov58_021D22DC(UnkStruct_02095EAC *param0, int param1);

int ov58_021D22FC(UnkStruct_02095EAC *param0, int param1);

void ov37_021E7478(Window *param0, int param1, u32 param2, UnkStruct_02095EAC *param3);

int ov37_021E6EB4(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6F5C(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6F14(UnkStruct_02095EAC *param0, int param1);

int ov58_021D1DBC(UnkStruct_02095EAC *param0, int param1);

int ov37_021E68D0(UnkStruct_02095EAC *param0, int param1);

int ov37_021E694C(UnkStruct_02095EAC *param0, int param1);

int ov37_021E76F0(UnkStruct_02095EAC *param0);

int ov58_021D22D8(UnkStruct_02095EAC *param0, int param1);

void ov37_021E784C(UnkStruct_02095EAC *param0, int param1);

extern UnkStruct_ov58_021D3180 ov37_021E7D20[24];

u32 ov37_021E75C4(void);

void ov37_021E741C(u16 *arg0);

int ov37_021E5A84(OverlayManager *appMan, int *param1);

int ov37_021E5A84(OverlayManager *appMan, int *param1) {
    UnkStruct_02095EAC *v0 = OverlayManager_GetData(appMan);
    int new_var;
    if ((sub_0203769C() == 0) && (v0->unk_9418 != 0)) {
        v0->unk_9418 = v0->unk_9418 & sub_02033250();
    }
    v0->unk_364 = *param1;
    ov37_021E784C(v0, *param1);
    switch (*param1) {
    case 0:
        if (IsPaletteFadeFinished()) {
            if (sub_0203769C() != 0) {
                new_var = ov37_021E75C4();
                if (new_var >= 2) {
                    sub_02037030(128, 0L, 0);
                    *param1 = 1;
                }
                break;
            } else {
                *param1 = 1;
            }
        }
        break;

    case 1:
        if (ov37_021E7D20[v0->unk_368].unk_00 != 0L) {
            *param1 = ov37_021E7D20[v0->unk_368].unk_00(v0, *param1);
        }
        ov37_021E7478(v0->unk_2DC, 0, (TextColor)((((14 & 0xFF) << 16) | ((13 & 0xFF) << 8)) | ((15 & 0xFF) << 0)), v0);
        if (sub_0203769C() == 0) {
            int v1 = ov37_021E76F0(v0);
            if ((*param1) == 1) {
                *param1 = v1;
            }
        }
        ov37_021E741C(&v0->unk_43D8);
        break;

    case 2:
        if ((ov37_021E7D20[v0->unk_368].unk_00 != 0L) && ov37_021E7D20[v0->unk_368].unk_04) {
            *param1 = ov37_021E7D20[v0->unk_368].unk_00(v0, *param1);
        }
        break;

    case 3:
        if (IsPaletteFadeFinished()) {
            return 1;
        }
        break;
    }

    SpriteList_RenderAndAnimateSprites(v0->unk_34);
    return 0;
}
