#include "global.h"
#include "field_system_def.h"

#include "unk_02032844.h"
#include "unk_struct_0209_bdf8.h"
#include "save.h"
#include "options.h"
#include "overlay_85_ov109_021D5140.h"
#include "game_stats.h"
#include "unk_struct_0209_c194.h"

struct GameStats;

struct JournalEntry;

struct Options;

struct SaveData;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D5140_t;

BOOL sub_02096D4C(UnkStruct_0209BDF8 *param0, u32 param1, const void *param2, u32 param3);

int sub_02096FFC(void);

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_0209BF64;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

void ov85_021E9C84(UnkStruct_ov109_021D5140 *param0, int param1, u8 param2);

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_02038C1C(s8 maxNumConnection);
#else
void sub_02038C1C(int maxNumConnection);
#endif

int sub_02037454(void);

u16 sub_0203769C(void);

void sub_02096DF4(int param0, int param1, void *param2, void *param3);

void sub_02096DF4(int param0, int param1, void *param2, void *param3) {
    UnkStruct_0209BDF8 *v0 = param3;
    UnkStruct_0209BF64 v1;
    UnkStruct_0209BF64 *v2 = param2;

    if (param0 != 0) {
        if (sub_0203769C() == 0) {
            v1 = *v2;
            v1.unk_00 = param0;
            v1.unk_01 = v0->unk_2C;

            switch (v2->unk_02) {
            case 0:
                if ((v0->unk_2C != sub_02037454()) || (v0->unk_2C != sub_02096FFC()) || (v0->unk_2C != MATH_CountPopulation(sub_02033250()))) {
                    v1.unk_03 = 0;
                } else {
                    v0->unk_30 |= 1 << param0;
                    v1.unk_03 = 1;

                    sub_02038C1C(sub_02037454());
                }
                break;
            case 1:
                break;
            }

            sub_02096D4C(v0, 2, &v1, sizeof(UnkStruct_0209BF64));
        }
    } else {
        switch (v2->unk_02) {
        case 0:
            if (v2->unk_00 == sub_0203769C()) {
                if (v2->unk_03 == 0) {
                    ov85_021E9C84(
                        v0->unk_00->unk_38, 8, v2->unk_00);
                } else {
                    v0->unk_38 = v2->unk_01;
                    ov85_021E9C84(
                        v0->unk_00->unk_38, 7, v2->unk_00);
                }
            }
            break;
        case 1:
            ov85_021E9C84(v0->unk_00->unk_38, 19, v2->unk_00);
            break;
        }
    }
}
