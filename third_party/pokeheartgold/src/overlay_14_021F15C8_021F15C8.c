#include "global.h"

#include "item.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "unk_0201956C.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021EA378(UnkStruct_ov14_boxapp *app);

void ov14_021E765C(UnkStruct_ov14_boxapp *app);
void ov14_021E884C(UnkStruct_ov14_boxsys *sys);
int ov14_021E7588(UnkStruct_ov14_boxapp *app, int a1);
int ov14_021E6480(UnkStruct_ov14_boxapp *app, int a1);
void ov14_021F08BC(UnkStruct_ov14_boxapp *app);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
void ov14_021F48B4(UnkStruct_ov14_boxapp *app);
void ov14_021F57B8(UnkStruct_ov14_boxapp *app);

struct BoxPokemon;

struct CAPSULE;

struct GridInputHandler;

struct Mail;

struct MailMessage;

struct ManagedSprite;

struct PC_Box;

struct Party;

struct PartyPokemon;

struct PokemonStorageSystem;

struct SEAL;

struct UnkStruct_0201956C;

struct UnkStruct_ov14_boxapp;

struct UnkStruct_ov14_boxsys;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;


int ov14_021F15C8(UnkStruct_ov14_boxapp *app, u32 a1) {
    UnkStruct_ov14_boxsys *sys;
    s16 pos[2];
    int count;
    int next;
    u32 slot;
    Pokemon *mon;

    app->sys->unk_88CC = a1 == 0xFF ? TRUE : FALSE;
    ManagedSprite_GetPositionXY(app->sys->sprites[9], &pos[1], &pos[0]);
    sys = app->sys;
    ManagedSprite_SetPositionXY(sys->sprites[sys->unk_4094[app->unk21]], pos[1], pos[0] + 4);
    if (a1 < 0x24) {
        if (a1 >= 0x1E) {
            count = Party_GetCount(app->party);
            if (app->unk21 < 0x1E) {
                if (a1 - 0x1E > (u32)count) {
                    ov14_021E765C(app);
                    ov14_021E8634(app->sys->unk_2F0);
                } else {
                    ov14_021E884C(app->sys);
                }
            } else {
                if (a1 - 0x1E >= (u32)count) {
                    ov14_021E765C(app);
                    ov14_021E8634(app->sys->unk_2F0);
                } else {
                    ov14_021E884C(app->sys);
                }
            }
        } else {
            ov14_021E884C(app->sys);
        }
    } else if (a1 != 0xFF) {
        app->unk25 = a1 + 6 * (app->unk25 / 6) - 0x25;
        ov14_021F48B4(app);
        ov14_021F57B8(app);
        slot = app->unk21;
        if (slot >= 0x1E) {
            slot -= 0x1E;
            mon = Party_GetMonByIndex(app->party, slot);
            if (ItemIdIsMail((u16)GetMonData(mon, MON_DATA_HELD_ITEM, NULL)) == TRUE) {
                ov14_021E884C(app->sys);
            } else if (GetMonData(mon, MON_DATA_BALL_CAPSULE_ID, NULL) != 0) {
                ov14_021E884C(app->sys);
            } else if (ov14_021E6480(app, slot) == 0) {
                ov14_021E884C(app->sys);
            } else if (app->unk25 == app->unk1F) {
                ov14_021E765C(app);
                ov14_021E8634(app->sys->unk_2F0);
            } else if (PCStorage_CountEmptySpotsInBox(app->storage, app->unk25) == 0) {
                ov14_021E884C(app->sys);
            } else {
                ov14_021E765C(app);
                ov14_021E8634(app->sys->unk_2F0);
            }
        } else if (app->unk25 == app->unk1F) {
            ov14_021E765C(app);
            ov14_021E8634(app->sys->unk_2F0);
        } else if (PCStorage_CountEmptySpotsInBox(app->storage, app->unk25) == 0) {
            ov14_021E884C(app->sys);
        } else {
            ov14_021E765C(app);
            ov14_021E8634(app->sys->unk_2F0);
        }
    } else {
        next = GridInputHandler_GetNextInput(app->sys->unk_2C);
        if ((u32)next < 0x24) {
            if (ov14_021E7588(app, next) == 0) {
                ov14_021E8634(app->sys->unk_2F0);
            }
        } else {
            ov14_021E765C(app);
            ov14_021E8634(app->sys->unk_2F0);
        }
    }
    app->sys->unk_44C = a1;
    app->sys->unk_44B = 0;
    ov14_021F08BC(app);
    app->unk22 = 2;
    return ov14_021F0234(app, ov14_021EA378, 0x2B);
}
