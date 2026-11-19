/* PORTOK hk39 */
#include "global.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "pokemon_types_def.h"
#include "struct_defs/personal_narc_structs.h"
#include "wild_encounter.h"
#include "bg_window.h"
#include "error_handling.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
struct Background;
struct BaseStats;
struct BgConfig;
struct BoxPokemon;
struct MsgData;
struct NARC;
struct String;
struct Window;
struct _MAT;
struct _MAT_ENTRY;
extern const u8 ov112_021F5578[];
typedef struct { u16 a; u16 b; u16 c; } Ov112Sub; /* invented */
typedef struct { u32 unk0; u32 unk4; u8 unk8[0x78]; Ov112Sub unk80[10]; u8 unkBC[4]; } Ov112Entry; /* invented */
extern const Ov112Entry ov112_021F4138[];
typedef struct { u16 species; u8 unk2[0xE]; } Ov112Mon; /* invented */
typedef struct { u8 unk0[0x52]; Ov112Mon mons[3]; } Ov112Out; /* invented */
extern const u8 ov112_021F413C[];
extern void *const ov112_021FF528[];
extern const u8 ov112_021FE498[];
extern void ov112_021E8B74(u8 *a, u8 *b, u8 *c, u32 idx, u32 *out);
extern void ov112_021E9148(Window *windows, u8 *data, const u8 *a, u32 *out);
extern void ov112_021E93BC(void *pixels, int a, int b, void *buf, int c, int d, int e, int f);
void ov112_021E8C5C(s32 arg0, BoxPokemon *boxMon, u16 *arg2);
s32 ov112_021E9470(u32 species);
u32 ov112_021E9480(u32 species, s32 arg1, u16 form);
s32 ov112_021E9464(BoxPokemon *boxMon);
void ov112_021E9A30(s32 arg0, s32 arg1, s32 arg2);

void ov112_021E8CB8(u8 *out, u8 *data);
typedef struct BF_ov112_021E8CB8_0 { u8 f0_5:5; u8 f5_2:2; u8 b7:1; } BF_ov112_021E8CB8_0; /* invented: bitfield word (hk9) */
typedef struct BF_ov112_021E8CB8_1 { u8 b0:1; u8 b1:1; } BF_ov112_021E8CB8_1; /* invented: bitfield word (hk9) */
typedef struct BF_ov112_021E8CB8_2 { u8 pad0:5; u8 f5_2:2; } BF_ov112_021E8CB8_2; /* invented: bitfield word (hk9) */
void ov112_021E8CB8(u8 *out, u8 *data) {
    u32 i;
    int j;
    u32 idx;
    void *buf;
    BASE_STATS *personal;
    String *str;
    void *file;
    u32 entry;
    u32 tmp;
    Window windows[15];

    idx = (u8)*(u32 *)(data + 0x1D798);
    buf = Heap_Alloc((enum HeapID)0x9A, 0x180);
    for (i = 0; i < 15; i++) {
        AddWindowParameterized(*(BgConfig **)(data + 0x18), &windows[i], 1, 0, 0, ov112_021F5578[i], 2, 0, 0);
        FillWindowPixelBuffer(&windows[i], 0);
    }
    *(u16 *)out = *(u16 *)(data + 0x1D77C);
    AcquireBoxMonLock(*(BoxPokemon **)(data + 0x1E430));
    for (j = 0; j < 4; j++) {
        ((u16 *)out)[2 + j] = GetBoxMonData(*(BoxPokemon **)(data + 0x1E430), 0x36 + j, NULL);
    }
    out[0xC] = GetBoxMonData(*(BoxPokemon **)(data + 0x1E430), 0xA1, NULL);
    ((BF_ov112_021E8CB8_0 *)(&out[0xD]))->f0_5 = (u8)GetBoxMonData(*(BoxPokemon **)(data + 0x1E430), 0x70, NULL);
    ((BF_ov112_021E8CB8_0 *)(&out[0xD]))->b7 = 0;
    ((BF_ov112_021E8CB8_1 *)(&out[0xE]))->b1 = BoxMonIsShiny(*(BoxPokemon **)(data + 0x1E430));
    out[0x26] = GetBoxMonData(*(BoxPokemon **)(data + 0x1E430), 9, NULL);
    GetBoxMonData(*(BoxPokemon **)(data + 0x1E430), 0x75, out + 0x10);
    ReleaseBoxMonLock(*(BoxPokemon **)(data + 0x1E430), 1);
    personal = AllocAndLoadMonPersonal(*(u16 *)out, (enum HeapID)0x9A);
    ((BF_ov112_021E8CB8_1 *)(&out[0xE]))->b0 = (u8)GetPersonalAttr(personal, 0x1C);
    FreeMonPersonal(personal);
    ((BF_ov112_021E8CB8_0 *)(&out[0xD]))->f5_2 = GetBoxMonGender(*(BoxPokemon **)(data + 0x1E430));

    entry = *(u32 *)(ov112_021F413C + idx * 0xC0);
    out[0x27] = entry - 1;
    str = NewString_ReadMsgData(*(MsgData **)(data + 0x1E44C), *(u32 *)(data + 0x1D798) + 0x5B);
    CopyStringToU16Array(str, (u16 *)(out + 0x28), 0x15);
    String_Delete(str);
    ov112_021E8B74(out + 0x52, out + 0x82, out + 0x88, idx, &tmp);
    ov112_021E8C5C((s32)ov112_021F4138[idx].unkBC, *(BoxPokemon **)(data + 0x1E430), (u16 *)(out + 0x82));
    ov112_021E9148(windows, data, (const u8 *)&ov112_021F4138[idx], &tmp);

    for (j = 0; j < 3; j++) {
        file = GfGfxLoader_LoadFromNarc((NarcId)0xFA, ov112_021E9470(((Ov112Out *)out)->mons[j].species), 1, (enum HeapID)0x9A, 1);
        MI_CpuCopy8(file, out + 0xB7E + j * 0x180, 0x180);
        Heap_Free(file);
    }
    file = GfGfxLoader_LoadFromNarc((NarcId)0x102, ov112_021E9480(*(u16 *)(out + 0x72), ((BF_ov112_021E8CB8_2 *)(&out[0x7F]))->f5_2, 0), 1, (enum HeapID)0x9A, 1);
    MI_CpuCopy8(file, out + 0xFFE, 0x600);
    Heap_Free(file);

    for (j = 0; j < 3; j++) {
        ov112_021E93BC(windows[10 + j].pixelBuffer, 0xA, 2, buf, 0, 0, 0x50, 0x10);
        MI_CpuCopy8(buf, out + 0x15FE + j * 0x140, 0x140);
    }

    for (j = 0; j < 10; j++) {
        *(u16 *)(out + 0x8C + j * 2) = ov112_021F4138[idx].unk80[j].a;
        *(u16 *)(out + 0xA0 + j * 2) = ov112_021F4138[idx].unk80[j].b;
        out[0xB4 + j] = ov112_021F4138[idx].unk80[j].c;
    }
    MI_CpuCopy8(ov112_021FF528[entry], out + 0xBE, 0xC0);

    ov112_021E93BC(windows[13].pixelBuffer, 0xA, 2, buf, 0, 0, 0x50, 0x10);
    MI_CpuCopy8(buf, out + 0x17E, 0x140);

    file = GfGfxLoader_LoadFromNarc((NarcId)0xFA, ov112_021E9464(*(BoxPokemon **)(data + 0x1E430)), 1, (enum HeapID)0x9A, 1);
    MI_CpuCopy8(file, out + 0x2BE, 0x180);
    Heap_Free(file);

    if (*(u16 *)(data + 0x1D77C) == 0) {
        GF_AssertFail();
    }
    file = GfGfxLoader_LoadFromNarc((NarcId)0x102, ov112_021E9480(*(u16 *)(data + 0x1D77C), data[0x1D796], data[0x1D794]), 1, (enum HeapID)0x9A, 1);
    if (*(u16 *)(data + 0x1D77C) == 0x147) {
        ov112_021E9A30((s32)file, (s32)ov112_021FE498, (s32)GetBoxMonData(*(BoxPokemon **)(data + 0x1E430), 0, NULL));
    }
    MI_CpuCopy8(file, out + 0x43E, 0x600);
    Heap_Free(file);

    ov112_021E93BC(windows[14].pixelBuffer, 0xA, 2, buf, 0, 0, 0x50, 0x10);
    MI_CpuCopy8(buf, out + 0xA3E, 0x140);

    for (j = 0; j < 10; j++) {
        ov112_021E93BC(windows[j].pixelBuffer, 0xC, 2, buf, 0, 0, 0x60, 0x10);
        MI_CpuCopy8(buf, out + 0x19BE + j * 0x180, 0x180);
    }
    for (i = 0; i < 15; i++) {
        RemoveWindow(&windows[i]);
    }
    Heap_Free(buf);
}
