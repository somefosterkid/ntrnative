// Black's code at main 0x0200be84-0x0200c054. No name is known: the module and address.

#include "types.h"
#include "field/beacon_view.h"
#include "save/high_link.h"
#include "struct_decls.h"
#include "gfl/str.h"
#include "save/playtime.h"

static u64 data_02146170;

void func_0200be84(HighLinkSave *save, const void *data) {
    HighLinkEntry *entry = save->unk10;
    const HighLinkEntry *src = data;
    u16 terminator = GFL_StrBufGetTerminator();
    s32 i;

    for (i = 0; i < 8; i++) {
        if (terminator == src->words[i]) {
            break;
        }
    }
    if (i != 8 && src->unk12 < 0x12) {
        for (i = 0; i < 20; i++, entry++) {
            if (entry->unk10 == 0) {
                *entry = *src;
                return;
            }
        }
        for (i = 1; i < 20; i++) {
            save->unk10[i - 1] = save->unk10[i];
        }
        save->unk10[19] = *src;
    }
}

void func_0200bf08(HighLinkSave *save, u32 power) {
    save->powers[0] = power;
}

u32 func_0200bf10(HighLinkSave *save) {
    return save->powers[0];
}

void func_0200bf18(HighLinkSave *save, s32 param_2) {
    long idx;

    idx = param_2 - 0x21;
    if (idx >= 0 && param_2 <= 0x26) {
        save->powers[idx / 8 + 1] |= 1 << (idx % 8);
    }
}


void func_0200bf54(HighLinkSave *save, u8 *dest, u32 count) {
    s32 i;

    i = 0;
    do {
        dest[i] = save->powers[i + 1];
        i = i + 1;
    } while (i < 2);
}

u32 func_0200bf70(HighLinkSave *save) {
    return save->unk1A4;
}


u32 func_0200bf78(HighLinkSave *save) {
    if (save->unk1A4 < 99999) {
        save->unk1A4 = save->unk1A4 + 1;
    }
    return save->unk1A4;
}

u64 func_0200bf94(HighLinkSave *save) {
    return save->seconds;
}

u64 func_0200bfa4(HighLinkSave *save, PlayTime *playTime, u32 mode) {
    u64 hours = func_0200864c(playTime);
    u64 minutes = func_02008650(playTime);
    u64 seconds = func_02008654(playTime);
    u64 now = minutes * 60 + hours * 3600 + seconds;

    switch (mode) {
    case 0:
        data_02146170 = now;
        break;
    case 1:
        save->seconds += now - data_02146170;
        break;
    case 2:
        return save->seconds + (now - data_02146170);
    }
    return save->seconds;
}

