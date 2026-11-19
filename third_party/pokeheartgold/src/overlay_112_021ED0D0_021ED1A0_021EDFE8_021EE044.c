#include "global.h"

#include "text.h"
#include "unk_02005D10.h"
struct Ov112PokewalkerApp;

s32 ov112_021EAAE4(struct Ov112PokewalkerApp *, void *);

void *ov112_021ED610(void *);

s32 ov112_021ED640(struct Ov112PokewalkerApp *);

s32 ov112_021EDFE8(struct Ov112PokewalkerApp *);

typedef struct {
    u8 b0 : 2;
    u8 b2 : 1;
    u8 b3 : 5;
} UnkStruct_ov112_021EE044_10E7;

typedef struct {
    u8 b0 : 5;
    u8 b5 : 1;
    u8 b6 : 1;
    u8 b7 : 1;
} UnkStruct_ov112_021EE044_AABC;

s32 ov112_021EE044(void *arg0);

s32 ov112_021EE044(void *arg0) {
    u8 *base = arg0;
    ov112_021EAAE4(arg0, base + 0x1F2C0);
    if ((TextPrinterCheckActive((u8) * (s32 *)(base + 0x1E524)) == 0) && (IsFanfarePlaying() == 0)) {
        if (ov112_021ED610(arg0) == 0) {
            MIi_CpuClearFast(0U, base + 0x9D44, 0x10U);
            ((UnkStruct_ov112_021EE044_10E7 *)(base + 0x10E7))->b2 = 0;
        }
        if (ov112_021ED640(arg0) == 0) {
            MIi_CpuClearFast(0U, base + 0xAD00, 0x2FCU);
            ((UnkStruct_ov112_021EE044_AABC *)(base + 0xAABC))->b5 = 0;
        }
        if (ov112_021EDFE8(arg0) != 0 || ((UnkStruct_ov112_021EE044_AABC *)(base + 0xAABC))->b5 != 0 || ((UnkStruct_ov112_021EE044_AABC *)(base + 0xAABC))->b6 != 0 || ((UnkStruct_ov112_021EE044_10E7 *)(base + 0x10E7))->b2 != 0) {
            *(s32 *)(base + 4) = 6;
            return 0x42;
        }
        return 0x27;
    }
    return 0x26;
}
