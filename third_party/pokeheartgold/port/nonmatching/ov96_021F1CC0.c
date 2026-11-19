#include "global.h"
#include "error_handling.h"
#include "fx_util.h"
#include "pokeathlon/pokeathlon.h"
struct OverlayManager;
struct OverlayManagerTemplate;
struct PlayerProfile;
struct PokeathlonCourseArgs;
struct PokeathlonCourseData;
struct PokeathlonCourseState;
struct PokeathlonFieldData;
struct PokeathlonParticipantData;
struct PokeathlonStateInfo;
struct PokeathlonUnkSubStruct_974;
extern VecFx32 ov96_0221BC64;
BOOL ov96_021F2F7C(void *obj, s32 a, s32 b);
void ov96_021E8228(PokeathlonCourseData *data, u8 index, u8 sub, u32 kind, u32 value);
BOOL ov96_021F218C(u8 *a, u8 *b);
void ov96_021F2AA4(u8 *entity);
BOOL ov96_021F22FC(u8 *alloc, u8 *entity, u8 index, VecFx32 *out);
void ov96_021F234C(u8 *alloc);

void ov96_021F1CC0(PokeathlonCourseData *param0);
void ov96_021F1CC0(PokeathlonCourseData *param0) {
    u8 *alloc = PokeathlonCourse_GetHeapAllocPtr4(param0);
    u8 *e;
    u8 *f;
    int j;
    int i;
    int flag;
    VecFx32 diff;
    VecFx32 dir;
    VecFx32 cur;
    VecFx32 forward;
    VecFx32 angleA;
    VecFx32 cross;
    float scale;
    float distF;
    float step;
    s32 dist;
    u32 angle;

    for (i = 0; i < 12; i++) {
        e = ((alloc + 0x20) +(( i) / 3) * 0x1b0 + (( i) % 3) * 0x90) ;
        if (e[0x46] != 0) {
            e[0x47] = (s8)e[0x47] - 1;
            if ((s8)e[0x47] <= 0) {
                e[0x46] = 0;
            }
        }
        if (*(s32 *)(e + 0x18) != 0) {
            if (ov96_021F2F7C(*(void **)e, *(s32 *)(e + 0x28), *(s32 *)(e + 0x2c)) != 0) {
                *(s32 *)(e + 0x18) = 0;
                e[0x45] = 0;
                e[0x44] = 0;
                ov96_021E8228(param0, (i / 3), (i % 3), 5, 1);
                ov96_021E8228(param0, (i / 3), (i % 3), 1, 1);
                f = alloc + i;
                if (f[0x7e0] != 0) {
                    ov96_021E8228(param0, (i / 3), (i % 3), 8, 1);
                }
                f[0x7e0] = 0;
            }
        }
    }
    for (i = 0; i < 12; i++) {
        e = ((alloc + 0x20) +(( i) / 3) * 0x1b0 + (( i) % 3) * 0x90) ;
        if (*(s32 *)(e + 0x18) == 2) {
            flag = 1;
            for (j = 0; j < 12; j++) {
                if (i == j) {
                    continue;
                }
                f = ((alloc + 0x20) +(( j) / 3) * 0x1b0 + (( j) % 3) * 0x90) ;
                if (*(s32 *)(f + 0x18) != 0) {
                    if (ov96_021F218C(e, f) != 0) {
                        flag = 0;
                        break;
                    }
                }
            }
            if (flag != 0) {
                *(s32 *)(e + 0x18) = 1;
            }
        }
    }
    forward = ov96_0221BC64;
    for (j = 0; j < 12; j++) {
        e = ((alloc + 0x20) +(( j) / 3) * 0x1b0 + (( j) % 3) * 0x90) ;
        if (*(s32 *)(e + 0x18) == 0) {
            continue;
        }
        e[0x41] = 0;
        *(s32 *)(e + 0x34) = 0;
        *(s32 *)(e + 0x38) = 0;
        *(s32 *)(e + 0x3c) = 0;
        cur = *(VecFx32 *)(e + 0x28);
        VEC_Subtract((VecFx32 *)(e + 0x1c), &cur, &diff);
        dist = VEC_Mag(&diff);
        if (dist <= 0) {
            ov96_021F2AA4(e);
            continue;
        }
        VEC_Normalize(&diff, &dir);
        if (e[0x44] != 0) {
            scale = 0.5f;
        } else {
            scale = (float)((double)(float)e[0x89] / 10.0);
        }
        distF = (float)dist;
        step = 4096.0f * scale;
        if (distF <= step) {
            cur = *(VecFx32 *)(e + 0x1c);
        } else {
            VecFx32 zero50 = {0, 0, 0};
            VEC_MultAdd((fx32)(4096.0f * scale), &dir, &zero50, &diff);
            VEC_Add(&diff, &cur, &cur);
        }
        if (dist != 0) {
            angleA = forward;
            cross.x = dir.y;
            cross.z = dir.x;
            cross.y = 0;
            angle = VEC_AngleBetween(&angleA, &cross);
            if (angle <= 0x2000 || angle >= 0xE000) {
                e[0x40] = 4;
            } else if (angle > 0x2000 && angle < 0x6000) {
                e[0x40] = 2;
            } else if (angle >= 0x6000 && angle <= 0xA000) {
                e[0x40] = 3;
            } else {
                e[0x40] = 1;
            }
        }
        if (*(s32 *)(e + 0x18) == 1) {
            if (ov96_021F22FC(alloc, e, j, &diff) == 0) {
                VecFx32 zero2C = {0, 0, 0};
                if (e[0x44] != 0) {
                    continue;
                }
                e[0x41] = 1;
                scale = (float)((double)(float)*(u16 *)(e + 0x8a) / 10.0);
                VEC_MultAdd((fx32)(4096.0f * (scale + (float)e[0x46])), &dir, &zero2C, (VecFx32 *)(e + 0x34));
                if (*(u16 *)(e + 0x8e) != 0) {
                    (*(u16 *)(e + 0x8e))--;
                } else {
                    GF_AssertFail();
                }
            } else {
                ov96_021F2AA4(e);
                *(VecFx32 *)(e + 0x28) = cur;
            }
        } else {
            *(u16 *)(e + 0x8e) = 0;
            *(VecFx32 *)(e + 0x28) = cur;
        }
    }
    ov96_021F234C(alloc);
}
