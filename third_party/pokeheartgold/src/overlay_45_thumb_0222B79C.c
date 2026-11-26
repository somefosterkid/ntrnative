#include "global.h"

#include "constants/heap.h"

#include "easy_chat.h"
#include "save.h"
#include "unk_struct_ov66_02232B20.h"
#include "overlay_45_wifi_plaza_system.h"

typedef u16 charcode_t;

u32 ov45_0222AA84(const WiFiPlazaProfile *profile);

/* DS: the callers were built against the wider return type below; the PC takes the definition's (abilint RET) */
#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov45_0222AAA8(const WiFiPlazaProfile *profile);
#else
u32 ov45_0222AAA8(const WiFiPlazaProfile *profile);
#endif

void ov45_0222D638(UnkStruct_ov66_02231560 *param0, u16 param1, u8 param2, BOOL param3);


const void *ov45_0222EA2C(s32 param0);

void ov45_0222EC10(UnkStruct_ov66_02232B20 *param0);

u16 ov45_0222EDC4(u32 param0);

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_45_thumb_0222E810_0222E9F8_0222EDA8.c (tools/pokehg_abi.py)
u8 ov45_0222EDF0(u32 param0);
#else
u8 ov45_0222EDF0(u8 param0);
#endif

void ov45_0222B79C(UnkStruct_ov66_0222F6C4 *param0, UnkStruct_ov66_02231560 *param1);

struct PasswordWordBank;
struct PlayerProfile;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef enum {
    UnkEnum_ov66_0223287C_00,
    UnkEnum_ov66_0223287C_01,
    UnkEnum_ov66_0223287C_02,
    UnkEnum_ov66_0223287C_03,
    UnkEnum_ov66_0223287C_04
} UnkEnum_ov66_0223287C;
u32 ov45_0222AD4C(const WiFiPlazaSystem *param0);
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
u8 ov45_0222E96C(void);
typedef struct PasswordWordBank PasswordWordBank;
void ov45_0222B840(WiFiPlazaSystem *param0);

void ov45_0222B79C(UnkStruct_ov66_0222F6C4 *param0, UnkStruct_ov66_02231560 *param1) {
    int v0;
    UnkStruct_ov66_02232B20 v1;
    const WiFiPlazaProfile *v2;
    u8 v4;
    u8 v5;

    memset(param1, 0, sizeof(UnkStruct_ov66_02231560));

    ov45_0222EC10(&v1);

    v2 = &param0->profile;
    ov45_0222D638(param1, ov45_0222AA84(v2), ov45_0222AAA8(v2), 0);

    for (v0 = 0; v0 < 20; v0++) {
        if (v1.unk_04[v0] != -1) {
            v2 = (const WiFiPlazaProfile *)ov45_0222EA2C(v1.unk_04[v0]);
            ov45_0222D638(param1, ov45_0222AA84(v2), ov45_0222AAA8(v2), 0);
        }
    }

    for (v0 = 0; v0 < 20; v0++) {
        v4 = ov45_0222EDC4((u8)v0);
        v5 = ov45_0222EDF0(v0);

        ov45_0222D638(param1, v4, v5, 1);
    }
}

void ov45_0222B840(WiFiPlazaSystem *param0) {
    s64 v0;
    s64 v1;

    if ((param0->unk_1FC.unk_00_0 == 1) && (param0->unk_1FC.unk_00_7 == 0) && (ov45_0222E96C() == UnkEnum_ov66_0223287C_02)) {
        v0 = ov45_0222AD4C(param0);
        v0 += param0->unk_04.unk_DC;

        v1 = param0->unk_04.unk_D4;

        if (v1 > v0) {
            param0->unk_1FC.unk_00_7 = 1;
            param0->unk_1FC.unk_04 = (30 * 30);
        }
    }
}
