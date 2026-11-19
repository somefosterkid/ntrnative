#include "global.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

extern s8 *ov45_02254EF0;

void ov45_0222FE84(s32 unused1, s32 unused2, enum PPW_LOBBY_CHANNEL_KIND unused3, s32 unused4, s8 *param4, u8 *param5, u32 unused5, void *unused6);

s32 ov45_0222F7B0(s32);
s32 ov45_0222F928(void *, s32, void *);
void *ov45_0222F954(void *, s32);
s32 ov45_0222FA74(void *, s32, s32);
s32 ov45_0222FAF8(void *, s32, s32);
s32 ov45_0222FB24(void *, s32, s32);
void ov45_0222FEC4(s32 arg0, void *arg1);

void ov45_0222F88C(struct UnkStruct_ov66_022336C4 *, s32, const void *);
void ov45_0222FF40(s32 arg0, const u8 *arg1, u32 unused);

void ov45_0222FE84(s32 unused1, s32 unused2, enum PPW_LOBBY_CHANNEL_KIND unused3, s32 unused4, s8 *param4, u8 *param5, u32 unused5, void *unused6) {
    if (strcmp(param4, ov45_02254EF0) == 0) {
        memcpy(_022577C0->unk_134, param5, sizeof(_022577C0->unk_134));
        if (_022577C0->unk_130 != 0) {
            _022577C0->unk_130 = 0U;
        }
    }
}

void ov45_0222FEC4(s32 arg0, void *arg1) {
    s32 v0;
    s32 v2;
    s32 v1;

    if (ov45_0222FB24(_022577C0, arg0, 0) != -1) {
        if (_022577C0->unk_12D != 4) {
            v1 = *(s32 *)((u8 *)(ov45_0222F954(_022577C0, arg0)) + 8);
            v2 = *(s32 *)((u8 *)arg1 + 8);
            v0 = ov45_0222F7B0(_022577C0->unk_12D);
            if ((v1 == 5) && (v2 == v0)) {
                ov45_0222FA74(_022577C0, arg0, 1);
            } else if ((v1 == v0) && (v2 != v0)) {
                ov45_0222FAF8(_022577C0, arg0, 1);
            }
        }
        ov45_0222F928(_022577C0, arg0, arg1);
    }
}

void ov45_0222FF40(s32 arg0, const u8 *arg1, u32 unused) {
    if (ov45_0222FB24(_022577C0, arg0, 0) != -1) {
        ov45_0222F88C(_022577C0, arg0, arg1);
        _022577C0->unk_0C.unk_08(arg0, arg1, _022577C0->unk_20);
    }
}
