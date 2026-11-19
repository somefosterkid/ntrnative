// The F:teach:ITEM:SPECIES[:FORGET] token of the route pilot (fy4, 2026-11-13): a TM or HM taught from the Bag the way the player
// does it, through the game's own menus. Included by port/src/route_test.c next to route_fly.h (FieldMoveStep calls TeachStep for
// "F:teach"); test-only like the rest.
//
//   F:teach:ITEM:SPECIES[:FORGET]
//                X (the field menu), the Bag entry, A; the Bag: right to the TMs and HMs pocket, the cursor to ITEM (an item id:
//                421 = HM02 Fly), A, Use, A through its messages, Yes; the Pokemon list: the first party member of species
//                SPECIES, A; when it knows 4 moves: Yes to forgetting one, the summary's move page: the cursor to the move
//                FORGET (a move id, required then: the pilot never guesses), A, A to confirm; A through the messages; B out of
//                the Bag and the field menu; done when the field has been free 30 frames and the party member's move slots
//                hold the new move (and no longer FORGET). Black and White only (the hooks: port/extern/src/ov021/field_menu.c,
//                ov070/itemmenu.c, ov091/plist_sys.c, plist_menu.c, ov131/p_sta_skill.c).
//
// Ends the leg, with a "stuck: F:teach: ..." line, when: no party member is SPECIES; it knows 4 moves and no FORGET is given;
// it does not know FORGET; the TMs and HMs pocket has no ITEM; it already knows the move; the list does not teach it (it cannot
// learn the move: the game says so and asks nothing); the forget page takes no A (an HM move cannot be forgotten); or a menu
// does not open. The logs: "pilot: F:teach:ITEM: <phase> at V-blank N" at each phase, and the hooks' state every 60 frames
// while a phase waits.

static int TeachFresh(unsigned v, unsigned now) {
    return v != 0 && v + 3 >= now;
}

static int TeachStep(const PortRouteState *st, const char *tok, int age) {
    static int sPhase, sWait, sTries, sItem, sSpecies, sForget, sSlot, sMove, sFree, sAsked, sForgetSlot, sTotal;
    static unsigned sDir;
    static const char *sFor;
    unsigned now = OS_GetVBlankCount();
    PortRouteMenu m;
    const PortRouteBag *bag = &gPortRouteBag;
    const PortRouteSkill *sk = &gPortRouteSkill;
    int menuInput = TeachFresh(gPortRouteMenu.vblank, now) && gPortRouteMenu.input;
    int bagLive = TeachFresh(bag->vblank, now) && bag->alive;
    int listLive = TeachFresh(gPortRoutePList.vblank, now);
    int listReady = listLive && gPortRoutePList.state >= 2 && gPortRoutePList.state <= 4 && gPortRoutePList.subState == 1;
    int subReady = TeachFresh(gPortRoutePList.menuVblank, now) && gPortRoutePList.menuCount > 0;
    int skillLive = TeachFresh(sk->vblank, now);
    char name[40];
    int i;

    if (tok != sFor || age == 1) {
        const char *p = tok + 8;

        sFor = tok;
        sPhase = sWait = sTries = sFree = sAsked = sTotal = 0;
        sMove = sForgetSlot = -1;
        sItem = atoi(p);
        p = strchr(p, ':');
        sSpecies = p != NULL ? atoi(p + 1) : 0;
        p = p != NULL ? strchr(p + 1, ':') : NULL;
        sForget = p != NULL ? atoi(p + 1) : 0;
        sSlot = -1;
        sMenuSlot = -1;
        sMenuBlocked = 0;
        sDir = 0;
        gPortRouteBag.vblank = 0;
        gPortRouteBag.alive = 0;
        gPortRouteSkill.vblank = 0;
        gPortRoutePList.menuVblank = 0;
        gPortRoutePList.vblank = 0;
    }
    snprintf(name, sizeof name, "F:teach:%d", sItem);
    sWait++;
    if (++sTotal % 60 == 0 && sWait > 60) {   // a phase that waits: the hooks' state
        fprintf(stderr, "pilot: %s: phase %d wait %d: menu %d bag %d/%d pocket %d idx %d menu %d/%d list %d st %d/%d ask %d msg %d "
                "sub %d/%d skill %d cur %d conf %d\n", name, sPhase, sWait, menuInput, bagLive, bag->phase, bag->pocket, bag->index,
                bag->menuLive, bag->menuCursor, listLive, gPortRoutePList.state, gPortRoutePList.subState, gPortRoutePListTeach.ask,
                gPortRoutePListTeach.msgWait, subReady, gPortRoutePList.menuCursor, skillLive, sk->cursor, sk->confirming);
    }
    switch (sPhase) {
    case 0:   // the checks, the field free for 8 frames, then X
        if (strcmp(st->game, "B") != 0 && strcmp(st->game, "W") != 0) {
            End(st, "stuck: F:teach: written for Black and White only");
        }
        if (sItem <= 0 || sSpecies <= 0) {
            End(st, "stuck: F:teach: want F:teach:ITEM:SPECIES[:FORGET] (numbers: item, species, move)");
        }
        for (i = 5; i >= 0; i--) {
            if (gPortRouteParty[i][0] == sSpecies) {
                sSlot = i;
            }
        }
        if (sSlot < 0) {
            End(st, "stuck: F:teach: no party member of that species");
        }
        if (gPortRouteParty[sSlot][4] != 0 && sForget == 0) {
            End(st, "stuck: F:teach: it knows 4 moves: give the move to forget (F:teach:ITEM:SPECIES:FORGET)");
        }
        if (sForget != 0) {
            for (i = 0; i < 4; i++) {
                if (gPortRouteParty[sSlot][1 + i] == sForget) {
                    sForgetSlot = i;
                }
            }
            if (sForgetSlot < 0) {
                fprintf(stderr, "pilot: %s: slot %d knows moves %d %d %d %d\n", name, sSlot, gPortRouteParty[sSlot][1], gPortRouteParty[sSlot][2],
                        gPortRouteParty[sSlot][3], gPortRouteParty[sSlot][4]);
                End(st, "stuck: F:teach: it does not know the move to forget");
            }
        }
        if (st->event) {
            sWait = 0;
            PressEvent(st);
            return 0;
        }
        SetKeys(sWait >= 8 ? 0x400u : 0);
        if (sWait >= 8) {
            fprintf(stderr, "pilot: %s: X at V-blank %u, species %d in slot %d, forget %d (slot %d)\n", name, now, sSpecies, sSlot, sForget,
                    sForgetSlot);
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
                End(st, "stuck: F:teach: the field menu does not open");
            }
            sPhase = 0;
            sWait = 0;
        }
        return 0;
    case 2:   // to the Bag entry, then A
        if (!menuInput) {
            SetKeys(0);
            if (sWait > 300) {
                End(st, "stuck: F:teach: the field menu stopped taking input");
            }
            return 0;
        }
        if (gPortRouteMenu.bagSlot < 0) {
            End(st, "stuck: F:teach: the field menu has no Bag entry");
        }
        if (sWait % 6 == 1 && sWait > 6 && gPortRouteMenu.slot == gPortRouteMenu.bagSlot) {
            fprintf(stderr, "pilot: %s: A on the Bag entry (slot %d) at V-blank %u\n", name, gPortRouteMenu.slot, now);
            SetKeys(1);
            sPhase = 3;
            sWait = 0;
            return 0;
        }
        if (sWait % 6 == 1) {
            m = gPortRouteMenu;
            m.saveSlot = m.bagSlot;
            sDir = MenuDir(&m, sDir);
        }
        SetKeys(FlyPulse(sWait) ? sDir : 0);
        return 0;
    case 3:   // A held a second frame, then the Bag's list takes keys
        SetKeys(sWait == 1 ? 1u : 0);
        if (bagLive && bag->phase == 1) {
            fprintf(stderr, "pilot: %s: the Bag at V-blank %u (pocket %d, index %d of %d)\n", name, now, bag->pocket, bag->index, bag->count);
            sPhase = 4;
            sWait = 0;
        } else if (sWait > 60 && menuInput) {
            if (++sTries > 3) {
                End(st, "stuck: F:teach: the field menu takes no A on the Bag entry");
            }
            sPhase = 2;
            sWait = 0;
        } else if (sWait > 600) {
            End(st, "stuck: F:teach: the Bag does not open");
        }
        return 0;
    case 4: {   // the TMs and HMs pocket, the cursor to ITEM, A
        int t = -1;

        if (!bagLive || bag->phase != 1) {
            SetKeys(0);
            if (sWait > 300) {
                End(st, "stuck: F:teach: the Bag's list stopped taking keys");
            }
            return 0;
        }
        if (bag->pocket != 2) {   // BAG_POCKET_TMS_HMS: right from Items (0), Medicine (1)
            SetKeys(FlyPulse(sWait) ? (bag->pocket < 2 ? 0x10u : 0x20u) : 0);
            if (sWait > 600) {
                End(st, "stuck: F:teach: the Bag does not go to the TMs and HMs pocket");
            }
            return 0;
        }
        for (i = 0; i < bag->count && i < 128; i++) {
            if (bag->items[i] == sItem) {
                t = i;
            }
        }
        if (t < 0) {
            End(st, "stuck: F:teach: the TMs and HMs pocket has no such item");
        }
        if (sWait % 6 == 1 && sWait > 6 && bag->index == t) {
            fprintf(stderr, "pilot: %s: A on the item (index %d) at V-blank %u\n", name, t, now);
            SetKeys(1);
            sPhase = 5;
            sWait = 0;
            return 0;
        }
        SetKeys(FlyPulse(sWait) && bag->index != t ? (bag->index < t ? 0x80u : 0x40u) : 0);
        if (sWait > 1200) {
            End(st, "stuck: F:teach: the Bag's cursor does not reach the item");
        }
        return 0;
    }
    case 5: {   // the item's menu: the cursor to Use (action 0), A
        int t = -1;

        if (sWait == 1) {
            SetKeys(1);
            return 0;
        }
        if (!bagLive || bag->phase != 2 || !bag->menuLive) {
            SetKeys(0);
            if (bagLive && bag->phase == 1 && sWait > 60) {   // the list took no A: again
                if (++sTries > 5) {
                    End(st, "stuck: F:teach: the Bag's list takes no A on the item");
                }
                sPhase = 4;
                sWait = 0;
            } else if (sWait > 300) {
                End(st, "stuck: F:teach: no item menu after A on the item");
            }
            return 0;
        }
        for (i = 3; i >= 0; i--) {
            if (bag->menuActions[i] == 0) {
                t = i;
            }
        }
        if (t < 0) {
            End(st, "stuck: F:teach: the item's menu has no Use");
        }
        if (sWait % 6 == 1 && bag->menuCursor == t) {
            fprintf(stderr, "pilot: %s: A on Use (entry %d) at V-blank %u\n", name, t, now);
            SetKeys(1);
            sPhase = 6;
            sWait = 0;
            return 0;
        }
        SetKeys(FlyPulse(sWait) ? (bag->menuCursor < t ? 0x80u : 0x40u) : 0);
        return 0;
    }
    case 6:   // the TM's messages (A), its question (Yes = entry 0, A), then the Pokemon list
        if (listReady) {
            fprintf(stderr, "pilot: %s: the Pokemon list at V-blank %u (mode %d, move %d, cursor %d)\n", name, now, gPortRoutePList.mode,
                    gPortRoutePListTeach.move, gPortRoutePList.cursorPos);
            sMove = gPortRoutePListTeach.move;
            for (i = 0; i < 4; i++) {
                if (gPortRouteParty[sSlot][1 + i] == sMove) {
                    End(st, "stuck: F:teach: it already knows the move");
                }
            }
            sPhase = 7;
            sWait = 0;
            sMenuSlot = -1;
            sMenuBlocked = 0;
            sDir = 0;
            return 0;
        }
        if (bagLive && (bag->phase == 3 || bag->phase == 5 || (bag->phase == 0 && !bag->menuLive))) {   // its messages want A too
            SetKeys(FlyPulse(sWait) ? 1u : 0);
            if (bag->phase == 5) {
                End(st, "stuck: F:teach: the Bag will not use the item (its message, then the list)");
            }
        } else if (bagLive && bag->phase == 4 && bag->menuLive) {
            if (bag->menuCursor == 0) {
                SetKeys(sWait % 6 == 1 || sWait % 6 == 2 ? 1u : 0);
            } else {
                SetKeys(FlyPulse(sWait) ? 0x40u : 0);
            }
        } else if (bagLive && bag->phase == 1 && sWait > 120) {
            End(st, "stuck: F:teach: the Bag went back to its list without teaching");
        } else {
            SetKeys(0);
        }
        if (sWait > 1200) {
            End(st, "stuck: F:teach: no Pokemon list after the TM's question");
        }
        return 0;
    case 7:   // the cursor to the Pokemon, then A
        if (!listReady) {
            SetKeys(0);
            if (sWait > 600) {
                End(st, "stuck: F:teach: the Pokemon list stopped taking input");
            }
            return 0;
        }
        if (sWait % 6 == 1 && gPortRoutePList.cursorPos == sSlot) {
            fprintf(stderr, "pilot: %s: A on party slot %d at V-blank %u\n", name, sSlot, now);
            SetKeys(1);
            sPhase = 8;
            sWait = 0;
            sAsked = 0;
            return 0;
        }
        if (sWait % 6 == 1) {
            memset(&m, 0, sizeof m);
            m.grid = 1;
            m.slot = gPortRoutePList.cursorPos;
            m.saveSlot = sSlot;
            sDir = MenuDir(&m, sDir);
        }
        SetKeys(FlyPulse(sWait) ? sDir : 0);
        return 0;
    case 8:   // the learning: A through the messages, Yes to forgetting a move, the move page, No to giving up
        if (sWait == 1) {
            SetKeys(1);
            return 0;
        }
        {   // each change of the list's state: a line
            static int sLast = -1;

            if (listLive && gPortRoutePList.state * 100 + gPortRoutePListTeach.ask * 10 + subReady != sLast) {
                sLast = gPortRoutePList.state * 100 + gPortRoutePListTeach.ask * 10 + subReady;
                fprintf(stderr, "pilot: %s: the list's state %d (ask %d, menu %d: cursor %d of %d) at V-blank %u\n", name,
                        gPortRoutePList.state, gPortRoutePListTeach.ask, subReady, gPortRoutePList.menuCursor, gPortRoutePList.menuCount, now);
            }
        }
        if (skillLive) {
            sAsked = 1;
            if (sk->exiting) {
                SetKeys(0);
            } else if (sk->confirming) {   // the cursor went to the new move: A confirms the forget
                SetKeys(sWait % 8 < 2 ? 1u : 0);
            } else if (sk->cursor == sForgetSlot && sk->moves[sForgetSlot] == sForget) {
                SetKeys(sWait % 8 < 2 ? 1u : 0);
            } else {
                SetKeys(FlyPulse(sWait) ? (sk->cursor < sForgetSlot || sk->cursor > 4 ? 0x80u : 0x40u) : 0);
            }
            if (sWait > 1800) {
                End(st, "stuck: F:teach: the move page takes no A on the move to forget (an HM move cannot be forgotten)");
            }
            return 0;
        }
        if (listLive && gPortRoutePList.state == 9 && subReady) {   // a question: entries 14 Yes, 15 No
            int want = gPortRoutePListTeach.ask == 1 ? 14 : gPortRoutePListTeach.ask == 2 ? 15 : -1, t = -1;

            sAsked = 1;
            if (want < 0) {
                End(st, "stuck: F:teach: the list asks a question the pilot does not know");
            }
            if (gPortRoutePListTeach.ask == 2 && sForget == 0) {
                End(st, "stuck: F:teach: the list asks to give up learning (no move to forget)");
            }
            for (i = 0; i < gPortRoutePList.menuCount; i++) {
                if (gPortRoutePList.menuItems[i] == want) {
                    t = i;
                }
            }
            if (t < 0) {
                End(st, "stuck: F:teach: the list's question has no Yes or No");
            }
            if (gPortRoutePList.menuCursor == t) {
                SetKeys(sWait % 8 < 2 ? 1u : 0);
            } else {
                SetKeys(FlyPulse(sWait) ? (gPortRoutePList.menuCursor < t ? 0x80u : 0x40u) : 0);
            }
            return 0;
        }
        if (listLive && gPortRoutePList.state == 7) {   // a message: A (one that does not wait for A still wants it to scroll)
            SetKeys(sWait % 8 == 1 ? 1u : 0);   // one frame: a held A would answer the question that follows
            return 0;
        }
        if (listReady && gPortRoutePList.mode == 6 && !sAsked && sWait > 120) {
            End(st, "stuck: F:teach: the list did not teach it (it cannot learn the move?)");
        }
        if ((bagLive && bag->phase == 1) || menuInput || (!listLive && !bagLive && !st->event && sWait > 60)) {
            fprintf(stderr, "pilot: %s: the list is done at V-blank %u (bag %d, menu %d)\n", name, now, bagLive, menuInput);
            sPhase = 9;
            sWait = 0;
            sFree = 0;
            return 0;
        }
        SetKeys(0);
        if (sWait > 3000) {
            End(st, "stuck: F:teach: the learning does not end");
        }
        return 0;
    case 9:   // B out of the Bag and the field menu, the field free 30 frames, the move in its slots
        if (bagLive || menuInput) {
            sFree = 0;
            SetKeys(FlyPulse(sWait) ? 2u : 0);
        } else if (!st->event && !listLive) {
            SetKeys(0);
            sFree++;
        } else {
            SetKeys(0);
            sFree = 0;
        }
        if (sFree >= 30) {
            int has = 0, gone = 1;

            for (i = 0; i < 4; i++) {
                has |= gPortRouteParty[sSlot][1 + i] == sMove;
                gone &= sForget == 0 || gPortRouteParty[sSlot][1 + i] != sForget;
            }
            fprintf(stderr, "pilot: %s: slot %d moves %d %d %d %d (V-blank %u)\n", name, sSlot, gPortRouteParty[sSlot][1],
                    gPortRouteParty[sSlot][2], gPortRouteParty[sSlot][3], gPortRouteParty[sSlot][4], now);
            if (!has || !gone) {
                End(st, "stuck: F:teach: the move is not in its slots after the list");
            }
            fprintf(stderr, "pilot: %s done: species %d knows move %d\n", name, sSpecies, sMove);
            return 1;
        }
        if (sWait > 1200) {
            End(st, "stuck: F:teach: the Bag or the field menu does not close");
        }
        return 0;
    }
    return 0;
}
