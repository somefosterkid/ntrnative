#include "global.h"

typedef struct {
    u32 agbGameCodeReversed;
    u8 gameSet;
    u8 gameType;
    u16 language;
} GBAPokemonCartInfo;

extern u8 sAgbCartNintendoLogo[0xa0 - 4];

extern const GBAPokemonCartInfo *sPmAgbCartridgeSpec;

int IdentifyPmAgbCartridge(const GBAPokemonCartInfo *cartInfo, int param1);

int IdentifyPmAgbCartridge(const GBAPokemonCartInfo *cartInfo, int param1) {
    u32 agbGameCode;

    sPmAgbCartridgeSpec = NULL;

    if (!CTRDG_IsAgbCartridge()) {
        return 1;
    }

    agbGameCode = CTRDG_GetAgbGameCode();
    {
        int i;

        for (i = 0; i < param1; i++) {
            if (cartInfo[i].agbGameCodeReversed == agbGameCode) {
                sPmAgbCartridgeSpec = cartInfo + i;
                break;
            }
        }

        if (sPmAgbCartridgeSpec == NULL) {
            return 2;
        }
    }

    CTRDG_Enable(TRUE);

    {
        u32 v2[0xc0 / 4];
        u8 *v3 = (u8 *)&(v2[0]);

        CTRDG_CpuCopy32((void *)0x8000000, (void *)&(v2[0]), 0xc0);

        {
            int v4;

            for (v4 = 0; v4 < sizeof(sAgbCartNintendoLogo); v4++) {
                if (sAgbCartNintendoLogo[v4] != v3[4 + v4]) {
                    return 4;
                }
            }
        }

        {
            int v5;
            u8 v6 = 0;

            for (v5 = 0xa0; v5 <= 0xbc; v5++) {
                v6 += v3[v5];
            }

            v6 = (u8)(-(v6 + 0x19));

            if (v3[0xbd] != v6) {
                return 4;
            }
        }
    }

    return 0;
}
