#include "global.h"
#include "constants/heap.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct DressUpPhoto;

struct MailMessage;

struct NARC;

struct PhotoAccessory;

struct PhotoPokemon;

struct SysTask;

struct SysTaskQueue;

typedef struct PhotoAccessory {
    u8 unk_00;
    u8 xPos;
    u8 yPos;
    s8 priority;
} PhotoAccessory;

typedef struct PhotoPokemon {
    u32 personality;
    u32 otID;
    u16 species;
    u16 nickname[10 + 1];
    u16 trainerName[7 + 1];
    s8 priority;
    u8 xPos;
    u8 yPos;
    u8 form;
    u8 trainerGender;
} PhotoPokemon;

typedef struct DressUpPhoto {
    u32 integrity;
    PhotoPokemon photoMon;
    u32 unk_3C;
    MailMessage title;
    PhotoAccessory accessories[10];
    u8 unk_70;
    u8 language;
} DressUpPhoto;

typedef struct {
    u32 personality;
    u32 otID;
    u16 species;
    u16 trainerName[7 + 1];
    s8 priority;
    u8 xPos;
    u8 yPos;
    u8 form;
} UnkStruct_ov61_0222AE80_sub1;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    s8 unk_03;
} UnkStruct_ov61_0222AE80_sub2;

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov62_02239DA4_sub1;

typedef struct {
    u32 integrity;
    UnkStruct_ov61_0222AE80_sub1 unk_04;
    u32 unk_24;
    MailMessage title;
    UnkStruct_ov61_0222AE80_sub2 unk_30[10];
    u8 unk_58;
    u8 language;
    u8 unk_5A[2];
    UnkStruct_ov62_02239DA4_sub1 unk_5C;
} UnkStruct_ov61_0222AE80;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

int sub_0202B994(void);

enum CharCode {
    CHAR_NONE = 0,
    CHAR_WIDE_SPACE,

    CHAR_HIRAGANA_SMALL_A,
    CHAR_HIRAGANA_A,
    CHAR_HIRAGANA_SMALL_I,
    CHAR_HIRAGANA_I,
    CHAR_HIRAGANA_SMALL_U,
    CHAR_HIRAGANA_U,
    CHAR_HIRAGANA_SMALL_E,
    CHAR_HIRAGANA_E,
    CHAR_HIRAGANA_SMALL_O,
    CHAR_HIRAGANA_O,
    CHAR_HIRAGANA_KA,
    CHAR_HIRAGANA_GA,
    CHAR_HIRAGANA_KI,
    CHAR_HIRAGANA_GI,
    CHAR_HIRAGANA_KU,
    CHAR_HIRAGANA_GU,
    CHAR_HIRAGANA_KE,
    CHAR_HIRAGANA_GE,
    CHAR_HIRAGANA_KO,
    CHAR_HIRAGANA_GO,
    CHAR_HIRAGANA_SA,
    CHAR_HIRAGANA_ZA,
    CHAR_HIRAGANA_SHI,
    CHAR_HIRAGANA_JI,
    CHAR_HIRAGANA_SU,
    CHAR_HIRAGANA_ZU,
    CHAR_HIRAGANA_SE,
    CHAR_HIRAGANA_ZE,
    CHAR_HIRAGANA_SO,
    CHAR_HIRAGANA_ZO,
    CHAR_HIRAGANA_TA,
    CHAR_HIRAGANA_DA,
    CHAR_HIRAGANA_CHI,
    CHAR_HIRAGANA_DJI,
    CHAR_HIRAGANA_SOKUON,
    CHAR_HIRAGANA_TSU,
    CHAR_HIRAGANA_DZU,
    CHAR_HIRAGANA_TE,
    CHAR_HIRAGANA_DE,
    CHAR_HIRAGANA_TO,
    CHAR_HIRAGANA_DO,
    CHAR_HIRAGANA_NA,
    CHAR_HIRAGANA_NI,
    CHAR_HIRAGANA_NU,
    CHAR_HIRAGANA_NE,
    CHAR_HIRAGANA_NO,
    CHAR_HIRAGANA_HA,
    CHAR_HIRAGANA_BA,
    CHAR_HIRAGANA_PA,
    CHAR_HIRAGANA_HI,
    CHAR_HIRAGANA_BI,
    CHAR_HIRAGANA_PI,
    CHAR_HIRAGANA_FU,
    CHAR_HIRAGANA_BU,
    CHAR_HIRAGANA_PU,
    CHAR_HIRAGANA_HE,
    CHAR_HIRAGANA_BE,
    CHAR_HIRAGANA_PE,
    CHAR_HIRAGANA_HO,
    CHAR_HIRAGANA_BO,
    CHAR_HIRAGANA_PO,
    CHAR_HIRAGANA_MA,
    CHAR_HIRAGANA_MI,
    CHAR_HIRAGANA_MU,
    CHAR_HIRAGANA_ME,
    CHAR_HIRAGANA_MO,
    CHAR_HIRAGANA_SMALL_YA,
    CHAR_HIRAGANA_YA,
    CHAR_HIRAGANA_SMALL_YU,
    CHAR_HIRAGANA_YU,
    CHAR_HIRAGANA_SMALL_YO,
    CHAR_HIRAGANA_YO,
    CHAR_HIRAGANA_RA,
    CHAR_HIRAGANA_RI,
    CHAR_HIRAGANA_RU,
    CHAR_HIRAGANA_RE,
    CHAR_HIRAGANA_RO,
    CHAR_HIRAGANA_WA,
    CHAR_HIRAGANA_WO,
    CHAR_HIRAGANA_N,

    CHAR_KATAKANA_SMALL_A,
    CHAR_KATAKANA_A,
    CHAR_KATAKANA_SMALL_I,
    CHAR_KATAKANA_I,
    CHAR_KATAKANA_SMALL_U,
    CHAR_KATAKANA_U,
    CHAR_KATAKANA_SMALL_E,
    CHAR_KATAKANA_E,
    CHAR_KATAKANA_SMALL_O,
    CHAR_KATAKANA_O,
    CHAR_KATAKANA_KA,
    CHAR_KATAKANA_GA,
    CHAR_KATAKANA_KI,
    CHAR_KATAKANA_GI,
    CHAR_KATAKANA_KU,
    CHAR_KATAKANA_GU,
    CHAR_KATAKANA_KE,
    CHAR_KATAKANA_GE,
    CHAR_KATAKANA_KO,
    CHAR_KATAKANA_GO,
    CHAR_KATAKANA_SA,
    CHAR_KATAKANA_ZA,
    CHAR_KATAKANA_SHI,
    CHAR_KATAKANA_JI,
    CHAR_KATAKANA_SU,
    CHAR_KATAKANA_ZU,
    CHAR_KATAKANA_SE,
    CHAR_KATAKANA_ZE,
    CHAR_KATAKANA_SO,
    CHAR_KATAKANA_ZO,
    CHAR_KATAKANA_TA,
    CHAR_KATAKANA_DA,
    CHAR_KATAKANA_CHI,
    CHAR_KATAKANA_DJI,
    CHAR_KATAKANA_SOKUON,
    CHAR_KATAKANA_TSU,
    CHAR_KATAKANA_DZU,
    CHAR_KATAKANA_TE,
    CHAR_KATAKANA_DE,
    CHAR_KATAKANA_TO,
    CHAR_KATAKANA_DO,
    CHAR_KATAKANA_NA,
    CHAR_KATAKANA_NI,
    CHAR_KATAKANA_NU,
    CHAR_KATAKANA_NE,
    CHAR_KATAKANA_NO,
    CHAR_KATAKANA_HA,
    CHAR_KATAKANA_BA,
    CHAR_KATAKANA_PA,
    CHAR_KATAKANA_HI,
    CHAR_KATAKANA_BI,
    CHAR_KATAKANA_PI,
    CHAR_KATAKANA_FU,
    CHAR_KATAKANA_BU,
    CHAR_KATAKANA_PU,
    CHAR_KATAKANA_HE,
    CHAR_KATAKANA_BE,
    CHAR_KATAKANA_PE,
    CHAR_KATAKANA_HO,
    CHAR_KATAKANA_BO,
    CHAR_KATAKANA_PO,
    CHAR_KATAKANA_MA,
    CHAR_KATAKANA_MI,
    CHAR_KATAKANA_MU,
    CHAR_KATAKANA_ME,
    CHAR_KATAKANA_MO,
    CHAR_KATAKANA_SMALL_YA,
    CHAR_KATAKANA_YA,
    CHAR_KATAKANA_SMALL_YU,
    CHAR_KATAKANA_YU,
    CHAR_KATAKANA_SMALL_YO,
    CHAR_KATAKANA_YO,
    CHAR_KATAKANA_RA,
    CHAR_KATAKANA_RI,
    CHAR_KATAKANA_RU,
    CHAR_KATAKANA_RE,
    CHAR_KATAKANA_RO,
    CHAR_KATAKANA_WA,
    CHAR_KATAKANA_WO,
    CHAR_KATAKANA_N,

    CHAR_WIDE_0,
    CHAR_WIDE_1,
    CHAR_WIDE_2,
    CHAR_WIDE_3,
    CHAR_WIDE_4,
    CHAR_WIDE_5,
    CHAR_WIDE_6,
    CHAR_WIDE_7,
    CHAR_WIDE_8,
    CHAR_WIDE_9,
    CHAR_WIDE_A,
    CHAR_WIDE_B,
    CHAR_WIDE_C,
    CHAR_WIDE_D,
    CHAR_WIDE_E,
    CHAR_WIDE_F,
    CHAR_WIDE_G,
    CHAR_WIDE_H,
    CHAR_WIDE_I,
    CHAR_WIDE_J,
    CHAR_WIDE_K,
    CHAR_WIDE_L,
    CHAR_WIDE_M,
    CHAR_WIDE_N,
    CHAR_WIDE_O,
    CHAR_WIDE_P,
    CHAR_WIDE_Q,
    CHAR_WIDE_R,
    CHAR_WIDE_S,
    CHAR_WIDE_T,
    CHAR_WIDE_U,
    CHAR_WIDE_V,
    CHAR_WIDE_W,
    CHAR_WIDE_X,
    CHAR_WIDE_Y,
    CHAR_WIDE_Z,
    CHAR_WIDE_a,
    CHAR_WIDE_b,
    CHAR_WIDE_c,
    CHAR_WIDE_d,
    CHAR_WIDE_e,
    CHAR_WIDE_f,
    CHAR_WIDE_g,
    CHAR_WIDE_h,
    CHAR_WIDE_i,
    CHAR_WIDE_j,
    CHAR_WIDE_k,
    CHAR_WIDE_l,
    CHAR_WIDE_m,
    CHAR_WIDE_n,
    CHAR_WIDE_o,
    CHAR_WIDE_p,
    CHAR_WIDE_q,
    CHAR_WIDE_r,
    CHAR_WIDE_s,
    CHAR_WIDE_t,
    CHAR_WIDE_u,
    CHAR_WIDE_v,
    CHAR_WIDE_w,
    CHAR_WIDE_x,
    CHAR_WIDE_y,
    CHAR_WIDE_z,
    CHAR_WIDE_TRANSPARENT,
    CHAR_WIDE_EXCLAMATION,
    CHAR_WIDE_QUESTION,
    CHAR_JP_COMMA_pl,
    CHAR_JP_PERIOD,
    CHAR_JP_ELLIPSIS_pl,
    CHAR_JP_DOT,
    CHAR_WIDE_SLASH,
    CHAR_JP_SINGLE_QUOTE_OPEN,
    CHAR_JP_SINGLE_QUOTE_CLOSE,
    CHAR_JP_DOUBLE_QUOTE_OPEN,
    CHAR_JP_DOUBLE_QUOTE_CLOSE,
    CHAR_JP_PAREN_OPEN,
    CHAR_JP_PAREN_CLOSE,
    CHAR_WIDE_MALE,
    CHAR_WIDE_FEMALE,
    CHAR_WIDE_PLUS,
    CHAR_WIDE_MINUS,
    CHAR_WIDE_MULTIPLY,
    CHAR_WIDE_DIVIDE,
    CHAR_WIDE_EQUALS,
    CHAR_WIDE_TILDE,
    CHAR_WIDE_COLON,
    CHAR_WIDE_SEMICOLON,
    CHAR_WIDE_PERIOD,
    CHAR_WIDE_COMMA,
    CHAR_WIDE_SPADE,
    CHAR_WIDE_CLUB,
    CHAR_WIDE_HEART,
    CHAR_WIDE_DIAMOND,
    CHAR_WIDE_STAR,
    CHAR_WIDE_DOUBLE_CIRCLE,
    CHAR_WIDE_CIRCLE,
    CHAR_WIDE_SQUARE,
    CHAR_WIDE_TRIANGLE,
    CHAR_WIDE_DIAMOND_OPEN,
    CHAR_WIDE_AT_SIGN,
    CHAR_WIDE_EIGHTH_NOTE,
    CHAR_WIDE_PERCENT,
    CHAR_WIDE_SUN,
    CHAR_WIDE_CLOUD,
    CHAR_WIDE_UMBRELLA,
    CHAR_WIDE_SILHOUETTE,
    CHAR_WIDE_EMOTE_SMILE,
    CHAR_WIDE_EMOTE_LAUGH,
    CHAR_WIDE_EMOTE_UPSET,
    CHAR_WIDE_EMOTE_FROWN,
    CHAR_WIDE_ARROW_CURVE_UP,
    CHAR_WIDE_ARROW_CURVE_DOWN,
    CHAR_WIDE_ZZZ,
    CHAR_YEN,
    CHAR_POCKET_ICON_ITEMS,
    CHAR_POCKET_ICON_KEY_ITEMS,
    CHAR_POCKET_ICON_TMS,
    CHAR_POCKET_ICON_MAIL,
    CHAR_POCKET_ICON_MEDICINE,
    CHAR_POCKET_ICON_BERRIES,
    CHAR_POCKET_ICON_POKEBALLS,
    CHAR_POCKET_ICON_BATTLE_ITEMS,
    CHAR_ARROW_LEFT,
    CHAR_ARROW_UP,
    CHAR_ARROW_DOWN,
    CHAR_ARROW_RIGHT,
    CHAR_ARROW_MENU,
    CHAR_WIDE_AMPERSAND,

    CHAR_0_pl,
    CHAR_1_pl,
    CHAR_2_pl,
    CHAR_3_pl,
    CHAR_4_pl,
    CHAR_5_pl,
    CHAR_6_pl,
    CHAR_7_pl,
    CHAR_8_pl,
    CHAR_9_pl,
    CHAR_A_pl,
    CHAR_B_pl,
    CHAR_C_pl,
    CHAR_D_pl,
    CHAR_E_pl,
    CHAR_F_pl,
    CHAR_G_pl,
    CHAR_H_pl,
    CHAR_I_pl,
    CHAR_J_pl,
    CHAR_K_pl,
    CHAR_L_pl,
    CHAR_M_pl,
    CHAR_N_pl,
    CHAR_O_pl,
    CHAR_P_pl,
    CHAR_Q_pl,
    CHAR_R_pl,
    CHAR_S_pl,
    CHAR_T_pl,
    CHAR_U_pl,
    CHAR_V_pl,
    CHAR_W_pl,
    CHAR_X_pl,
    CHAR_Y_pl,
    CHAR_Z_pl,
    CHAR_a_pl,
    CHAR_b_pl,
    CHAR_c_pl,
    CHAR_d_pl,
    CHAR_e_pl,
    CHAR_f_pl,
    CHAR_g_pl,
    CHAR_h_pl,
    CHAR_i_pl,
    CHAR_j_pl,
    CHAR_k_pl,
    CHAR_l_pl,
    CHAR_m_pl,
    CHAR_n_pl,
    CHAR_o_pl,
    CHAR_p_pl,
    CHAR_q_pl,
    CHAR_r_pl,
    CHAR_s_pl,
    CHAR_t_pl,
    CHAR_u_pl,
    CHAR_v_pl,
    CHAR_w_pl,
    CHAR_x_pl,
    CHAR_y_pl,
    CHAR_z_pl,
    CHAR_A_GRAVE,
    CHAR_A_ACUTE,
    CHAR_A_CIRCUMFLEX,
    CHAR_A_TILDE,
    CHAR_A_DIERESIS,
    CHAR_A_OVERDOT,
    CHAR_AE_pl,
    CHAR_C_OGONEK,
    CHAR_E_GRAVE,
    CHAR_E_ACUTE,
    CHAR_E_CIRCUMFLEX,
    CHAR_E_DIERESIS,
    CHAR_I_GRAVE,
    CHAR_I_ACUTE,
    CHAR_I_CIRCUMFLEX,
    CHAR_I_DIERESIS,
    CHAR_D_BAR,
    CHAR_N_TILDE,
    CHAR_O_GRAVE,
    CHAR_O_ACUTE,
    CHAR_O_CIRCUMFLEX,
    CHAR_O_TILDE,
    CHAR_O_DIERSIS,
    CHAR_MULTIPLY,
    CHAR_NULL_SIGN,
    CHAR_U_GRAVE,
    CHAR_U_ACUTE,
    CHAR_U_CIRCUMFLEX,
    CHAR_U_DIERESIS,
    CHAR_Y_ACUTE,
    CHAR_UNK_P_LIKE_THING,
    CHAR_ESZETT_pl,
    CHAR_a_GRAVE,
    CHAR_a_ACUTE,
    CHAR_a_CIRCUMFLEX,
    CHAR_a_TILDE,
    CHAR_a_DIERESIS,
    CHAR_a_OVERDOT,
    CHAR_ae_pl,
    CHAR_c_OGONEK,
    CHAR_e_GRAVE,
    CHAR_e_ACUTE,
    CHAR_e_CIRCUMFLEX,
    CHAR_e_DIERESIS,
    CHAR_i_GRAVE,
    CHAR_i_ACUTE,
    CHAR_i_CIRCUMFLEX,
    CHAR_i_DIERESIS,
    CHAR_d_BAR,
    CHAR_n_TILDE,
    CHAR_o_GRAVE,
    CHAR_o_ACUTE,
    CHAR_o_CIRCUMFLEX,
    CHAR_o_TILDE,
    CHAR_o_DIERSIS,
    CHAR_DIVIDE,
    CHAR_null_SIGN,
    CHAR_u_GRAVE,
    CHAR_u_ACUTE,
    CHAR_u_CIRCUMFLEX,
    CHAR_u_DIERESIS,
    CHAR_y_ACUTE,
    CHAR_UNK_p_LIKE_THING,
    CHAR_y_dieresis,
    CHAR_OE_pl,
    CHAR_oe_pl,
    CHAR_S_OGONEK,
    CHAR_s_OGONEK,
    CHAR_a_SUPERSCRIPT,
    CHAR_o_SUPERSCRIPT,
    CHAR_er_SUPERSCRIPT,
    CHAR_re_SUPERSCRIPT,
    CHAR_r_SUPERSCRIPT,
    CHAR_POKEDOLLARS,
    CHAR_EXCLAMATION_INVERTED,
    CHAR_QUESTION_INVERTED,
    CHAR_EXCLAMATION,
    CHAR_QUESTION,
    CHAR_COMMA_pl,
    CHAR_PERIOD_pl,
    CHAR_ELLIPSIS_pl,
    CHAR_DOT,
    CHAR_SLASH_pl,
    CHAR_SINGLE_QUOTE_OPEN,
    CHAR_SINGLE_QUOTE_CLOSE,
    CHAR_DOUBLE_QUOTE_OPEN,
    CHAR_DOUBLE_QUOTE_CLOSE,
    CHAR_DOUBLE_QUOTE_CLOSE_INVERTED,
    CHAR_DOUBLE_PAREN_OPEN,
    CHAR_DOUBLE_PAREN_CLOSE,
    CHAR_PAREN_OPEN,
    CHAR_PAREN_CLOSE,
    CHAR_MALE_pl,
    CHAR_FEMALE_pl,
    CHAR_PLUS_pl,
    CHAR_MINUS,
    CHAR_ASTERISK_pl,
    CHAR_HASH_pl,
    CHAR_EQUALS_pl,
    CHAR_AMPERSAND_pl,
    CHAR_TILDE_pl,
    CHAR_COLON_pl,
    CHAR_SEMICOLON_pl,
    CHAR_SPADE,
    CHAR_CLUB,
    CHAR_HEART,
    CHAR_DIAMOND_pl,
    CHAR_STAR_pl,
    CHAR_DOUBLE_CIRCLE,
    CHAR_CIRCLE_pl,
    CHAR_SQUARE_pl,
    CHAR_TRIANGLE_pl,
    CHAR_DIAMOND_OPEN,
    CHAR_AT_SIGN,
    CHAR_EIGHT_NOTE,
    CHAR_PERCENT_pl,
    CHAR_SUN_pl,
    CHAR_CLOUD,
    CHAR_UMBRELLA_pl,
    CHAR_SILHOUETTE,
    CHAR_EMOTE_SMILE,
    CHAR_EMOTE_LAUGH,
    CHAR_EMOTE_UPSET,
    CHAR_EMOTE_FROWN,
    CHAR_ARROW_CURVE_UP,
    CHAR_ARROW_CURVE_DOWN,
    CHAR_ZZZ,
    CHAR_SPACE_pl,
    CHAR_e_SUPERSCRIPT,
    CHAR_PK_pl,
    CHAR_MN_pl,
    CHAR_NUM_SPACE,
    CHAR_SPACE_1px,
    CHAR_SPACE_2px,
    CHAR_SPACE_4px,
    CHAR_SPACE_8px,
    CHAR_SPACE_16px,
    CHAR_DEGREES,
    CHAR_UNDERSCORE_pl,
    CHAR_WIDE_UNDERSCORE,

    CHAR_CONTROL_SCREEN_INDICATOR = 0x0200,
    CHAR_CONTROL_PAUSE,
    CHAR_CONTROL_CALLBACK,
    CHAR_CONTROL_CURSOR_X,
    CHAR_CONTROL_CURSOR_Y,

    CHAR_CONTROL_CLEAR = 0x25BC,
    CHAR_CONTROL_SCROLL,

    CHAR_CR = 0xE000,

    CHAR_PLACEHOLDER_BEGIN = 0xF0FD,

    CHAR_COMPRESSED_MARK = 0xF100,

    CHAR_CONTROL_MOVE = 0xFE06,

    CHAR_CONTROL_SET_COLOR = 0xFF00,
    CHAR_CONTROL_SET_SIZE,

    CHAR_FORMAT_ARG = 0xFFFE,
    CHAR_EOS,
};

void Heap_Free(void *ptr);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

void ov39_02229224(const UnkStruct_ov61_0222AE80 *param0, DressUpPhoto *photo);

void ov39_02229224(const UnkStruct_ov61_0222AE80 *param0, DressUpPhoto *photo) {
    int v0;
    int i;

    v0 = sub_0202B994();
    MI_CpuClear8(photo, v0);

    photo->integrity = param0->integrity;

    photo->photoMon.personality = param0->unk_04.personality;
    photo->photoMon.otID = param0->unk_04.otID;
    photo->photoMon.species = param0->unk_04.species;

    for (i = 0; i < 7 + 1; i++) {
        photo->photoMon.trainerName[i] = param0->unk_04.trainerName[i];
    }

    photo->photoMon.priority = param0->unk_04.priority;
    photo->photoMon.xPos = param0->unk_04.xPos;
    photo->photoMon.yPos = param0->unk_04.yPos;
    photo->photoMon.form = param0->unk_04.form;

    photo->unk_3C = param0->unk_24;
    photo->title = *((MailMessage *)(&param0->title));

    for (i = 0; i < (11 - 1); i++) {
        photo->accessories[i] = *((PhotoAccessory *)(&param0->unk_30[i]));
    }

    photo->unk_70 = param0->unk_58;
    photo->language = param0->language;

    for (i = 0; i < 10 + 1; i++) {
        photo->photoMon.nickname[i] = CHAR_EOS;
    }

    photo->photoMon.trainerGender = 0;
}
