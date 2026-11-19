#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96hurdle_work.h"

struct PokeathlonCourseData;

void ov96_02206368(PokeathlonCourseData *data, Ov96HurdleWork *ctx, Ov96HurdleRunner *player);

int ov96_02207B8C(Ov96HurdleRunner *player);

u8 ov96_02207BD4(u8 value);

void ov96_021E8228(PokeathlonCourseData *data, u32 index, u32 sub, u32 kind, u32 value);

u32 ov96_021EAF8C(void *obj);

u32 ov96_02207300(Ov96HurdleWork *ctx, u32 dir, u32 param, VecFx32 *prevPos, VecFx32 *pos, VecFx32 *outPos);

int ov96_02207418(PokeathlonCourseData *data, Ov96HurdleRunner *players, VecFx32 *out);

void ov96_022078B0(void *unk564, Ov96HurdleRunner *player, u8 *out);

void ov96_02205E30(PokeathlonCourseData *data);

void ov96_02205E30(PokeathlonCourseData *data) {
    Ov96HurdleWork *ctx;
    Ov96HurdleRunner *player;
    VecFx32 hitPos;
    u8 skip[4];
    VecFx32 prevPos;
    VecFx32 friction;
    int i;
    int hit;
    int fric;
    fx32 fricMag;
    f32 speed = 6.0f;

    ctx = PokeathlonCourse_GetHeapAllocPtr4(data);
    ctx->unk_519 = 0;
    for (i = 0; i < 4; i++) {
        skip[i] = 0;
        if (ctx->runners[i].active != 0) {
            ov96_02206368(data, ctx, &ctx->runners[i]);
            skip[i] = 1;
        }
    }

    for (i = 0; i < 4; i++) {
        player = &ctx->runners[i];
        if (skip[i] != 0) {
            continue;
        }

        if (ov96_02207B8C(player) != 0) {
            player->unk_A7 = ov96_02207BD4(player->unk_AA);
            player->unk_AA = 0;
            player->unk_AB = 1;
            player->vel.x = 0;
            player->vel.y = 0;
            player->vel.z = 0;
            player->unk_B0 = 2;
            player->unk_A9 = 10;
            ov96_021E8228(data, (u8)player->unk_98, player->unk_B1, 3, player->unk_A7);
        }

        if (player->unk_A9 != 0 && player->unk_AB != 0) {
            if (player->unk_A8 == 0) {
                if (player->unk_A7 != 0) {
                    player->unk_AC = player->unk_AC + 1;
                    if (player->unk_AC > 999) {
                        player->unk_AC = 999;
                    }
                    player->unk_A7 = player->unk_A7 - 1;
                }
                player->unk_A9 = player->unk_A9 - 1;
                if (player->unk_A9 == 0) {
                    player->unk_A9 = 0;
                    player->unk_9C = 1;
                    player->unk_9D = 1;
                    ov96_021E8228(data, (u8)player->unk_98, player->unk_B1, 7, 1);
                } else {
                    player->unk_A8 = 4;
                }
            } else {
                player->unk_A8--;
            }
        }

        if (player->unk_AB != 0) {
            continue;
        }

        prevPos = player->pos;
        VEC_Add(&player->pos, &player->vel, &player->pos);

        if (player->pos.x < 0xD0000 || player->pos.x > 0x130000) {
            if (player->pos.y < 0x68000) {
                player->pos.y = 0x68000;
            } else if (player->pos.y > 0x188000) {
                player->pos.y = 0x188000;
            }
        } else {
            if (player->pos.y < 0x68000) {
                player->pos.y = 0x68000;
            } else if (player->pos.y > 0x1A0000) {
                player->pos.y = 0x1A0000;
            }
        }

        if (player->pos.y > 0x188000) {
            if (player->pos.x < 0xD0000) {
                player->pos.x = 0xD0000;
            } else if (player->pos.x > 0x130000) {
                player->pos.x = 0x130000;
            }
        } else {
            if (player->pos.x < 0x90000) {
                player->pos.x = 0x90000;
            } else if (player->pos.x > 0x170000) {
                player->pos.x = 0x170000;
            }
        }

        player->dir = (u8)ov96_02207300(ctx, player->dir, ov96_021EAF8C(player->unk_00[player->unk_B1]), &prevPos, &player->pos, &player->pos);

        switch (player->dir) {
        case 1:
            if (player->vel.y > 0) {
                player->vel.y *= -0.3;
            }
            break;
        case 2:
            if (player->vel.x < 0) {
                player->vel.x *= -0.3;
            }
            break;
        case 3:
            if (player->vel.y < 0) {
                player->vel.y *= -0.3;
            }
            break;
        case 4:
            if (player->vel.x > 0) {
                player->vel.x *= -0.3;
            }
            break;
        case 5:
            if (player->vel.x > 0) {
                player->vel.x *= -0.3;
            }
            if (player->vel.y > 0) {
                player->vel.y *= -0.3;
            }
            break;
        case 6:
            if (player->vel.x < 0) {
                player->vel.x *= -0.3;
            }
            if (player->vel.y > 0) {
                player->vel.y *= -0.3;
            }
            break;
        case 7:
            if (player->vel.x < 0) {
                player->vel.x *= -0.3;
            }
            if (player->vel.y < 0) {
                player->vel.y *= -0.3;
            }
            break;
        case 8:
            if (player->vel.x > 0) {
                player->vel.x *= -0.3;
            }
            if (player->vel.y < 0) {
                player->vel.y *= -0.3;
            }
            break;
        }

        hit = ov96_02207418(data, ctx->runners, &hitPos);
        if (ctx->unk_519 == 0 && hit != 0) {
            ctx->unk_519 = 1;
            ctx->unk_51E = hitPos.x / 4096;
            ctx->unk_51C = hitPos.y / 4096;
        }

        friction = player->vel;
        if (VEC_Mag(&friction) > 0) {
            fric = 4096.0 * (0.1 * speed);
            if (ctx->unk_510 != 0) {
                fric += 0x3000;
            }
            VEC_Normalize(&friction, &friction);
            friction.x = ((s64)friction.x * fric + 0x800) >> 12;
            friction.y = ((s64)friction.y * fric + 0x800) >> 12;
            fricMag = VEC_Mag(&friction);
            if (VEC_Mag(&player->vel) >= fricMag) {
                VEC_Subtract(&player->vel, &friction, &player->vel);
            } else {
                player->vel.x = 0;
                player->vel.y = 0;
            }
        }

        if (player->unk_9C == 0 && player->unk_A6 == 1) {
            player->unk_A2 = player->unk_A2 - 1;
            if (player->unk_A2 == 0) {
                player->unk_A6 = 0;
            }
        }

        ov96_022078B0(ctx->slots, player, &ctx->unk_6BC[i]);
    }
}
