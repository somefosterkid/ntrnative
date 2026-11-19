// The game's side of the route pilot's story readout (port/include/port_route_story.h): included once by each game's
// port/extern*/src/system/game_event.c stand-in after port_route_map_game.h, so it compiles with that game's headers
// (Black's src/ and include/, Black 2's third_party/pokebw2). It reads what the game itself checks: the EventWork's flags
// and work values (src/system/event_work.c), the zone's NPC entries and their spawn flags (fldmmdl.c / field_actor.c's
// SpawnAllZoneNPCs: an NPC is created when its flag is clear), the triggers' work and value and the BG events' hidden
// item flags (eventdata_system.c), the zone's init scripts and its scene table (script_sys.c's
// FieldScript_GetSceneChangeSCRID: the first entry whose work has its value runs). Nothing is changed in the game.
#include "save/event_work.h"
#include "field/hidden_item.h"
#include "pml/poke_party.h"
#include "constants/pokemon.h"
#include "port_route_story.h"
#include "port_route_cheat.h"
#include <stdio.h>
#include <string.h>
#include "field/field_script_event.h"
#include "save/bag.h"
#include "save/save_control.h"
#include "save/trainer_card.h"
#include "gfl/rtc_cache.h"
#include "field/event_mapchange.h"
#include "system/season.h"
#include "gfl/heap.h"

static GameSystem *sPortStoryGsys;

// the pilot's X: cheats (port/include/port_route_cheat.h), on the next field frame, with the game's own functions
static void PortStory_Cheat(GameSystem *gsys) {
    PortRouteCheat *c = &gPortRouteCheat;
    GameData *gd = GSYS_GetGameData(gsys);
    TrainerGameInfoSave *info = getTrainerCardDataBlkAddress(gd);
    BoxPkmCreateParams p;
    int ok;

    if (c->kind == PORT_CHEAT_WARP && c->after) {   // X:warp's second half: a script's warp is followed by its FadeIn command,
        if (GSYS_GetNowEvent(gsys) != NULL) {       // which sets up the message BG's balloon windows (FieldG2D_Prepare3DSurface);
            c->after = 2;                           // without it the first speech bubble of a scene dereferences NULL
        } else if (c->after == 2) {
            FieldFadeTCB_Start(gsys, GSYS_GetField(gsys), 0, 0, 0);   // 0: the quick fade-in
            c->after = 0;
            c->done = 1;
        }
        return;
    }
    if (!c->pending) {
        return;
    }
    if (c->kind == PORT_CHEAT_SCRIPT) {   // R:script:ZONE:ID: when no event runs, start the zone's script the way the field's
        if (GSYS_GetNowEvent(gsys) != NULL) {   // event provider does (a trigger's: no actor); wait for a free field
            return;
        }
        c->pending = 0;
        if ((int)PlayerState_GetZoneID(GSYS_GetPlayerState(gsys)) != c->a) {
            fprintf(stderr, "cheat: R:script %d:%d refused, the player is in zone %d\n", c->a, c->b,
                    (int)PlayerState_GetZoneID(GSYS_GetPlayerState(gsys)));
            c->done = 2;
            return;
        }
        GSYS_SetNowEvent(gsys, EventScriptCall_Create(gsys, (u16)c->b, NULL, Field_GetHeapID(GSYS_GetField(gsys))));
        fprintf(stderr, "cheat: script %d of zone %d started\n", c->b, c->a);
        c->done = 1;
        return;
    }
    if (c->kind == PORT_CHEAT_WARP) {   // X:warp:ZONE:X:Z[:DIR]: the map change a script's warp command makes, when no event runs
        VecFx32 pos;

        if (GSYS_GetNowEvent(gsys) != NULL) {
            return;
        }
        c->pending = 0;
        pos.x = c->b * FX32_CONST(16);
        pos.y = 0;
        pos.z = c->c * FX32_CONST(16);
        GSYS_SetNowEvent(gsys, EventMapChange_CreateGridDefault(gsys, GSYS_GetField(gsys), (u16)c->a, &pos, (u16)c->d));
        fprintf(stderr, "cheat: warp to zone %d at %d,%d dir %d\n", c->a, c->b, c->c, c->d);
        c->after = 1;   // done = 1 when the map change has ended and the fade-in is started
        return;
    }
    c->pending = 0;
    switch (c->kind) {
    case PORT_CHEAT_WORK:
        if (c->a < 0x4000 || c->a >= 0x4000 + EVENT_WORK_COUNT) {
            fprintf(stderr, "cheat: work 0x%x is out of range\n", c->a);
            break;
        }
        fprintf(stderr, "cheat: work 0x%x was %u, set to %d\n", c->a, (unsigned)*EventWork_GetWkPtr(GameData_GetEventWork(gd), (u16)c->a), c->b);
        *EventWork_GetWkPtr(GameData_GetEventWork(gd), (u16)c->a) = (u16)c->b;
        break;
    case PORT_CHEAT_FLAG:
        fprintf(stderr, "cheat: flag 0x%x was %d, set to %d\n", c->a, (int)EventWork_FlagGet(GameData_GetEventWork(gd), (u16)c->a), c->b);
        if (c->b) {
            EventWork_FlagSet(GameData_GetEventWork(gd), (u16)c->a);
        } else {
            EventWork_FlagReset(GameData_GetEventWork(gd), (u16)c->a);
        }
        break;
    case PORT_CHEAT_MONEY:
        subCashFromTotal(info, getCash(info));
        addCashToTotal(info, (u32)c->a);
        fprintf(stderr, "cheat: money set to %u\n", (unsigned)getCash(info));
        break;
    case PORT_CHEAT_ITEM:
        ok = BagSave_AddItem(GameData_GetBag(gd), (u16)c->a, (u16)c->b, HEAPID_GAMESYSTEM);
        fprintf(stderr, "cheat: item %d x%d %s, bag holds %d\n", c->a, c->b, ok ? "added" : "NOT added",
                (int)BagSave_GetItemCountByID(GameData_GetBag(gd), (u16)c->a, HEAPID_GAMESYSTEM));
        break;
    case PORT_CHEAT_MON:
        memset(&p, 0, sizeof(p));
        p.heapId = HEAPID_GAMESYSTEM;
        p.species = (u32)c->a;
        p.level = (u32)c->b;
        p.ability = 2;
        p.sex = 2;
        p.param1C = 2;
        p.ball = 4;
        ok = addPkmToParty(gd, &p);
        if (ok && c->d > 0) {   // X:mon:SPECIES:LEVEL:LEAD:MOVE: move slot 0 is MOVE (full PP); the field-move menus read real move slots
            PartyPkm *np = PokeParty_GetPkm(GameData_GetParty(gd), (u32)(PokeParty_GetPkmCount(GameData_GetParty(gd)) - 1));

            PokeParty_SetParam(np, PKM_PARAM_MOVE1, (uintptr_t)c->d);
            PokeParty_SetParam(np, PKM_PARAM_MOVE1_PP, 15);
        }
        if (ok && c->c == 1 && PokeParty_GetPkmCount(GameData_GetParty(gd)) > 1) {   // X:mon:SPECIES:LEVEL:1 = the lead
            PokeParty_SwapPkms(GameData_GetParty(gd), PokeParty_GetPkmCount(GameData_GetParty(gd)) - 1, 0, HEAPID_GAMESYSTEM);
        }
        fprintf(stderr, "cheat: mon %d level %d %s, party %d%s%s\n", c->a, c->b, ok ? "added" : "NOT added (party full)",
                (int)PokeParty_GetPkmCount(GameData_GetParty(gd)), ok && c->c == 1 ? " (lead)" : "", ok && c->d > 0 ? " (move 1 set)" : "");
        break;
    case PORT_CHEAT_BADGE:
        addBadge(info, (u32)c->a);
        fprintf(stderr, "cheat: badge %d added, badges %u\n", c->a, (unsigned)getBadgeCount(info));
        break;
    case PORT_CHEAT_HEAL:   // X:heal: the Pokemon Center's own heal (HP, status, PP)
        PokeParty_RecoverAll(GameData_GetParty(gd));
        fprintf(stderr, "cheat: party healed (HP, status, PP), party %d\n", (int)PokeParty_GetPkmCount(GameData_GetParty(gd)));
        break;
    }
    c->done = 1;
}

static void PortStory_Fill(PortStory *s, int full) {
    GameData *gd;
    EventWork *ew;
    EventData *ed;
    Field *field;
    FieldPlayer *player;
    FieldActor *actor, *other;
    MMSys *mmsys;
    u32 index;
    int i;

    s->ok = 0;
    if (sPortStoryGsys == NULL || (gd = GSYS_GetGameData(sPortStoryGsys)) == NULL ||
        (ew = GameData_GetEventWork(gd)) == NULL) {
        return;
    }
    s->zone = PlayerState_GetZoneID(GSYS_GetPlayerState(sPortStoryGsys));
    s->flags = ew->flags;
    s->nflags = (int)sizeof(ew->flags) * 8;
    s->works = ew->works;
    s->nworks = EVENT_WORK_COUNT;
    memset(s->temp, 0, sizeof s->temp);
    for (i = 0; i < PORT_STORY_NTEMP; i++) {
        if (EventWork_FlagGet(ew, (u16)(PORT_STORY_TEMP0 + i))) {
            s->temp[i / 8] |= 1 << (i % 8);
        }
    }
    s->ok = 1;
    s->nnpc = s->nlive = s->nbg = s->ntrig = s->nscene = s->ninit = 0;
    if (!full || (ed = GameData_GetEventData(gd)) == NULL) {
        return;
    }
    for (i = 0; i < ed->npcCount && ed->npcs != NULL && s->nnpc < 64; i++) {
        const ZoneNPC *n = &ed->npcs[i];

        s->npc[s->nnpc].uid = n->uid;
        s->npc[s->nnpc].flag = n->spawnFlag;
        s->npc[s->nnpc].script = n->scrId;
        s->npc[s->nnpc].ev = n->evType;
        s->npc[s->nnpc].rail = n->isRail == 1;
        s->npc[s->nnpc].x = n->isRail == 1 ? -1 : n->pos.grid.x;
        s->npc[s->nnpc].z = n->isRail == 1 ? -1 : n->pos.grid.z;
        s->nnpc++;
    }
    for (i = 0; i < ed->entityCount && ed->entities != NULL && s->nbg < 64; i++) {
        const ZoneBGEntity *b = &ed->entities[i];
        const s32 *coords = (const s32 *)&b->pos;

        s->bg[s->nbg].script = b->scrId;
        s->bg[s->nbg].cond = b->condition;
        s->bg[s->nbg].flag = b->condition == 2 ? GetHiddenItemEventFlagNoBySCRID(b->scrId) : 0;
        s->bg[s->nbg].rail = b->isRail == 1;
        s->bg[s->nbg].x = b->isRail == 1 ? -1 : coords[0];
        s->bg[s->nbg].z = b->isRail == 1 ? -1 : coords[1];
        s->nbg++;
    }
    for (i = 0; i < ed->triggerCount && ed->triggers != NULL && s->ntrig < 64; i++) {
        const ZoneTrigger *g = &ed->triggers[i];

        s->trig[s->ntrig].script = g->scrId;
        s->trig[s->ntrig].work = g->workId;
        s->trig[s->ntrig].value = g->workValue;
        s->trig[s->ntrig].type = g->type;
        s->trig[s->ntrig].rail = g->isRail == 1;
        s->trig[s->ntrig].x = g->isRail == 1 ? -1 : g->pos.grid.x;
        s->trig[s->ntrig].z = g->isRail == 1 ? -1 : g->pos.grid.z;
        s->trig[s->ntrig].w = g->isRail == 1 ? 0 : g->pos.grid.width;
        s->trig[s->ntrig].h = g->isRail == 1 ? 0 : g->pos.grid.height;
        s->ntrig++;
    }
    {   // the init scripts: 6-byte entries {type, script or (type 1) the offset of the scene table} up to type 0
        const u8 *p = (const u8 *)ed->initScript, *q;
        int n;

        for (n = 0; p != NULL && n < 16 && (p[0] | p[1] << 8) != 0; n++, p += 6) {
            unsigned type = p[0] | p[1] << 8;

            if (type != 1) {
                if (s->ninit < 16) {
                    s->init[s->ninit].type = type;
                    s->init[s->ninit].script = p[2] | p[3] << 8;
                    s->ninit++;
                }
                continue;
            }
            if ((p[2] | p[3] << 8 | p[4] << 16 | (u32)p[5] << 24) == 0) {
                continue;
            }
            q = p + 6 + (p[2] | p[3] << 8 | p[4] << 16 | (u32)p[5] << 24);
            for (; (q[0] | q[1] << 8) != 0 && s->nscene < 32; q += 6) {
                s->scene[s->nscene].work = q[0] | q[1] << 8;
                s->scene[s->nscene].value = q[2] | q[3] << 8;
                s->scene[s->nscene].script = q[4] | q[5] << 8;
                s->nscene++;
            }
        }
    }
    if ((field = GSYS_GetField(sPortStoryGsys)) == NULL || (player = Field_GetPlayer(field)) == NULL ||
        (actor = FieldPlayer_GetActor(player)) == NULL) {
        return;
    }
    mmsys = GetActorMModelSystem(actor);
    index = 0;
    while (mmsys != NULL && NextActor(mmsys, &other, &index) && s->nlive < 64) {
        if (other != actor) {
            s->live[s->nlive++] = GetActorUID(other);
        }
    }
}

// the stand-in's hook, each frame its game system updates the events
static void PortStory_Hook(GameSystem *gsys) {
    sPortStoryGsys = gsys;
    gPortRouteStoryFill = PortStory_Fill;
    gPortRoutePartyCount = PokeParty_GetPkmCount(GameData_GetParty(GSYS_GetGameData(gsys)));   // C:ball checks it rose
    {   // F:fly: the first party slot that knows Fly (move 19), -1 for none
        PokeParty *pty = GameData_GetParty(GSYS_GetGameData(gsys));
        int i, m;

        gPortRouteFlySlot = -1;
        for (i = 0; i < 6; i++) {   // F:teach: species and moves
            for (m = 0; m < 5; m++) {
                gPortRouteParty[i][m] = i >= gPortRoutePartyCount ? 0 :
                    (int)PokeParty_GetParam(PokeParty_GetPkm(pty, (u32)i), (PkmField)(m == 0 ? PKM_PARAM_SPECIES : PKM_PARAM_MOVE1 + m - 1), NULL);
            }
        }
        for (i = 0; i < gPortRoutePartyCount && gPortRouteFlySlot < 0; i++) {
            for (m = 0; m < 4; m++) {
                if (PokeParty_GetParam(PokeParty_GetPkm(pty, (u32)i), (PkmField)(PKM_PARAM_MOVE1 + m), NULL) == 19) {
                    gPortRouteFlySlot = i;
                    gPortRouteFlyMove = m;
                    break;
                }
            }
        }
    }
    {   // X:time waits for this: the game's cached clock and the season it makes
        RTCDate date;
        RTCTime time;

        RTC_GetCachedDateTime(&date, &time);
        gPortRouteClock[0] = (int)date.month;
        gPortRouteClock[1] = (int)date.day;
        gPortRouteClock[2] = (int)time.hour;
        gPortRouteClock[3] = (int)Season_GetRealTime();
    }
    if (gPortRouteCatch) {   // C:ball:ITEM: the balls in bag order (the battle bag lists the Items pocket's balls so)
        BagItem *slot;
        int n = 0, i;

        for (i = 0; n < 16 && (slot = BagSave_GetItemIndexHandle(GameData_GetBag(GSYS_GetGameData(gsys)), BAG_POCKET_ITEMS, (u16)i)) != NULL; i++) {
            if (slot->item >= 1 && slot->item <= 15 && slot->count != 0) {
                gPortRouteBalls[n++] = slot->item;
            }
        }
        gPortRouteBalls[n] = 0;
    }
    PortStory_Cheat(gsys);
    PortRouteStory_Frame(PORT_ROUTE_GAME);
}
