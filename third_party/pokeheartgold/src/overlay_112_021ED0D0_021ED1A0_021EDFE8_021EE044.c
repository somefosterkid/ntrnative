#include "global.h"

#include "text.h"
#include "unk_02005D10.h"
#include "overlay_112_pokewalker_app.h"
struct Ov112PokewalkerApp;

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

s32 ov112_021EE044(Ov112PokewalkerApp *arg0) {
    ov112_021EAAE4(arg0, arg0->unk_1F2C0);
    if ((TextPrinterCheckActive((u8)arg0->unk_1E524) == 0) && (IsFanfarePlaying() == 0)) {
        if (ov112_021ED610(arg0) == 0) {
            MIi_CpuClearFast(0U, &arg0->unk_9D44, 0x10U);
            ((UnkStruct_ov112_021EE044_10E7 *)&arg0->unk_10E7)->b2 = 0;
        }
        if (ov112_021ED640(arg0) == 0) {
            MIi_CpuClearFast(0U, &arg0->unk_AD00, 0x2FCU);
            ((UnkStruct_ov112_021EE044_AABC *)&arg0->unk_AABC)->b5 = 0;
        }
        if (ov112_021EDFE8(arg0) != 0 || ((UnkStruct_ov112_021EE044_AABC *)&arg0->unk_AABC)->b5 != 0 || ((UnkStruct_ov112_021EE044_AABC *)&arg0->unk_AABC)->b6 != 0 || ((UnkStruct_ov112_021EE044_10E7 *)&arg0->unk_10E7)->b2 != 0) {
            arg0->unk_4 = 6;
            return 0x42;
        }
        return 0x27;
    }
    return 0x26;
}
