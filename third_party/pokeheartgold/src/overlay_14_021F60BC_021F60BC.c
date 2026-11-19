#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon_storage_system.h"
#include "text.h"
#include "unk_0201956C.h"
#include "overlay_14_box_funcs.h"

struct UnkStruct_0201956C;

struct Window;

void ov14_021F60BC(UnkStruct_0201956C *mgr, int index, Window *window) {
    u16 w;
    u16 h;
    u16 *buf;
    u16 tile;
    u16 pal;
    u16 i;
    u16 j;

    buf = sub_02019B08(mgr, index);
    sub_02019B44(mgr, index, &w, &h);
    tile = GetWindowBaseTile(window);
    pal = window->paletteNum << 12;
    buf[0] = pal + 0x3E8;
    buf[w - 1] = pal + 0x3EA;
    buf[w * (h - 1)] = pal + 0x3F1;
    buf[w * h - 1] = pal + 0x3F3;
    for (i = 0; i < w - 2; i++) {
        buf[i + 1] = pal + 0x3E9;
        buf[i + w * (h - 1) + 1] = pal + 0x3F2;
    }
    for (i = 0; i < h - 2; i++) {
        buf[w * (i + 1)] = pal + 0x3EB;
        buf[w * (i + 2) - 1] = pal + 0x3ED;
    }
    for (i = 0; i < h - 2; i++) {
        for (j = 0; j < w - 2; j++) {
            buf[j + w * (i + 1) + 1] = pal + tile;
            tile++;
        }
    }
}
