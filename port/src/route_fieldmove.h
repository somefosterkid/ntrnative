// The F: tokens of the route pilot (tw9, 2026-11-12): field moves used the way the player does, by A on the target and Yes.
// Included by port/src/route_test.c after TalkStep, WallStep and GoStep (it uses their statics); the Pilot's token switch
// calls FieldMoveStep for every token that starts with "F:". Test-only, as the rest of route_test.c.
//
//   F:cut:UID  F:smash:UID  F:strength:UID   walk next to the actor of uid UID (a Cut tree, a Rock Smash rock, a Strength
//                       boulder: the dump's "N uid U" line), face it, A, answer the move's question Yes (the pulse key is
//                       A for the token, then back to what it was), wait for the event to end. The party needs the move:
//                       the test cheat (BWPORT_TESTCHEAT=win, the leg default) gives it (PortRoute_CheatFieldMove).
//                       The same walk as G:npc:UID with the pulse key A.
//   F:strength:UID:DIR[:N]  Strength, then push the boulder N tiles (default 1) in DIR (0 up 1 down 2 left 3 right, or
//                       U D L R): walk to the boulder's far side (next to its footprint, any of its tiles there that the
//                       player reaches: PortRouteMap_PushFrom), face it, A + Yes, then hold DIR until the BOULDER is N tiles
//                       on (its tile, not the player's: a push moves the boulder and leaves the player where he stands,
//                       EventStrengthRockPush_Callback's ACMD_STAY_WALK; a 2x2 boulder still fills the tile in front after
//                       one push, so a key held until the player moves pushed it twice). Done when the field is free.
//   F:push:UID:DIR[:N]  the same push, Strength already on (no A). A boulder that does not move for 120 frames (blocked:
//                       StrengthRockSpaceCheck wants every tile in front free or every one a hole) is "boulder UID stuck".
//                       Into a Strength hole: the boulder drops in (collision off) and the hole is floor from then on.
//   F:surf:X,Z:DIR      Surf: walk to the shore tile X,Z (land, the water or the shore's edge tile next to it in DIR), face
//                       DIR, A + Yes, wait for the start event to end (the player ends on the water, 1 or 2 tiles on).
//   F:surf              the same from the nearest land tile (by steps over the dump's walkable tiles) that faces water, or an
//                       edge tile with water beyond it. Surfing already: done at once. Then plain G:X,Z walks over the water
//                       and back onto a shore (route_map.c: the water is walkable while the player surfs; docs/route-writer.md).
//   F:stone:X,Z:DIR     Chargestone Cave's floating crystal on tile X,Z (the dump's "N uid 253": all share one uid, so the
//                       tile names it): walk to the tile behind it, hold DIR (0 up 1 down 2 left 3 right) until the push event
//                       starts, wait for it to end. The player stays put, the crystal slides to the tile in front of the next
//                       electric rock (the dump's E) in DIR, and not at all when there is none (or it is next to one already:
//                       it is then fixed) = "stone X,Z not pushed". Logs where it stopped.
//   F:waterfall:X,Z:DIR Waterfall (the global script 10015 / 0x271f, or 10006 / 0x2716 when the game asks first): the player
//                       is surfing (F:surf first), stands on the water tile X,Z and faces DIR toward the waterfall tile (the
//                       dump's F), A + Yes, wait for the climb to end (the player ends on the water above). Needs the move
//                       (the test cheat gives it) and the badge (X:badge); no badge or no fall in front = "started no event".
//   F:waterfall         the same from the nearest water tile (by steps over the dump's surf-walkable tiles) with a waterfall
//                       tile in front. Not surfing: "no path".
//   F:whirlpool:X,Z:DIR Whirlpool (HeartGold / SoulSilver: A facing a whirlpool tile, the dump's W, field_control.c ->
//                       std_field_whirlpool): the player is surfing, stands on the water tile X,Z and faces DIR toward the
//                       whirlpool, A + Yes, wait for the crossing to end (ScrCmd_Whirlpool's task moves the player on while
//                       the tile under him is a whirlpool: he ends on the first water tile past it). Needs the move (the test
//                       cheat gives it) and the badge the script checks; no whirlpool in front = "started no event".
//   F:whirlpool         the same from the nearest water tile (by steps over the dump's surf-walkable tiles) with a whirlpool
//                       tile in front. Not surfing: "no path".
//   F:climb:X,Z:DIR     Rock Climb (Gen 4, FIELD_MOVES script 3 when A faces a climb wall tile the way it climbs): walk to the
//                       land tile X,Z, face DIR toward the wall (the dump's flags PORT_MAP_CLIMBNS / CLIMBEW), A + Yes, wait for
//                       the climb to end (the game walks the player over every climb tile that way and hops it off onto the
//                       next tile: ov5_021DFB54.c SubTask_RockClimb_LoopOrHopOff). Needs the move (the test cheat gives it)
//                       and the badge the script checks; no wall in front = "started no event".
//   F:climb             the same from the nearest land tile (by steps over the dump's walkable tiles) with a climb wall in front.
//   F:fly:ZONE          Fly through the game's menus (X, the Pokemon entry, the party member that knows Fly, its Fly entry,
//                       the Town Map's cursor to the place of zone ZONE, A), then the flight; Black and White only. Its own
//                       file: port/src/route_fly.h (header: the phases and every "stuck: F:fly: ..." line).
//   F:teach:ITEM:SPECIES[:FORGET]  teach a TM or HM (ITEM, an item id: 421 = HM02 Fly) from the Bag to the first party member of
//                       SPECIES through the game's menus (X, Bag, the TMs and HMs pocket, Use, Yes, the Pokemon; with 4 moves Yes
//                       to forgetting one and the move FORGET (a move id, required then) on the summary's move page); Black and
//                       White only. Its own file: port/src/route_teach.h (the phases and every "stuck: F:teach: ..." line).
//   Flash and Dive: not written yet.
static int FmDir(const char *s) {
    switch (*s) {
    case '0': case 'U': return 0;
    case '1': case 'D': return 1;
    case '2': case 'L': return 2;
    case '3': case 'R': return 3;
    }
    return -1;
}

// F:surf without coordinates: the nearest land tile (breadth first over the pass bits from the player, '.' and ',') with
// water in front, or an edge tile with water beyond it (CheckSurfBeginTiles). 1 and the tile and the direction, or 0.
// fall (F:waterfall): the nearest water tile ('~', the pass bits carry the surfing player over the water) with a waterfall
// tile ('F') in front instead. fall 2 (F:climb): the nearest land tile with a Rock Climb wall tile in front that climbs that way.
// fall 3 (F:whirlpool): the nearest water tile with a whirlpool tile ('W') in front.
static int FmShore(const PortMap *m, int fall, int *ox, int *oz, int *odir) {
    static const int kDx[4] = {0, 0, -1, 1}, kDz[4] = {-1, 1, 0, 0};
    static unsigned short sQ[PORT_MAP_N * PORT_MAP_N];
    static unsigned char sSeen[PORT_MAP_N][PORT_MAP_N];
    int head = 0, tail = 0, d;

    memset(sSeen, 0, sizeof sSeen);
    sQ[tail++] = (unsigned short)((m->pz - m->z0) * PORT_MAP_N + (m->px - m->x0));
    sSeen[m->pz - m->z0][m->px - m->x0] = 1;
    while (head < tail) {
        int cx = sQ[head] % PORT_MAP_N, cz = sQ[head] / PORT_MAP_N;
        const PortMapCell *c = &m->cell[cz][cx];

        head++;
        for (d = 0; d < 4; d++) {
            int nx = cx + kDx[d], nz = cz + kDz[d];
            const PortMapCell *n;

            if (nx < 1 || nz < 1 || nx >= PORT_MAP_N - 1 || nz >= PORT_MAP_N - 1) {
                continue;
            }
            n = &m->cell[nz][nx];
            if (fall == 2 ? ((c->kind == '.' || c->kind == ',') && (n->flags & (d < 2 ? PORT_MAP_CLIMBNS : PORT_MAP_CLIMBEW)))
                : fall ? (c->kind == '~' && n->kind == (fall == 3 ? 'W' : 'F'))
                     : ((c->kind == '.' || c->kind == ',') &&
                        (n->kind == '~' || (n->kind == 'e' && m->cell[nz + kDz[d]][nx + kDx[d]].kind == '~')))) {
                *ox = m->x0 + cx;
                *oz = m->z0 + cz;
                *odir = d;
                return 1;
            }
            if ((c->pass & (1 << d)) && (fall == 1 || fall == 3 ? n->kind == '~' : (n->kind == '.' || n->kind == ',')) && !sSeen[nz][nx]) {
                sSeen[nz][nx] = 1;
                sQ[tail++] = (unsigned short)(nz * PORT_MAP_N + nx);
            }
        }
    }
    return 0;
}

static int FieldMoveStep(const PortRouteState *st, const char *tok, int age) {
    static const int kDx[4] = {0, 0, -1, 1}, kDz[4] = {-1, 1, 0, 0};
    static char sGo[40];
    static int sKind, sUid, sDir, sN, sPhase, sSavedPulse, sX, sZ, sGoAge, sWait, sTx, sTz, sSx, sSz, sFall, sAx, sAz, sBx, sBz, sFree;
    static const char *sFor;
    int i;

    if (strncmp(tok, "F:fly:", 6) == 0) {   // route_fly.h
        return FlyStep(st, tok, age);
    }
    if (strncmp(tok, "F:teach:", 8) == 0) {   // route_teach.h
        return TeachStep(st, tok, age);
    }
    if (tok != sFor || age == 1) {
        char word[16];
        const char *p = tok + 2;
        size_t n = 0;

        sFor = tok;
        while (*p != '\0' && *p != ':' && *p != ' ' && n + 1 < sizeof word) {
            word[n++] = *p++;
        }
        word[n] = '\0';
        if (*p == ':') {
            p++;
        }
        sKind = -1;
        sUid = sDir = sX = sZ = sPhase = sWait = sGoAge = sFall = 0;
        sN = 1;
        if (strcmp(word, "cut") == 0 || strcmp(word, "smash") == 0 || strcmp(word, "strength") == 0 ||
            strcmp(word, "push") == 0) {
            sUid = atoi(p);
            while (*p != '\0' && *p != ':' && *p != ' ') {
                p++;
            }
            if (*p == ':' && FmDir(p + 1) >= 0) {
                sDir = FmDir(p + 1);
                p += 2;
                sKind = word[0] == 'p' ? 2 : 1;
                if (*p == ':') {
                    sN = atoi(p + 1);
                }
            } else {
                sKind = (word[0] == 'p') ? -1 : 0;
            }
        } else if (strcmp(word, "stone") == 0) {   // F:stone:X,Z:DIR
            sSx = atoi(p);
            while (*p != '\0' && *p != ',' && *p != ' ') {
                p++;
            }
            if (*p == ',') {
                sSz = atoi(p + 1);
                while (*p != '\0' && *p != ':' && *p != ' ') {
                    p++;
                }
                if (*p == ':' && FmDir(p + 1) >= 0) {
                    sDir = FmDir(p + 1);
                    sKind = 4;
                }
            }
        } else if (strcmp(word, "surf") == 0 || strcmp(word, "waterfall") == 0 || strcmp(word, "climb") == 0 ||
                   strcmp(word, "whirlpool") == 0) {   // the same walk, A and Yes: a kind 3
            sFall = word[0] == 'w' ? (word[1] == 'h' ? 3 : 1) : word[0] == 'c' ? 2 : 0;
            if (*p == '\0' || *p == ' ') {   // no coordinates: the nearest shore (waterfall), chosen when the walk starts
                sKind = 3;
                sDir = -1;
            }
            sX = atoi(p);
            while (*p != '\0' && *p != ',' && *p != ' ') {
                p++;
            }
            if (*p == ',') {
                sZ = atoi(p + 1);
                while (*p != '\0' && *p != ':' && *p != ' ') {
                    p++;
                }
                if (*p == ':' && FmDir(p + 1) >= 0) {
                    sDir = FmDir(p + 1);
                    sKind = 3;
                }
            }
        }
        if (sKind < 0 || sN < 1) {
            fprintf(stderr, "pilot: bad token: %.*s: want F:cut|smash|strength:UID[:DIR], F:push:UID:DIR[:N], F:surf|waterfall|whirlpool|climb[:X,Z:DIR]\n",
                    (int)(NextTok(tok) - tok), tok);
            End(st, "stuck: bad token");
        }
        sSavedPulse = (int)sPulse;
        sPulse = sKind == 4 ? sPulse : 1;   // A: the move's question is answered Yes (a stone is pushed by walking: no A)
        if (sKind == 0) {
            snprintf(sGo, sizeof sGo, "G:npc:%d", sUid);
        }
    }
    if (sKind == 0) {   // the same as G:npc:UID
        if (GoStep(st, sGo, ++sGoAge)) {
            sPulse = (unsigned)sSavedPulse;
            return 1;
        }
        return 0;
    }
    if (sPhase == 0) {   // the tile to stand on
        if (!PortRouteMap_Get()->ok) {
            return 0;   // no field yet (the first frames of a leg from a save): wait for it
        }
        if (sKind == 3) {
            const PortMap *m = PortRouteMap_Get();

            if (sDir < 0) {   // F:surf, F:waterfall alone
                if (sFall == 0 && m->ok && m->surf) {
                    fprintf(stderr, "pilot: %.*s: already surfing\n", (int)(NextTok(tok) - tok), tok);
                    sPulse = (unsigned)sSavedPulse;
                    return 1;
                }
                if ((sFall == 1 || sFall == 3) && !m->surf) {
                    GoFail(st, tok, "not surfing (F:surf first)");
                    return 0;
                }
                if (!m->ok || !FmShore(m, sFall, &sX, &sZ, &sDir)) {
                    GoFail(st, tok, sFall == 2   ? "no land tile with a climb wall in front near"
                                    : sFall == 3 ? "no water tile with a whirlpool in front near"
                                    : sFall      ? "no water tile with a waterfall in front near"
                                                 : "no shore (land with water in front) near");
                    return 0;
                }
                fprintf(stderr, "pilot: %.*s: the %s is %d,%d facing %d\n", (int)(NextTok(tok) - tok), tok,
                        sFall == 2 ? "climb's foot" : sFall ? "water tile" : "shore", sX, sZ, sDir);
            }
        } else if (sKind == 4) {
            const PortMap *m = PortRouteMap_Get();

            for (i = 0; m->ok && i < m->nnpc && (m->npc[i].x != sSx || m->npc[i].z != sSz); i++) {
            }
            if (!m->ok || i == m->nnpc) {
                GoFail(st, tok, "no crystal on that tile (the dump's N lines)");
                return 0;
            }
            sUid = m->npc[i].id;
            sX = sSx - kDx[sDir];   // the far side from the push
            sZ = sSz - kDz[sDir];
        } else {
            const PortMap *m = PortRouteMap_Get();

            for (i = 0; m->ok && i < m->nnpc && m->npc[i].id != sUid; i++) {
            }
            if (!m->ok || i == m->nnpc) {
                GoFail(st, tok, "no actor of that uid here");
                return 0;
            }
            // the far side from the push, past its footprint (x .. x + w - 1, z - h + 1 .. z: a 2x2 boulder), the tile of it
            // the player reaches first
            if (sKind == 1 || sKind == 2) {
                PortRouteMap_PushFrom(m, i, sDir, &sX, &sZ);
            } else {
                sX = m->npc[i].x;
                sZ = m->npc[i].z;
            }
        }
        snprintf(sGo, sizeof sGo, "G:%d,%d", sX, sZ);
        sPhase = 1;
        sGoAge = 0;
        return 0;
    }
    if (sPhase == 1) {   // walk there
        if (GoStep(st, sGo, ++sGoAge)) {
            sPhase = 2;
            sWait = 0;
            if (sKind == 4) {
                sPhase = 6;   // the key held into the crystal
            } else if (sKind == 2) {
                sPhase = 4;   // no A
            } else {
                TalkStep(st, tok, sDir, 1, 1);
            }
        }
        return 0;
    }
    if (sPhase == 2) {   // face it, A, Yes, the event's end
        if (TalkStep(st, tok, sDir, 1, 0)) {
            if (sKind == 3) {
                fprintf(stderr, "pilot: %.*s done at %d,%d zone %u (V-blank %u)\n", (int)(NextTok(tok) - tok), tok, st->gx,
                        st->gz, st->zone, (unsigned)OS_GetVBlankCount());
                sPulse = (unsigned)sSavedPulse;
                return 1;
            }
            sPhase = 4;
        }
        return 0;
    }
    if (sPhase == 6) {   // a crystal: the key held into it until the push event starts
        if (st->event) {
            SetKeys(0);
            sPhase = 7;
            sWait = 0;
        } else if (++sWait > 120) {
            char why[96];

            SetKeys(0);
            snprintf(why, sizeof why, "stone %d,%d not pushed (no electric rock in reach that way, or fixed by one)", sSx, sSz);
            GoFail(st, tok, why);
        } else {
            SetKeys(kDirKey[sDir]);
        }
        return 0;
    }
    if (sPhase == 7) {   // the push event runs; done when the field has been free for 8 frames
        if (st->event) {
            sWait = 0;
        } else if (++sWait >= 8) {
            const PortMap *m = PortRouteMap_Get();
            int ex = sSx, ez = sSz;

            for (i = 0; m->ok && i < m->nnpc; i++) {   // the crystal: the same uid, on the line the push went
                int dx = m->npc[i].x - sSx, dz = m->npc[i].z - sSz;

                if (m->npc[i].id == sUid && (dx != 0 || dz != 0) && (dx == 0 || dz == 0) &&
                    dx * kDx[sDir] + dz * kDz[sDir] > 0) {
                    ex = m->npc[i].x;
                    ez = m->npc[i].z;
                }
            }
            fprintf(stderr, "pilot: %.*s done: crystal %d,%d now at %d,%d, player %d,%d (V-blank %u)\n", (int)(NextTok(tok) - tok),
                    tok, sSx, sSz, ex, ez, st->gx, st->gz, (unsigned)OS_GetVBlankCount());
            sPulse = (unsigned)sSavedPulse;
            return 1;
        }
        return 0;
    }
    {   // phase 4: the push starts; 5: the key held until the boulder is N tiles on; 8: let go, the field free 20 frames
        const PortMap *m = PortRouteMap_Get();
        int ax = 0, az = 0, found = 0;

        for (i = 0; m->ok && i < m->nnpc; i++) {
            if (m->npc[i].id == sUid && !m->npc[i].rail) {
                ax = m->npc[i].x;
                az = m->npc[i].z;
                found = 1;
                break;
            }
        }
        if (sPhase == 4) {
            if (!found) {
                return 0;   // the field after the Strength event: the map again next frame
            }
            sAx = sBx = ax;
            sAz = sBz = az;
            sTx = ax + kDx[sDir] * sN;
            sTz = az + kDz[sDir] * sN;
            sWait = sFree = 0;
            sPhase = 5;
        }
        if (sPhase == 5) {
            if (found && ax == sTx && az == sTz) {
                SetKeys(0);
                fprintf(stderr, "pilot: %.*s: boulder %d from %d,%d to %d,%d\n", (int)(NextTok(tok) - tok), tok, sUid, sBx, sBz, ax,
                        az);
                sPhase = 8;
                return 0;
            }
            if (found && (ax != sAx || az != sAz)) {
                sAx = ax;   // a push on the way: the stuck count again
                sAz = az;
                sWait = 0;
            } else if (!st->event && ++sWait >= 120) {
                char why[96];

                snprintf(why, sizeof why, "boulder %d stuck at %d,%d (key %d held 120 frames from %d,%d)", sUid, ax, az, sDir, st->gx,
                         st->gz);
                GoFail(st, tok, why);
                return 0;
            }
            SetKeys(kDirKey[sDir]);
            return 0;
        }
        SetKeys(0);
        sFree = st->event ? 0 : sFree + 1;
        if (sFree >= 20) {
            fprintf(stderr, "pilot: %.*s done at %d,%d zone %u (V-blank %u)\n", (int)(NextTok(tok) - tok), tok, st->gx, st->gz,
                    st->zone, (unsigned)OS_GetVBlankCount());
            sPulse = (unsigned)sSavedPulse;
            return 1;
        }
    }
    return 0;
}
