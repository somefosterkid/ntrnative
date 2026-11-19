#include "global.h"
#include "overlay_96_ov96_mon_icon_work.h"
#include "overlay_96_ov96_sprite_mgr.h"

typedef struct Ov96_021FC630Entry {
    u32 unk0;
    Ov96Obj *obj;
} Ov96_021FC630Entry;

typedef struct Ov96_021FC630Row {
    Ov96_021FC630Entry entries[5];
    u32 unk28;
} Ov96_021FC630Row;

void ov96_021FC630(Ov96_021FC630Row *rows, void *a1, u8 a2);

void ov96_021FC630(Ov96_021FC630Row *rows, void *a1, u8 a2) {
    u8 i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 5; j++) {
            Ov96_021FC630Entry *e = &rows[i].entries[j];
            e->obj = ov96_021EB3E4(a1, a2, 1, 0x6A, 10);
            ov96_021EB52C(e->obj, 1, 0);
            ov96_021EB630(e->obj, 5);
        }
    }
}
