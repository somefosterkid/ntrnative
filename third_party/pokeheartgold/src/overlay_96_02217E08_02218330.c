#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_rd_work.h"

s32 _s32_div_f(s32);

typedef struct UnkStruct_ov96_02218330_Packet {
    u8 x;
    u8 y;
    u8 unk2_0 : 6;
    u8 unk2_6 : 2;
    u8 unk3;
    u8 unk4_0 : 4;
    u8 unk4_4 : 2;
    u8 unk4_6 : 2;
    u8 unk5_0 : 3;
    u8 unk5_3 : 2;
    u8 unk5_5 : 2;
    u8 unk5_7 : 1;
} UnkStruct_ov96_02218330_Packet;

typedef struct UnkStruct_ov96_02218330_PacketData {
    UnkStruct_ov96_02218330_Packet entries[4];
    u32 unk18;
    u32 unk1C_0 : 7;
    u32 unk1C_7 : 1;
    u32 unk1C_8 : 24;
} UnkStruct_ov96_02218330_PacketData;


void ov96_02218330(Ov96RdCourse *work, PokeathlonCourseData *data);

void ov96_02218330(Ov96RdCourse *work, PokeathlonCourseData *data) {
    UnkStruct_ov96_02218330_PacketData *packet;
    int flag;
    Ov96RdObj *obj;
    UnkStruct_ov96_02218330_Packet *ent;
    int i;
    int x, y;
    int v;

    flag = 0;
    packet = (UnkStruct_ov96_02218330_PacketData *)ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(data) + 0x28);
    // DS walks obj itself by one runner (sizeof(Ov96RdPlayer)); &work->players[i].obj per iteration does not match
    for (i = 0, obj = &work->players[0].obj, ent = packet->entries; i < 4; i++, obj = (Ov96RdObj *)((u8 *)obj + sizeof(Ov96RdPlayer)), ent++) {
        x = (s16)(obj->pos2C.x >> 12);
        y = (s16)(obj->pos2C.y >> 12);
        if (obj->w64 != 0) {
            obj->w64--;
        } else if (obj->w64 == 0) {
            obj->a = 0;
            obj->mode = 0;
        }
        ent->unk4_4 = obj->a;
        ent->unk2_6 = obj->mode;
        ent->unk4_6 = obj->e;
        ent->unk5_3 = obj->f16 - 1;
        if (x > 0xFF) {
            x = 0xFF;
        } else if (x < 0) {
            x = 0;
        }
        ent->x = x;
        if (y > 0xFF) {
            y = 0xFF;
        } else if (y < 0) {
            y = 0;
        }
        ent->y = y;
        ent->unk2_0 = obj->unk_50 >> 12;
        ent->unk4_0 = obj->state;
        ent->unk3 = obj->cnt;
        ent->unk5_5 = obj->f24;
        ent->unk5_0 = obj->b5F;
        if (obj->f31) {
            flag = 1;
            obj->f31 = 0;
        }
    }
    packet->unk1C_7 = flag;
    packet->unk1C_8 = 0;
    for (i = 0; i < 12; i++) {
        v = work->players[i / 3].obj.entries[i % 3].value >> 12;
        if (v == 0) {
            v = 2;
        } else if (v < 30) {
            v = 1;
        } else {
            v = 0;
        }
        packet->unk1C_8 += v << (i * 2);
    }
}
