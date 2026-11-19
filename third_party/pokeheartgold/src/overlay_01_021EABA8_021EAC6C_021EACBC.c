#include "global.h"

#include "camera.h"
#include "error_handling.h"
#include "overlay_01_021EABA8.h"

typedef struct UnkTblB {
    u8 pad[4];
    u16 d;
    u8 pad2[0x1E];
} UnkTblB;

extern const FieldCameraShift ov01_02206464[];

extern UnkTblB ov01_02206478[];

extern s32 ov01_02209B60;

void ov01_021EACBC(FieldCameraState *p, u8 a1);

void ov01_021EACBC(FieldCameraState *p, u8 a1) {
    UnkTblB *t;
    if (a1 != 0) {
        if (p->active != 0) {
            if (p->type == a1) {
                p->unk2E = (p->unk2E + 1) % 2;
                return;
            }
            GF_AssertFail();
            return;
        }
        t = &ov01_02206478[p->cameraType];
        p->type = a1;
        p->active = 1;
        if (ov01_02209B60 == 0) {
            p->unk2C = t->d;
            p->block = ov01_02206464[a1 - 1].block;
            ov01_02209B60 = p->type;
        } else {
            p->unk2C = ov01_02206464[a1 - 1].block.angle;
            p->block.angle = t->d;
            p->block.unk2 = 0;
            p->block.offset = ov01_02206464[a1 - 1].block.offset;
            p->block.offset.x *= -1;
            p->block.offset.y *= -1;
            p->block.offset.z *= -1;
            ov01_02209B60 = 0;
        }
        p->duration = ov01_02206464[a1 - 1].frames;
        p->frame = 0;
    }
}

void ov01_021EAE50(Camera *camera, u16 *from, u16 *to, s32 step, u8 total);

void ov01_021EAEA4(Camera *camera, void *offset, s32 step, s32 total);

void ov01_021EAD8C(FieldCameraState *state);

void ov01_021EAD8C(FieldCameraState *state) {
    VecFx32 sp4;
    u8 temp_r5;
    u8 var_r1;

    if (state->active != 0) {
        if (state->unk2E == 0) {
            state->frame = (u8)(state->frame + 1);
            ov01_021EAE50(state->camera, &state->unk2C, &state->block.angle, state->frame, state->duration);
            ov01_021EAEA4(state->camera, &state->block.offset, state->frame, state->duration);
            if ((u32)state->frame >= (u32)state->duration) {
                state->active = 0;
            }
        } else {
            sp4 = state->block.offset;
            sp4.x *= -1;
            sp4.y *= -1;
            sp4.z *= -1;
            temp_r5 = state->duration;
            ov01_021EAE50(state->camera, &state->block.angle, &state->unk2C, (u8)(temp_r5 - state->frame), temp_r5);
            ov01_021EAEA4(state->camera, &sp4, state->frame, state->duration);
            state->frame = (u8)(state->frame - 1);
            if (state->frame == 0) {
                if (ov01_02209B60 != 0) {
                    var_r1 = 0;
                    ov01_02209B60 = (s32)var_r1;
                } else {
                    var_r1 = state->type;
                    ov01_02209B60 = (s32)var_r1;
                }
                state->unk2E = 0U;
                state->active = 0;
            }
        }
    }
}
