/* PORTOK hk39 */
#include "global.h"
#include "unk_02054648.h"
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov70_0225C894_t;
struct UnkStruct_ov70_0225DEE8_t;
struct UnkStruct_ov70_02261E10_t;
struct UnkStruct_ov70_0226CF38_t;
typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;
typedef struct UnkStruct_ov70_0226CF38_t UnkStruct_ov70_0226CF38;
typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;
typedef struct UnkStruct_ov66_0222DFF8_t WiFiPlazaSystem;
typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;
typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;
void ov70_022622EC(UnkStruct_ov70_02261E10 *param0, u8 param1, const VecFx32 *param2);
void ov70_022623F0(UnkStruct_ov70_02261E10 *param0, u32 param1, u16 param2);
typedef struct {
    u16 unk_00;
    u16 unk_02;
    union {
        struct {
            s16 unk_00;
            s16 unk_02;
            u16 unk_04;
            s16 unk_06;
            s16 unk_08;
        } val1;
    } unk_04;
} UnkStruct_ov70_0226D2A0;
struct UnkStruct_ov70_0226CF38_t {
    UnkStruct_ov70_0225DEE8 *unk_00;
    WiFiPlazaSystem *unk_04;
    UnkStruct_ov70_02261E10 *unk_08;
    UnkStruct_ov70_0225C894 *unk_0C;
    u8 unk_10[9][3];
    u8 unk_2B;
    UnkStruct_ov70_0226D2A0 unk_2C[9];
    UnkStruct_ov63_0222CC3C unk_AA[3];
    u16 unk_B6;
    u16 unk_B8;
    fx32 unk_BC;
    fx32 unk_C0;
};
UnkStruct_ov70_0226CF38 *ov49_02268FAC(UnkStruct_ov70_0225DEE8 *param0, enum HeapID heapID);
BOOL ov02_0224EF6C(u8 arg0, s32 arg1, s32 arg2);
s32 sub_02054774(FieldSystem *fieldSystem, s32 arg1, s32 arg2, s32 arg3, u8 *arg4);
extern u8 ov02_02253A4C[];

BOOL ov02_0224E828(FieldSystem *fieldSystem, s32 x, s32 z, int mode, s32 arg5, u16 *outX, u16 *outZ, u8 arg8);
BOOL ov02_0224E828(FieldSystem *fieldSystem, s32 x, s32 z, int mode, s32 arg5, u16 *outX, u16 *outZ, u8 arg8)
{
    u8 buf[4];
    u8 cnt;
    u8 idx;
    u8 h1;
    BOOL flag;
    s32 k;
    s32 off;
    s32 dz;
    s32 zz;
    s32 ret;
    s32 t;
    s32 ix;

    float fz;
    float fx;

    buf[1] = ov02_02253A4C[2];
    buf[2] = ov02_02253A4C[3];
    *outX = x;
    *outZ = z;

    switch (mode) {
    case 0:
    case 1:
        cnt = 0;
        k = 0;
        idx = (mode != 0) ? 1 : cnt;
        dz = (s8)buf[1 + idx];
        off = k;
        for (; k < 2; k++) {
            zz = (s16)(z + (off + dz));
            h1 = GetMetatileBehavior(fieldSystem, x, zz);
            flag = sub_020548C0(fieldSystem, x, zz);
            if (ov02_0224EF6C(h1, flag, arg8)) {
                t = zz * 16 + 8;
                if (t > 0) {
                    fz = (float)(t << 12) + 0.5f;
                } else {
                    fz = (float)(t << 12) - 0.5f;
                }
                t = x * 16 + 8;
                if (t > 0) {
                    fx = (float)(t << 12) + 0.5f;
                } else {
                    fx = (float)(t << 12) - 0.5f;
                }
                ix = (s32)fx;
                ret = sub_02054774(fieldSystem, arg5, ix, (s32)fz, buf);
                if (buf[0] == 1 && ret == arg5) {
                    cnt++;
                }
            }
            off += dz;
        }
        if (cnt == 2) {
            *outX = x;
            if (mode == 0) {
                *outZ = z - 1;
            } else {
                *outZ = z + 2;
            }
            return 1;
        }
        break;
    case 2:
    case 3:
        x = (s16)(x + (s8)buf[(mode == 2) ? 1 : 2]);
        h1 = GetMetatileBehavior(fieldSystem, x, z);
        flag = sub_020548C0(fieldSystem, x, z);
        t = z * 16 + 8;
        if (t > 0) {
            fz = (float)(t << 12) + 0.5f;
        } else {
            fz = (float)(t << 12) - 0.5f;
        }
        t = x * 16 + 8;
        if (t > 0) {
            fx = (float)(t << 12) + 0.5f;
        } else {
            fx = (float)(t << 12) - 0.5f;
        }
        ix = (s32)fx;
        ret = sub_02054774(fieldSystem, arg5, ix, (s32)fz, buf);
        if (ov02_0224EF6C(h1, flag, arg8) && buf[0] == 1 && ret == arg5) {
            s8 *p = (s8 *)&buf[1];
            for (k = 0; k < 2; k++, p++) {
                zz = (s16)(z + *p);
                h1 = GetMetatileBehavior(fieldSystem, x, zz);
                flag = sub_020548C0(fieldSystem, x, zz);
                t = zz * 16 + 8;
                if (t > 0) {
                    fz = (float)(t << 12) + 0.5f;
                } else {
                    fz = (float)(t << 12) - 0.5f;
                }
                t = x * 16 + 8;
                if (t > 0) {
                    fx = (float)(t << 12) + 0.5f;
                } else {
                    fx = (float)(t << 12) - 0.5f;
                }
                ix = (s32)fx;
                ret = sub_02054774(fieldSystem, arg5, ix, (s32)fz, buf);
                if (ov02_0224EF6C(h1, flag, arg8) && buf[0] == 1 && ret == arg5) {
                    *outX = x;
                    *outZ = z + k;
                    return 1;
                }
            }
        }
        break;
    default:
        break;
    }
    return 0;
}
