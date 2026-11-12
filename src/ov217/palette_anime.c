// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov310/palette_anime.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/research_radar/palette_anime.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "system/palanm.h"

// How the colors move toward the animation's color, as a blend of 0 to 16
enum {
    // Pulses, with the period and the strongest blend
    MODE_PULSE_60_10,
    MODE_PULSE_30_3,
    MODE_PULSE_90_3,
    // Flashes every 4 frames, then stops and puts the colors back
    MODE_FLASH_20_6,
    MODE_FLASH_10_6,
    MODE_FLASH_30_6,
    MODE_FLASH_20_9,
    // Fades to 7 and stays
    MODE_FADE_IN,
    // Fades from 7, then stops and puts the colors back
    MODE_FADE_OUT,
    MODE_HOLD,
};

struct PaletteAnime {
    HeapID heapId;
    BOOL setUp;
    u16 *dst;
    u16 *backup;
    u8 count;
    u16 color;
    BOOL active;
    u32 mode;
    u32 frames;
};

static void func_ov217_021c2a18(PaletteAnime *anime, u32 mode, u16 color);
static void func_ov217_021c2a28(PaletteAnime *anime);
static void func_ov217_021c2a30(PaletteAnime *anime);
static void func_ov217_021c2aa0(PaletteAnime *anime);
static void func_ov217_021c2b10(PaletteAnime *anime);
static void func_ov217_021c2b80(PaletteAnime *anime);
static void func_ov217_021c2bf0(PaletteAnime *anime);
static void func_ov217_021c2c30(PaletteAnime *anime);
static void func_ov217_021c2c70(PaletteAnime *anime);
static void func_ov217_021c2cb0(PaletteAnime *anime);
static void func_ov217_021c2cf0(PaletteAnime *anime);
static void func_ov217_021c2d28(PaletteAnime *anime);
static void func_ov217_021c2d68(PaletteAnime *anime);
static void func_ov217_021c2d88(PaletteAnime *anime);
static BOOL func_ov217_021c2d9c(PaletteAnime *anime);
static PaletteAnime *func_ov217_021c2da0(HeapID heapId);
static void func_ov217_021c2dc4(PaletteAnime *anime);
static void func_ov217_021c2dcc(PaletteAnime *anime);
static void func_ov217_021c2de0(PaletteAnime *anime, u16 *dst, const u16 *src, u8 count);
static void func_ov217_021c2e20(PaletteAnime *anime);

// pokebw2's PaletteAnime_Create
PaletteAnime *func_ov217_021c29c8(HeapID heapId) {
    PaletteAnime *anime = func_ov217_021c2da0(heapId);

    func_ov217_021c2dcc(anime);
    return anime;
}

// pokebw2's PaletteAnime_Delete
void func_ov217_021c29d8(PaletteAnime *anime) {
    func_ov217_021c2e20(anime);
    func_ov217_021c2dc4(anime);
}

// pokebw2's PaletteAnime_Setup
void func_ov217_021c29e8(PaletteAnime *anime, u16 *dst, const u16 *src, u8 count) {
    func_ov217_021c2de0(anime, dst, src, count);
}

// pokebw2's PaletteAnime_Update
void func_ov217_021c29f0(PaletteAnime *anime) {
    func_ov217_021c2a30(anime);
}

// pokebw2's PaletteAnime_Start
void func_ov217_021c29f8(PaletteAnime *anime, u32 mode, u16 color) {
    func_ov217_021c2a18(anime, mode, color);
}

// pokebw2's PaletteAnime_Stop
void func_ov217_021c2a00(PaletteAnime *anime) {
    func_ov217_021c2a28(anime);
}

// pokebw2's PaletteAnime_Restore
void func_ov217_021c2a08(PaletteAnime *anime) {
    func_ov217_021c2d88(anime);
}

// pokebw2's PaletteAnime_IsActive
BOOL func_ov217_021c2a10(PaletteAnime *anime) {
    return func_ov217_021c2d9c(anime);
}

// pokebw2's PaletteAnime_StartCore
static void func_ov217_021c2a18(PaletteAnime *anime, u32 mode, u16 color) {
    anime->active = TRUE;
    anime->mode = mode;
    anime->frames = 0;
    anime->color = color;
}

// pokebw2's PaletteAnime_StopCore
static void func_ov217_021c2a28(PaletteAnime *anime) {
    anime->active = FALSE;
}

// pokebw2's PaletteAnime_UpdateCore
static void func_ov217_021c2a30(PaletteAnime *anime) {
    if (!anime->setUp || !anime->active) {
        return;
    }

    switch (anime->mode) {
    case MODE_PULSE_60_10:
        func_ov217_021c2aa0(anime);
        break;
    case MODE_PULSE_30_3:
        func_ov217_021c2b10(anime);
        break;
    case MODE_PULSE_90_3:
        func_ov217_021c2b80(anime);
        break;
    case MODE_FLASH_20_6:
        func_ov217_021c2bf0(anime);
        break;
    case MODE_FLASH_10_6:
        func_ov217_021c2c30(anime);
        break;
    case MODE_FLASH_30_6:
        func_ov217_021c2c70(anime);
        break;
    case MODE_FLASH_20_9:
        func_ov217_021c2cb0(anime);
        break;
    case MODE_FADE_IN:
        func_ov217_021c2cf0(anime);
        break;
    case MODE_FADE_OUT:
        func_ov217_021c2d28(anime);
        break;
    case MODE_HOLD:
        func_ov217_021c2d68(anime);
        break;
    }
}

// pokebw2's PaletteAnime_Pulse60
static void func_ov217_021c2aa0(PaletteAnime *anime) {
    u8 value = (int)(16.0f + 16.0f * FX_FX32_TO_F32(FX_SinIdx((u16)((anime->frames << 16) / 60))));
    value = value * 10 / 32;

    func_02021f64(anime->backup, anime->dst, anime->count, value, anime->color);
    anime->frames++;
}

// pokebw2's PaletteAnime_Pulse30
static void func_ov217_021c2b10(PaletteAnime *anime) {
    u8 value = (int)(16.0f + 16.0f * FX_FX32_TO_F32(FX_SinIdx((u16)((anime->frames << 16) / 30))));
    value = value * 3 / 32;

    func_02021f64(anime->backup, anime->dst, anime->count, value, anime->color);
    anime->frames++;
}

// pokebw2's PaletteAnime_Pulse90
static void func_ov217_021c2b80(PaletteAnime *anime) {
    u8 value = (int)(16.0f + 16.0f * FX_FX32_TO_F32(FX_SinIdx((u16)((anime->frames << 16) / 90))));
    value = value * 3 / 32;

    func_02021f64(anime->backup, anime->dst, anime->count, value, anime->color);
    anime->frames++;
}

// pokebw2's PaletteAnime_Flash20
static void func_ov217_021c2bf0(PaletteAnime *anime) {
    u8 evy;

    if (((anime->frames >> 2) & 1) == 0) {
        evy = 0;
    } else {
        evy = 6;
    }
    func_02021f64(anime->backup, anime->dst, anime->count, evy, anime->color);
    anime->frames++;
    if (anime->frames > 20) {
        func_ov217_021c2a28(anime);
        func_ov217_021c2d88(anime);
    }
}

// pokebw2's PaletteAnime_Flash10
static void func_ov217_021c2c30(PaletteAnime *anime) {
    u8 evy;

    if (((anime->frames >> 2) & 1) == 0) {
        evy = 0;
    } else {
        evy = 6;
    }
    func_02021f64(anime->backup, anime->dst, anime->count, evy, anime->color);
    anime->frames++;
    if (anime->frames > 10) {
        func_ov217_021c2a28(anime);
        func_ov217_021c2d88(anime);
    }
}

// pokebw2's PaletteAnime_Flash30
static void func_ov217_021c2c70(PaletteAnime *anime) {
    u8 evy;

    if (((anime->frames >> 2) & 1) == 0) {
        evy = 0;
    } else {
        evy = 6;
    }
    func_02021f64(anime->backup, anime->dst, anime->count, evy, anime->color);
    anime->frames++;
    if (anime->frames > 30) {
        func_ov217_021c2a28(anime);
        func_ov217_021c2d88(anime);
    }
}

// pokebw2's PaletteAnime_FlashStrong20
static void func_ov217_021c2cb0(PaletteAnime *anime) {
    u8 evy;

    if (((anime->frames >> 2) & 1) == 0) {
        evy = 0;
    } else {
        evy = 9;
    }
    func_02021f64(anime->backup, anime->dst, anime->count, evy, anime->color);
    anime->frames++;
    if (anime->frames > 20) {
        func_ov217_021c2a28(anime);
        func_ov217_021c2d88(anime);
    }
}

// pokebw2's PaletteAnime_FadeIn
static void func_ov217_021c2cf0(PaletteAnime *anime) {
    u8 evy;

    if (anime->frames < 20) {
        evy = anime->frames * 7 / 20;
    } else {
        evy = 7;
    }
    func_02021f64(anime->backup, anime->dst, anime->count, evy, anime->color);
    anime->frames++;
}

// pokebw2's PaletteAnime_FadeOut
static void func_ov217_021c2d28(PaletteAnime *anime) {
    func_02021f64(anime->backup, anime->dst, anime->count, 7 - anime->frames * 7 / 30, anime->color);
    anime->frames++;
    if (anime->frames > 30) {
        func_ov217_021c2a28(anime);
        func_ov217_021c2d88(anime);
    }
}

// pokebw2's PaletteAnime_Hold
static void func_ov217_021c2d68(PaletteAnime *anime) {
    func_02021f64(anime->backup, anime->dst, anime->count, 7, anime->color);
    anime->frames++;
}

// pokebw2's PaletteAnime_RestoreCore
static void func_ov217_021c2d88(PaletteAnime *anime) {
    MI_CpuCopy8(anime->backup, anime->dst, anime->count * sizeof(u16));
}

// pokebw2's PaletteAnime_IsActiveCore
static BOOL func_ov217_021c2d9c(PaletteAnime *anime) {
    return anime->active;
}

// pokebw2's PaletteAnime_Alloc
static PaletteAnime *func_ov217_021c2da0(HeapID heapId) {
    PaletteAnime *anime = GFL_HeapAllocate(heapId, sizeof(PaletteAnime), FALSE, "palette_anime.c", 667);

    anime->heapId = heapId;
    return anime;
}

// pokebw2's PaletteAnime_Free
static void func_ov217_021c2dc4(PaletteAnime *anime) {
    GFL_HeapFree(anime);
}

// pokebw2's PaletteAnime_InitWork
static void func_ov217_021c2dcc(PaletteAnime *anime) {
    HeapID heapId = anime->heapId;

    MI_CpuFill8(anime, 0, sizeof(PaletteAnime));
    anime->heapId = heapId;
}

// pokebw2's PaletteAnime_SetupCore
static void func_ov217_021c2de0(PaletteAnime *anime, u16 *dst, const u16 *src, u8 count) {
    int i;

    anime->setUp = TRUE;
    anime->dst = dst;
    anime->count = count;
    anime->backup = GFL_HeapAllocate(anime->heapId, count * sizeof(u16), FALSE, "palette_anime.c", 728);
    for (i = 0; i < count; i++) {
        anime->backup[i] = src[i];
    }
}

// pokebw2's PaletteAnime_Release
static void func_ov217_021c2e20(PaletteAnime *anime) {
    GFL_HeapFree(anime->backup);
    anime->setUp = FALSE;
}
