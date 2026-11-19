#include "global.h"

#include "error_handling.h"

int ov07_02234718(int param0);

static int hgasset_ov07_02234718_v0[] __attribute__((section(BWPORT_DATA_SECTION))) = {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0
    };

int ov07_02234718(int param0) {
    __typeof__(hgasset_ov07_02234718_v0) v0; __builtin_memcpy((void *)&v0, &hgasset_ov07_02234718_v0, sizeof(v0));
    int v1 = param0 - 5;

    if (v1 >= NELEMS(v0)) {
        GF_ASSERT(FALSE);
        return 0;
    }

    return v0[v1];
}

// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);
__attribute__((constructor)) static void hgasset_register_src_overlay_07_02234718_02234718_c_0(void) {
    PORT_AssetAdd("IPKE", "src/overlay_07_02234718_02234718.c:hgasset_ov07_02234718_v0", (void *)&hgasset_ov07_02234718_v0, sizeof(hgasset_ov07_02234718_v0), "u8");
    PORT_AssetAdd("IPGE", "src/overlay_07_02234718_02234718.c:hgasset_ov07_02234718_v0", (void *)&hgasset_ov07_02234718_v0, sizeof(hgasset_ov07_02234718_v0), "u8");
}
