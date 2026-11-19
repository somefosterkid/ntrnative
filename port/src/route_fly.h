// The F:fly:ZONE token of the route pilot (fy2, 2026-11-13): Fly used the way the player does it, through the game's own menus.
// Included by port/src/route_test.c next to route_fieldmove.h (FieldMoveStep calls FlyStep for "F:fly"); test-only like the rest.
//
//   F:fly:ZONE   X (the field menu), the Pokemon entry, A; the Pokemon list: the first party member that knows Fly (move 19: the
//                test cheat X:mon:SPECIES:LEVEL:1:19 gives one a real move slot; the list reads real slots), A; the submenu: down
//                to its Fly entry (item 16 + Fly's move slot), A; the Town Map (TOWNMAP_MODE_FLY: only the towns visited): the D-pad takes the cursor
//                to the place whose zone is ZONE, the game's own pull and ring do the rest, A when the ring is on it; the game's fly
//                event; done when the player is in ZONE and the field has been free 60 frames. Ends the leg, with a line, when
//                no party member knows Fly, the submenu has no Fly (HidenEvent says "cannot fly here"), ZONE is no place on the map
//                or not visited (canFly 0), or the flight does not arrive. Black, White and Platinum (the games' menus are hooked in
//                port/extern/src/ov021/field_menu.c, ov091/plist_sys.c, plist_menu.c, ov072/townmap.c; Platinum's: start_menu.c,
//                applications/party_menu/main.c, applications/town_map/main.c through pokeplatinum-wroutefly.patch and
//                port/game/route_gen4.c, whose Town Map is a grid: the cursor is exact on a cell, the party menu's context menu
//                is the submenu, the test cheat gives slot 0 the Fly entry when nobody knows it). ZONE is the zone id the
//                dump and story.py print; the Town Map's place has the same id in its first parameter.
//                HeartGold / SoulSilver (route_gen4.c's HEARTGOLD half, pokeheartgold-route.patch): the field menu's icons by
//                keys (pokeKey: the game's own move table), party_menu.c's list and context menu, the Pokegear's fly map
//                (fly_map.c: a grid, the cursor a cell; A on a flypoint opens its context menu, whose first entry A takes);
//                the party needs a real Fly move (no cheat entry).
//
// The logs: "pilot: F:fly:ZONE: <phase> at V-blank N" at each phase.

// press a key for two frames in every six (the menus read typed keys; a held key repeats later)
static int FlyPulse(int wait) {
    return wait % 6 == 1 || wait % 6 == 2;
}

static int FlyStep(const PortRouteState *st, const char *tok, int age) {
    static int sPhase, sWait, sTries, sZone, sFree, sTarget, sPressed, sArrive;
    static unsigned sSeen;   // case 5: the submenu entries the cursor was on since the last press across
    static int sAcross;      // case 5: this pulse presses right (a column came round without the entry)
    static unsigned sDir;
    static const char *sFor;
    unsigned now = OS_GetVBlankCount();
    PortRouteMenu m;
    int menuInput = gPortRouteMenu.vblank != 0 && gPortRouteMenu.vblank + 3 >= now && gPortRouteMenu.input;
    int listReady = gPortRoutePList.vblank != 0 && gPortRoutePList.vblank + 3 >= now && gPortRoutePList.state >= 2 &&
                    gPortRoutePList.state <= 4 && gPortRoutePList.subState == 1;
    int subReady = gPortRoutePList.menuVblank != 0 && gPortRoutePList.menuVblank + 3 >= now && gPortRoutePList.menuCount > 0;
    const PortRouteTownMap *tm = &gPortRouteTownMap;
    int tmLive = tm->vblank != 0 && tm->vblank + 3 >= now && tm->alive;
    char name[32];

    if (tok != sFor || age == 1) {
        sFor = tok;
        sPhase = sWait = sTries = sFree = sPressed = sArrive = 0;
        sTarget = -1;
        sZone = atoi(tok + 6);
        sMenuSlot = -1;
        sMenuBlocked = 0;
        sDir = 0;
        gPortRouteTownMap.vblank = 0;
        gPortRouteTownMap.alive = 0;
        gPortRoutePList.menuVblank = 0;
        gPortRoutePList.vblank = 0;
    }
    snprintf(name, sizeof name, "F:fly:%d", sZone);
    sWait++;
    switch (sPhase) {
    case 0:   // the field free for 8 frames, then X
        if (strcmp(st->game, "B") != 0 && strcmp(st->game, "W") != 0 && strcmp(st->game, "PT") != 0 && strcmp(st->game, "HG") != 0 &&
            strcmp(st->game, "SS") != 0) {
            End(st, "stuck: F:fly: written for Black, White, Platinum, HeartGold and SoulSilver only");
        }
        if (gPortRouteFlySlot < 0) {
            End(st, "stuck: F:fly: no party member knows Fly (X:mon:SPECIES:LEVEL:1:19 gives one)");
        }
        if (st->event) {
            sWait = 0;
            PressEvent(st);
            return 0;
        }
        SetKeys(sWait >= 8 ? 0x400u : 0);
        if (sWait >= 8) {
            fprintf(stderr, "pilot: %s: X at V-blank %u, Fly in slot %d\n", name, now, gPortRouteFlySlot);
            sPhase = 1;
            sWait = 0;
        }
        return 0;
    case 1:   // X held a second frame, then the menu takes input
        SetKeys(sWait == 1 ? 0x400u : 0);
        if (menuInput) {
            sPhase = 2;
            sWait = 0;
            sMenuSlot = -1;
            sMenuBlocked = 0;
            sDir = 0;
        } else if (sWait > 300) {
            if (++sTries > 3) {
                End(st, "stuck: F:fly: the field menu does not open");
            }
            sPhase = 0;
            sWait = 0;
        }
        return 0;
    case 2:   // to the Pokemon entry, then A
        if (!menuInput) {
            SetKeys(0);
            if (sWait > 300) {
                End(st, "stuck: F:fly: the field menu stopped taking input");
            }
            return 0;
        }
        if (gPortRouteMenu.pokeSlot < 0) {
            End(st, "stuck: F:fly: the field menu has no Pokemon entry");
        }
        if (sWait % 6 == 1 && sWait > 6 && gPortRouteMenu.slot == gPortRouteMenu.pokeSlot) {
            fprintf(stderr, "pilot: %s: A on the Pokemon entry (slot %d) at V-blank %u\n", name, gPortRouteMenu.slot, now);
            SetKeys(1);
            sPhase = 3;
            sWait = 0;
            return 0;
        }
        if (sWait % 6 == 1) {
            m = gPortRouteMenu;
            m.saveSlot = m.pokeSlot;
            m.key = m.pokeKey;   // HeartGold / SoulSilver: the game's own step toward POKEMON (0 elsewhere: the grid's way)
            sDir = MenuDir(&m, sDir);
        }
        SetKeys(FlyPulse(sWait) ? sDir : 0);
        return 0;
    case 3:   // A held a second frame, then the list takes input
        SetKeys(sWait == 1 ? 1u : 0);

        if (listReady) {
            fprintf(stderr, "pilot: %s: the Pokemon list at V-blank %u (mode %d, cursor %d)\n", name, now, gPortRoutePList.mode,
                    gPortRoutePList.cursorPos);
            sPhase = 4;
            sWait = 0;
            sMenuSlot = -1;
            sMenuBlocked = 0;
            sDir = 0;
        } else if (sWait > 60 && menuInput) {   // the menu took no A (a frame it did not read keys in): again
            if (++sTries > 3) {
                End(st, "stuck: F:fly: the field menu takes no A on the Pokemon entry");
            }
            sPhase = 2;
            sWait = 0;
        } else if (sWait > 600) {
            End(st, "stuck: F:fly: the Pokemon list does not open");
        }
        return 0;
    case 4:   // the cursor to the Fly Pokemon, then A
        if (!listReady) {
            SetKeys(0);
            if (sWait > 600) {
                End(st, "stuck: F:fly: the Pokemon list stopped taking input");
            }
            return 0;
        }
        if (gPortRouteFlySlot < 0) {
            End(st, "stuck: F:fly: the party lost its Fly move");
        }
        if (sWait % 6 == 1 && gPortRoutePList.cursorPos == gPortRouteFlySlot) {
            fprintf(stderr, "pilot: %s: A on party slot %d at V-blank %u\n", name, gPortRouteFlySlot, now);
            SetKeys(1);
            sPhase = 5;
            sWait = 0;
            return 0;
        }
        if (sWait % 6 == 1) {
            memset(&m, 0, sizeof m);
            m.grid = 1;
            m.slot = gPortRoutePList.cursorPos;
            m.saveSlot = gPortRouteFlySlot;
            sDir = MenuDir(&m, sDir);
        }
        SetKeys(FlyPulse(sWait) ? sDir : 0);
        return 0;
    case 5:   // A held a second frame, then the submenu (its items: 16 + move slot = that move) takes input: to Fly, A
        if (sWait == 1) {
            SetKeys(1);
            sSeen = 0;
            sAcross = 0;
            return 0;
        }
        if (!subReady) {
            SetKeys(0);
            if (sWait > 300) {
                End(st, "stuck: F:fly: no submenu after A on the Pokemon");
            }
            return 0;
        } else {
            int idx = -1, i;

            for (i = 0; i < gPortRoutePList.menuCount; i++) {
                if (gPortRoutePList.menuItems[i] == 16 + gPortRouteFlyMove) {   // POKELIST_MENU_MOVE1 + the move slot
                    idx = i;
                }
            }
            if (idx < 0) {
                fprintf(stderr, "pilot: %s: submenu items %d %d %d %d %d %d\n", name, gPortRoutePList.menuItems[0],
                        gPortRoutePList.menuItems[1], gPortRoutePList.menuItems[2], gPortRoutePList.menuItems[3],
                        gPortRoutePList.menuItems[4], gPortRoutePList.menuItems[5]);
                End(st, "stuck: F:fly: the submenu has no Fly entry");
            }
            if (sWait % 6 == 1 && gPortRoutePList.menuCursor == idx) {
                fprintf(stderr, "pilot: %s: A on Fly (submenu entry %d of %d) at V-blank %u\n", name, idx, gPortRoutePList.menuCount, now);
                SetKeys(1);
                sPhase = 6;
                sWait = 0;
                return 0;
            }
            if (sWait % 6 == 1) {   // up / down; when they bring the cursor round a column without the entry (HeartGold's two columns), right once
                unsigned bit = 1u << (gPortRoutePList.menuCursor & 31);

                sAcross = (sSeen & bit) != 0;
                sSeen = sAcross ? 0 : sSeen | bit;
            }
            SetKeys(FlyPulse(sWait) ? (sAcross ? 0x10u : idx > gPortRoutePList.menuCursor ? 0x80u : 0x40u) : 0);
        }
        return 0;
    case 6:   // A held a second frame, then the Town Map
        SetKeys(sWait == 1 ? 1u : 0);
        if (tmLive) {
            fprintf(stderr, "pilot: %s: the Town Map at V-blank %u (mode %d, %d places, cursor %d,%d)\n", name, now, tm->mode, tm->count,
                    tm->cx, tm->cy);
            sPhase = 7;
            sWait = 0;
        } else if (sWait > 400) {   // HidenEvent's check: "cannot fly here" and the list is back
            End(st, "stuck: F:fly: no Town Map (cannot fly here?)");
        }
        return 0;
    case 7: {   // the cursor to the place, A when the ring is on it
        int i, tx, ty, dx, dy, tol = tm->grid ? 0 : 2;   // Platinum's cursor is a cell of a grid: exactly on it
        unsigned keys = 0;

        if (!tmLive) {
            SetKeys(0);
            if (tm->vblank != 0 && !tm->alive) {   // the map ended: the fly event
                fprintf(stderr, "pilot: %s: the Town Map ended at V-blank %u (result %d, zone %d)\n", name, now, tm->result, tm->flyZone);
                if (tm->result != 2) {
                    End(st, "stuck: F:fly: the Town Map closed without a flight");
                }
                sPhase = 8;
                sWait = 0;
                sArrive = 0;
            } else if (sWait > 300) {
                End(st, "stuck: F:fly: the Town Map stopped");
            }
            return 0;
        }
        if (tm->moving) {
            SetKeys(0);
            return 0;
        }
        if (sTarget < 0) {
            for (i = 0; i < tm->count; i++) {
                if (tm->p[i].zone == sZone) {
                    sTarget = i;
                }
            }
            if (sTarget < 0) {
                fprintf(stderr, "pilot: %s: the map's places (zone:canFly):", name);
                for (i = 0; i < tm->count; i++) {
                    fprintf(stderr, " %d:%d", tm->p[i].zone, tm->p[i].canFly);
                }
                fprintf(stderr, "\n");
                End(st, "stuck: F:fly: the zone is no place on the Town Map");
            }
            if (!tm->p[sTarget].canFly) {
                End(st, "stuck: F:fly: the zone's town is not visited (canFly 0)");
            }
            fprintf(stderr, "pilot: %s: place %d at %d,%d (scroll %d,%d), the cursor at %d,%d\n", name, sTarget, tm->p[sTarget].x,
                    tm->p[sTarget].y, tm->sx, tm->sy, tm->cx, tm->cy);
        }
        tx = tm->sx + tm->p[sTarget].x;
        ty = tm->sy + tm->p[sTarget].y;
        dx = tx - tm->cx;
        dy = ty - tm->cy;
        if (dx > tol) {
            keys |= 0x10u;
        } else if (dx < -tol) {
            keys |= 0x20u;
        }
        if (dy > tol) {
            keys |= 0x80u;
        } else if (dy < -tol) {
            keys |= 0x40u;
        }
        if (keys == 0 && tm->hit == sTarget && tm->ring == sTarget) {   // the ring is on it: A (the pick wants the cursor's A press)
            SetKeys(sWait % 8 < 2 ? 1u : 0);
            if (!sPressed) {
                sPressed = 1;
                fprintf(stderr, "pilot: %s: A on the place at V-blank %u (cursor %d,%d)\n", name, now, tm->cx, tm->cy);
            }
        } else {
            SetKeys(keys);
        }
        if (sWait > 1500) {
            End(st, "stuck: F:fly: the cursor did not reach the place");
        }
        return 0;
    }
    case 8:   // the fly event: in the zone, the field free for 60 frames
        SetKeys(0);
        sArrive++;
        if (st->zone == (unsigned)sZone && !st->event) {
            sFree++;
        } else {
            sFree = 0;
        }
        if (sFree >= 60) {
            fprintf(stderr, "pilot: %s done at %d,%d zone %u (V-blank %u)\n", name, st->gx, st->gz, st->zone, now);
            return 1;
        }
        if (sArrive > 2400) {
            fprintf(stderr, "pilot: %s: in zone %u after 2400 frames\n", name, st->zone);
            End(st, "stuck: F:fly: the flight did not arrive");
        }
        return 0;
    }
    return 0;
}
