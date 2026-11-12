// The game's side of the route pilot's story readout (port/include/port_route_story.h): included once by each game's
// port/extern*/src/system/game_event.c stand-in after port_route_map_game.h, so it compiles with that game's headers
// (Black's src/ and include/, Black 2's third_party/pokebw2). It reads what the game itself checks: the EventWork's flags
// and work values (src/system/event_work.c), the zone's NPC entries and their spawn flags (fldmmdl.c / field_actor.c's
// SpawnAllZoneNPCs: an NPC is created when its flag is clear), the triggers' work and value and the BG events' hidden
// item flags (eventdata_system.c), the zone's init scripts and its scene table (script_sys.c's
// FieldScript_GetSceneChangeSCRID: the first entry whose work has its value runs). Nothing is changed in the game.
#include "save/event_work.h"
#include "field/hidden_item.h"
#include "port_route_story.h"

static GameSystem *sPortStoryGsys;

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
    PortRouteStory_Frame(PORT_ROUTE_GAME);
}
