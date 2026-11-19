#include "global.h"

#include "constants/seals.h"

#include "application/capsule_menu/capsule_menu.h"
#include "application/custom_ball/data/cb_data.naix"
#include "msgdata/msg/msg_0015.h" // seal names
#include "msgdata/msg/msg_0434.h" // seal descriptions; stripped

typedef struct {
    u8 memberIdx;
    u8 nameIdx;
    u8 particleIndex;
    u8 unused : 2;
    u8 nonAlphabetIndex : 6;
} SealTypeValues;

static SealTypeValues sSealTypeValues[81] __attribute__((section(BWPORT_DATA_SECTION))) = {
    [SEAL_NONE] = { NARC_cb_data_00039_NCGR_lz, msg_0015_00000, 0, 0, msg_0434_00000 },
    [SEAL_HEART_A] = { NARC_cb_data_00040_NCGR_lz, msg_0015_00001, 0, 0, msg_0434_00000 },
    [SEAL_HEART_B] = { NARC_cb_data_00041_NCGR_lz, msg_0015_00002, 0, 0, msg_0434_00001 },
    [SEAL_HEART_C] = { NARC_cb_data_00042_NCGR_lz, msg_0015_00003, 0, 0, msg_0434_00002 },
    [SEAL_HEART_D] = { NARC_cb_data_00043_NCGR_lz, msg_0015_00004, 0, 0, msg_0434_00003 },
    [SEAL_HEART_E] = { NARC_cb_data_00044_NCGR_lz, msg_0015_00005, 0, 0, msg_0434_00004 },
    [SEAL_HEART_F] = { NARC_cb_data_00045_NCGR_lz, msg_0015_00006, 0, 0, msg_0434_00005 },
    [SEAL_STAR_A] = { NARC_cb_data_00047_NCGR_lz, msg_0015_00007, 0, 0, msg_0434_00006 },
    [SEAL_STAR_B] = { NARC_cb_data_00046_NCGR_lz, msg_0015_00008, 0, 0, msg_0434_00007 },
    [SEAL_STAR_C] = { NARC_cb_data_00049_NCGR_lz, msg_0015_00009, 0, 0, msg_0434_00008 },
    [SEAL_STAR_D] = { NARC_cb_data_00048_NCGR_lz, msg_0015_00010, 0, 0, msg_0434_00009 },
    [SEAL_STAR_E] = { NARC_cb_data_00051_NCGR_lz, msg_0015_00011, 0, 0, msg_0434_00010 },
    [SEAL_STAR_F] = { NARC_cb_data_00050_NCGR_lz, msg_0015_00012, 0, 0, msg_0434_00011 },
    [SEAL_LINE_A] = { NARC_cb_data_00052_NCGR_lz, msg_0015_00013, 0, 0, msg_0434_00012 },
    [SEAL_LINE_B] = { NARC_cb_data_00053_NCGR_lz, msg_0015_00014, 0, 0, msg_0434_00013 },
    [SEAL_LINE_C] = { NARC_cb_data_00054_NCGR_lz, msg_0015_00015, 0, 0, msg_0434_00014 },
    [SEAL_LINE_D] = { NARC_cb_data_00055_NCGR_lz, msg_0015_00016, 0, 0, msg_0434_00015 },
    [SEAL_SMOKE_A] = { NARC_cb_data_00056_NCGR_lz, msg_0015_00017, 0, 0, msg_0434_00016 },
    [SEAL_SMOKE_B] = { NARC_cb_data_00057_NCGR_lz, msg_0015_00018, 0, 0, msg_0434_00017 },
    [SEAL_SMOKE_C] = { NARC_cb_data_00058_NCGR_lz, msg_0015_00019, 0, 0, msg_0434_00018 },
    [SEAL_SMOKE_D] = { NARC_cb_data_00059_NCGR_lz, msg_0015_00020, 0, 0, msg_0434_00019 },
    [SEAL_ELE_A] = { NARC_cb_data_00060_NCGR_lz, msg_0015_00021, 0, 0, msg_0434_00020 },
    [SEAL_ELE_B] = { NARC_cb_data_00061_NCGR_lz, msg_0015_00022, 0, 0, msg_0434_00021 },
    [SEAL_ELE_C] = { NARC_cb_data_00062_NCGR_lz, msg_0015_00023, 0, 0, msg_0434_00022 },
    [SEAL_ELE_D] = { NARC_cb_data_00063_NCGR_lz, msg_0015_00024, 0, 0, msg_0434_00023 },
    [SEAL_FOAMY_A] = { NARC_cb_data_00064_NCGR_lz, msg_0015_00025, 0, 0, msg_0434_00024 },
    [SEAL_FOAMY_B] = { NARC_cb_data_00065_NCGR_lz, msg_0015_00026, 0, 0, msg_0434_00025 },
    [SEAL_FOAMY_C] = { NARC_cb_data_00066_NCGR_lz, msg_0015_00027, 0, 0, msg_0434_00026 },
    [SEAL_FOAMY_D] = { NARC_cb_data_00067_NCGR_lz, msg_0015_00028, 0, 0, msg_0434_00027 },
    [SEAL_FIRE_A] = { NARC_cb_data_00068_NCGR_lz, msg_0015_00029, 0, 0, msg_0434_00028 },
    [SEAL_FIRE_B] = { NARC_cb_data_00069_NCGR_lz, msg_0015_00030, 0, 0, msg_0434_00029 },
    [SEAL_FIRE_C] = { NARC_cb_data_00070_NCGR_lz, msg_0015_00031, 0, 0, msg_0434_00030 },
    [SEAL_FIRE_D] = { NARC_cb_data_00071_NCGR_lz, msg_0015_00032, 0, 0, msg_0434_00031 },
    [SEAL_PARTY_A] = { NARC_cb_data_00072_NCGR_lz, msg_0015_00033, 0, 0, msg_0434_00032 },
    [SEAL_PARTY_B] = { NARC_cb_data_00073_NCGR_lz, msg_0015_00034, 0, 0, msg_0434_00033 },
    [SEAL_PARTY_C] = { NARC_cb_data_00074_NCGR_lz, msg_0015_00035, 0, 0, msg_0434_00034 },
    [SEAL_PARTY_D] = { NARC_cb_data_00075_NCGR_lz, msg_0015_00036, 0, 0, msg_0434_00035 },
    [SEAL_FLORA_A] = { NARC_cb_data_00076_NCGR_lz, msg_0015_00037, 0, 0, msg_0434_00036 },
    [SEAL_FLORA_B] = { NARC_cb_data_00077_NCGR_lz, msg_0015_00038, 0, 0, msg_0434_00037 },
    [SEAL_FLORA_C] = { NARC_cb_data_00078_NCGR_lz, msg_0015_00039, 0, 0, msg_0434_00038 },
    [SEAL_FLORA_D] = { NARC_cb_data_00079_NCGR_lz, msg_0015_00040, 0, 0, msg_0434_00039 },
    [SEAL_FLORA_E] = { NARC_cb_data_00080_NCGR_lz, msg_0015_00041, 0, 0, msg_0434_00040 },
    [SEAL_FLORA_F] = { NARC_cb_data_00081_NCGR_lz, msg_0015_00042, 0, 0, msg_0434_00041 },
    [SEAL_SONG_A] = { NARC_cb_data_00082_NCGR_lz, msg_0015_00043, 0, 0, msg_0434_00042 },
    [SEAL_SONG_B] = { NARC_cb_data_00083_NCGR_lz, msg_0015_00044, 0, 0, msg_0434_00043 },
    [SEAL_SONG_C] = { NARC_cb_data_00084_NCGR_lz, msg_0015_00045, 0, 0, msg_0434_00044 },
    [SEAL_SONG_D] = { NARC_cb_data_00085_NCGR_lz, msg_0015_00046, 0, 0, msg_0434_00045 },
    [SEAL_SONG_E] = { NARC_cb_data_00086_NCGR_lz, msg_0015_00047, 0, 0, msg_0434_00046 },
    [SEAL_SONG_F] = { NARC_cb_data_00087_NCGR_lz, msg_0015_00048, 0, 0, msg_0434_00047 },
    [SEAL_SONG_G] = { NARC_cb_data_00088_NCGR_lz, msg_0015_00049, 0, 0, msg_0434_00048 },
    [SEAL_A] = { NARC_cb_data_00089_NCGR_lz, msg_0015_00050, 0, 0, msg_0434_00000 },
    [SEAL_B] = { NARC_cb_data_00090_NCGR_lz, msg_0015_00051, 0, 0, msg_0434_00000 },
    [SEAL_C] = { NARC_cb_data_00091_NCGR_lz, msg_0015_00052, 0, 0, msg_0434_00000 },
    [SEAL_D] = { NARC_cb_data_00092_NCGR_lz, msg_0015_00053, 0, 0, msg_0434_00000 },
    [SEAL_E] = { NARC_cb_data_00093_NCGR_lz, msg_0015_00054, 0, 0, msg_0434_00000 },
    [SEAL_F] = { NARC_cb_data_00094_NCGR_lz, msg_0015_00055, 0, 0, msg_0434_00000 },
    [SEAL_G] = { NARC_cb_data_00095_NCGR_lz, msg_0015_00056, 0, 0, msg_0434_00000 },
    [SEAL_H] = { NARC_cb_data_00096_NCGR_lz, msg_0015_00057, 0, 0, msg_0434_00000 },
    [SEAL_I] = { NARC_cb_data_00097_NCGR_lz, msg_0015_00058, 0, 0, msg_0434_00000 },
    [SEAL_J] = { NARC_cb_data_00098_NCGR_lz, msg_0015_00059, 0, 0, msg_0434_00000 },
    [SEAL_K] = { NARC_cb_data_00099_NCGR_lz, msg_0015_00060, 0, 0, msg_0434_00000 },
    [SEAL_L] = { NARC_cb_data_00100_NCGR_lz, msg_0015_00061, 0, 0, msg_0434_00000 },
    [SEAL_M] = { NARC_cb_data_00101_NCGR_lz, msg_0015_00062, 0, 0, msg_0434_00000 },
    [SEAL_N] = { NARC_cb_data_00102_NCGR_lz, msg_0015_00063, 0, 0, msg_0434_00000 },
    [SEAL_O] = { NARC_cb_data_00103_NCGR_lz, msg_0015_00064, 0, 0, msg_0434_00000 },
    [SEAL_P] = { NARC_cb_data_00104_NCGR_lz, msg_0015_00065, 0, 0, msg_0434_00000 },
    [SEAL_Q] = { NARC_cb_data_00105_NCGR_lz, msg_0015_00066, 0, 0, msg_0434_00000 },
    [SEAL_R] = { NARC_cb_data_00106_NCGR_lz, msg_0015_00067, 0, 0, msg_0434_00000 },
    [SEAL_S] = { NARC_cb_data_00107_NCGR_lz, msg_0015_00068, 0, 0, msg_0434_00000 },
    [SEAL_T] = { NARC_cb_data_00108_NCGR_lz, msg_0015_00069, 0, 0, msg_0434_00000 },
    [SEAL_U] = { NARC_cb_data_00109_NCGR_lz, msg_0015_00070, 0, 0, msg_0434_00000 },
    [SEAL_V] = { NARC_cb_data_00110_NCGR_lz, msg_0015_00071, 0, 0, msg_0434_00000 },
    [SEAL_W] = { NARC_cb_data_00111_NCGR_lz, msg_0015_00072, 0, 0, msg_0434_00000 },
    [SEAL_X] = { NARC_cb_data_00112_NCGR_lz, msg_0015_00073, 0, 0, msg_0434_00000 },
    [SEAL_Y] = { NARC_cb_data_00113_NCGR_lz, msg_0015_00074, 0, 0, msg_0434_00000 },
    [SEAL_Z] = { NARC_cb_data_00114_NCGR_lz, msg_0015_00075, 0, 0, msg_0434_00000 },
    [SEAL_SHOCK] = { NARC_cb_data_00115_NCGR_lz, msg_0015_00076, 0, 0, msg_0434_00000 },
    [SEAL_MYSTERY] = { NARC_cb_data_00116_NCGR_lz, msg_0015_00077, 0, 0, msg_0434_00000 },
    [SEAL_LIQUID] = { NARC_cb_data_00117_NCGR_lz, msg_0015_00078, 0, 0, msg_0434_00049 },
    [SEAL_BURST] = { NARC_cb_data_00118_NCGR_lz, msg_0015_00079, 0, 0, msg_0434_00050 },
    [SEAL_TWINKLE] = { NARC_cb_data_00119_NCGR_lz, msg_0015_00080, 0, 0, msg_0434_00051 },
};

u8 CapsuleMenu_GetSealMemberIdx(u32 idx) {
    GF_ASSERT(idx < NELEMS(sSealTypeValues));
    return sSealTypeValues[idx].memberIdx;
}

u8 CapsuleMenu_GetSealNameIndex(u8 idx) {
    GF_ASSERT(idx < NELEMS(sSealTypeValues));
    return sSealTypeValues[idx].nameIdx;
}

u8 CapsuleMenu_GetSealParticleIndex(u32 idx) {
    GF_ASSERT(idx < NELEMS(sSealTypeValues));
    return sSealTypeValues[idx].particleIndex;
}

BOOL CapsuleMenu_IsAlphabetSeal(u32 idx) {
    GF_ASSERT(idx < NELEMS(sSealTypeValues));
    if (idx < SEAL_A || idx > SEAL_MYSTERY) {
        return FALSE;
    }

    return TRUE;
}

u32 CapsuleMenu_GetSealNonAlphabetIndex(u8 idx) {
    GF_ASSERT(idx < NELEMS(sSealTypeValues));
    return sSealTypeValues[idx].nonAlphabetIndex;
}

// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);
__attribute__((constructor)) static void hgasset_register_src_application_capsule_menu_main_static_c_0(void) {
    PORT_AssetAdd("IPKE", "src/application/capsule_menu/main_static.c:sSealTypeValues", (void *)&sSealTypeValues, sizeof(sSealTypeValues), "u8");
    PORT_AssetAdd("IPGE", "src/application/capsule_menu/main_static.c:sSealTypeValues", (void *)&sSealTypeValues, sizeof(sSealTypeValues), "u8");
}
