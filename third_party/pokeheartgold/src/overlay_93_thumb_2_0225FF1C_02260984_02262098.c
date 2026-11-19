#include "global.h"

#include "obj_char_transfer.h"
#include "overlay_93_thumb_2.h"
#include "sprite_system.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct FontOAM;

void ov93_02262098(UnkStruct_ov117_02265EC8 *param0, int param1);

void ov93_02262098(UnkStruct_ov117_02265EC8 *param0, int param1) {
    int v0, v1, v2;

    v1 = 100000;
    GF_ASSERT(6 == 6);

    for (v0 = 0; v0 < 6; v0++) {
        v2 = param1 / v1;
        param0->unk_18C[v0] = v2 * 16;
        param1 -= v2 * v1;
        v1 /= 10;
    }
}

void ov93_022620D4(UnkStruct_ov117_02265EC8 *param0);

void ov93_022620D4(UnkStruct_ov117_02265EC8 *param0) {
    int v0;

    for (v0 = 0; v0 < 6; v0++) {
        param0->unk_186[v0] = param0->unk_18C[v0];
        param0->unk_198[v0] = 2 * (6 - v0 - 1);
        param0->unk_192[v0] = 0;
    }

    param0->unk_19F = 0;
}

BOOL ov93_02262108(UnkStruct_ov117_02265EC8 *param0);

BOOL ov93_02262108(UnkStruct_ov117_02265EC8 *param0) {
    int v0;

    for (v0 = 0; v0 < 6; v0++) {
        if (param0->unk_180[v0] != param0->unk_186[v0]) {
            return 0;
        }
    }

    return 1;
}

BOOL ov93_02262130(UnkStruct_ov117_02265EC8 *param0);

BOOL ov93_02262130(UnkStruct_ov117_02265EC8 *param0) {
    int v0;
    int v1 = 6 - 1;
    int v2, v3, v4, v5;

    if (param0->unk_19F > 6) {
        return 1;
    }

    v3 = 0;
    v4 = 0;

    for (v0 = 0; v0 < 6; v0++, v1--) {
        v3 = v4;

        if (param0->unk_198[v1] > 0) {
            param0->unk_198[v1]--;
            v4 = 0;
            continue;
        }

        if ((param0->unk_192[v1] > 1) && (param0->unk_180[v1] == param0->unk_186[v1])) {
            v4 = 1;
            continue;
        }

        v4 = 0;
        v2 = param0->unk_180[v1];
        param0->unk_180[v1] += 23;
        v5 = param0->unk_186[v1] + 16 * 10;

        if (((v2 <= param0->unk_186[v1]) && (param0->unk_180[v1] >= param0->unk_186[v1])) || ((v2 <= v5) && (param0->unk_180[v1] >= v5))) {
            if (param0->unk_192[v1] < 1) {
                param0->unk_192[v1]++;
            } else if ((v1 == 6 - 1) || (v3 == 1)) {
                param0->unk_192[v1]++;
                param0->unk_180[v1] = param0->unk_186[v1];
                param0->unk_19F++;

                if (param0->unk_19F >= 6) {
                    return 1;
                }
            }
        }

        param0->unk_180[v1] %= 10 * 16;
    }

    return 0;
}

extern ManagedSpriteTemplate ov93_02262F6C;

ManagedSprite *ov93_02262230(UnkStruct_ov117_02261280_t *arg0) {
    ManagedSprite *managedSprite;

    managedSprite = SpriteSystem_NewSprite(arg0->unk_24, arg0->unk_28, &ov93_02262F6C);
    Sprite_TickFrame(managedSprite->sprite);
    return managedSprite;
}
