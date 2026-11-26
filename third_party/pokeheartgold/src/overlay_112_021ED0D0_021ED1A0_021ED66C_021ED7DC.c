#include "global.h"

#include "overlay_112_pokewalker_app.h"

typedef struct UnkStruct_ov112_021ED7DC_Bits {
    u16 val;
    u8 b : 5;
    u8 c : 2;
    u8 d : 1;
} UnkStruct_ov112_021ED7DC_Bits;

typedef struct UnkStruct_ov112_021ED7DC_Arg {
    void *ptr;
    u32 count;
    UnkStruct_ov112_021ED7DC_Bits bits;
} UnkStruct_ov112_021ED7DC_Arg;

#if !defined(SDK_PORT) && !defined(NONMATCHING)
// DS form only: a bit-field view of the 0x10-byte mon record (mwcc extracts the fields differently from the typed shifts)
typedef struct UnkStruct_ov112_021ED7DC_Src {
    u16 val;
    u8 unk2[11];
    u8 b : 5;
    u8 c : 2;
    u8 unkD_7 : 1;
    u8 unkE_0 : 1;
    u8 d : 1;
    u8 unkE_2 : 6;
} UnkStruct_ov112_021ED7DC_Src;
#endif

s32 ov112_021F30BC(void *, UnkStruct_ov112_021ED7DC_Arg *);

s32 ov112_021F3140(void *, UnkStruct_ov112_021ED7DC_Bits *);

s32 ov112_021F31BC(void *);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov112_021ED7DC(Ov112PokewalkerApp *arg0) {
    UnkStruct_ov112_021ED7DC_Bits bits;
    UnkStruct_ov112_021ED7DC_Arg arg = { 0 };

    u32 cur;
    u32 base;

    arg.ptr = arg0->unk_9D54;

    arg.bits.val = arg0->unk_9D44;
    arg.bits.b = (u32)(arg0->unk_9D51 << 0x1B) >> 0x1B;
    arg.bits.c = (u32)(arg0->unk_9D51 << 0x19) >> 0x1E;
    arg.bits.d = (u32)(arg0->unk_9D52[0] << 0x1E) >> 0x1F;
    cur = arg0->unk_10F0;
    base = arg0->unk_1D758;
    if (cur < base) {
        arg.count = 0;
    } else {
        arg.count = cur - base;
    }
    ov112_021F30BC(arg0->unk_AA34, &arg);
    if (ov112_021F31BC(arg0->unk_9DFC) != 0) {

        bits.val = arg0->unk_AD00.unk_0;
        bits.b = (u32)(arg0->unk_AD00.unk_D << 0x1B) >> 0x1B;
        bits.c = (u32)(arg0->unk_AD00.unk_D << 0x19) >> 0x1E;
        bits.d = (u32)(arg0->unk_AD00.unk_E[0] << 0x1E) >> 0x1F;
        ov112_021F3140(arg0->unk_9DFC, &bits);
    }
}
#else
void ov112_021ED7DC(Ov112PokewalkerApp *arg0_) {
    u8 *arg0 = (u8 *)arg0_;
    UnkStruct_ov112_021ED7DC_Bits bits;
    UnkStruct_ov112_021ED7DC_Arg arg = { 0 };

    u32 cur;
    u32 base;

    arg.ptr = arg0 + 0x9D54;

    arg.bits.val = ((UnkStruct_ov112_021ED7DC_Src *)(arg0 + 0x9D44))->val;
    arg.bits.b = ((UnkStruct_ov112_021ED7DC_Src *)(arg0 + 0x9D44))->b;
    arg.bits.c = ((UnkStruct_ov112_021ED7DC_Src *)(arg0 + 0x9D44))->c;
    arg.bits.d = ((UnkStruct_ov112_021ED7DC_Src *)(arg0 + 0x9D44))->d;
    cur = *(u32 *)(arg0 + 0x10F0);
    base = *(u32 *)(arg0 + 0x1D758);
    if (cur < base) {
        arg.count = 0;
    } else {
        arg.count = cur - base;
    }
    ov112_021F30BC(arg0 + 0xAA34, &arg);
    if (ov112_021F31BC(arg0 + 0x9DFC) != 0) {

        bits.val = ((UnkStruct_ov112_021ED7DC_Src *)(arg0 + 0xAD00))->val;
        bits.b = ((UnkStruct_ov112_021ED7DC_Src *)(arg0 + 0xAD00))->b;
        bits.c = ((UnkStruct_ov112_021ED7DC_Src *)(arg0 + 0xAD00))->c;
        bits.d = ((UnkStruct_ov112_021ED7DC_Src *)(arg0 + 0xAD00))->d;
        ov112_021F3140(arg0 + 0x9DFC, &bits);
    }
}
#endif
