#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "math_util.h"
#include "unk_02005D10.h"

#include "overlay_96_unk_struct_ov96_021_f94a8.h"

void ov96_021E8228(PokeathlonCourseData *data, u32 participant, u32 slot, s32 kind, u32 value);

s32 ov96_021FC164(void *param0, s32 slot, s32 pos);

extern const f32 ov96_0221C444[];


void ov96_021FBA3C(PokeathlonCourseData *data, UnkStruct_ov96_021F94A8 *work, s16 *out, u8 participant, u32 slot, u8 index);

void ov96_021FBA3C(PokeathlonCourseData *data, UnkStruct_ov96_021F94A8 *work, s16 *out, u8 participant, u32 slot, u8 index) {
    UnkStruct_ov96_021FBA3C *entry;
    s32 pos;
    u8 roll;
    f32 total;
    s32 step;

    entry = &work->cpu[index];
    if (entry->finished != 1 && entry->state != 2 && entry->level == 0 && entry->speed < entry->maxSpeed) {
        entry->speed += entry->accel;
        if (entry->speed > entry->maxSpeed) {
            entry->speed = entry->maxSpeed;
        }
    }
    if (entry->state == 2) {
        entry->timer--;
        if (entry->timer == 0) {
            entry->state = 0;
        }
    } else {
        pos = ov96_021FC164(work->unk_D8, slot, entry->pos);
        if (pos != -1 && entry->lastPos != pos) {
            roll = LCRandom() % 100;
            if (roll < entry->chanceA) {
                if (entry->level < 4) {
                    entry->level++;
                    entry->speed = entry->maxSpeed + ov96_0221C444[entry->level - 1];
                }
                ov96_021E8228(data, participant, slot, 6, 1);
                ov96_021E8228(data, participant, slot, 2, 1);
            } else if (roll < entry->chanceB) {
                entry->state = 2;
                entry->timer = 30;
                entry->level = 0;
                entry->speed = 0.5f;
                ov96_021E8228(data, participant, slot, 1, 1);
            } else {
                ov96_021E8228(data, participant, slot, 2, 1);
            }
            entry->lastPos = pos;
        }
    }
    total = entry->speed + entry->frac;
    step = (s32)total;
    entry->frac = total - step;
    out[slot] = entry->pos += step;
    if (entry->finished == 0 && entry->pos >= 0x1000) {
        out[slot + 8] = 1;
        out[slot + 5] = work->frameCount;
        entry->unk1C = work->frameCount;
        entry->finished = 1;
        PlaySE(0x8A1);
    }
}
