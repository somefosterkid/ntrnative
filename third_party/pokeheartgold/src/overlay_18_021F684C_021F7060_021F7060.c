#include "global.h"

#include "unk_02005D10.h"
#include "unk_02019BA4.h"

#include "application/pokedex/pokedex_internal.h"

void ov18_021F7018(void *, u32);

extern u8 ov18_021FBC34[];


void ov18_021F7060(PokedexAppData *param_1, u32 param_2, u32 param_3) {
    u32 i;
    u8 *p;
    u8 v;

    if (param_3 - 0x1b <= 1) {
        v = ov18_021FBC34[param_1->unk_189C * 8];
        if ((param_2 == 0) || (param_2 == 6)) {
            p = ov18_021FBC34;
            for (i = 0; (s32)i <= 0x1a; i++, p += 8) {
                if (v == *p) {
                    param_2 = i;
                    GridInputHandler_SetNextLastUnk0FInputs(param_1->unk_1864, (u8)i, (u8)param_3, param_3 & 0xff);
                    break;
                }
            }
        } else if ((param_2 == 0x15) || (param_2 == 0x1a)) {
            p = ov18_021FBC34 + 0xd0;
            i = 0x1a;
            do {
                if (v == *p) {
                    param_2 = i;
                    GridInputHandler_SetNextLastUnk0FInputs(param_1->unk_1864, (u8)i, (u8)param_3, param_3 & 0xff);
                    break;
                }
                p -= 8;
                i--;
            } while ((s32)i >= 0);
        }
    }
    ov18_021F7018(param_1, param_2);
    param_1->unk_189C = param_3;
    PlaySE(0x8e8);
}
