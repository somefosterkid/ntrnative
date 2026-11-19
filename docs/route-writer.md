# Route writer quickstart

Read this instead of docs/port-worker.md "Routes" and the long writer rules. It is enough to write legs; the long docs are
for details (links at the end). A route is a list of legs in `tools/routes/GAME.txt` (GAME: b w b2 w2); each leg starts from
a save in the library (`$BWPORT_SAVES`; its `README` is the index), plays pilot tokens headless with the
test cheat on (battles are won), and ends by saving a new save for the next leg. Goal: a game scripted start to finish,
legs short (one area or one scene), every leg repeatable.

## Setup (once per worktree)
`bash tools/setup_worktree.sh`, then run the commands the way your brief says (a runner for your worktree); scripts for anything
with quotes or `$vars`. The first route run builds the port (1-2 min). One `route.py` at a time per worktree; headless only.
Never print dumps or logs whole: `grep -n`, `sed -n 'A,Bp'`.
The pilot's code (port/src/route_test.c, route_map.c, port/include/port_route*.h): read `python3 tools/route_index.py` first (every
function, type and token handler with its line and first comment, the token table, the G: goal words: about 200 lines), then only the
line ranges you need. One file: `route_index.py route_map.c`; one token with where its functions are: `route_index.py --token W`.

Leg tries on another machine (rrm1): add `--remote auto` (or `laptop`, `bd`, `local`) to any route.py command (look, add, legs,
--chain; tools/route_remote.py). It snapshots the worktree (HEAD + uncommitted changes + new files git does not ignore, through a
temporary index: your index and files are untouched; submodule contents are not shipped), runs the same command on the first free
slot of that machine and brings back what a local run leaves: the output (paths turned into yours), the route-file edit an `add`
wrote, RUN_WORK's new files (logs, map dumps, `--shot` pngs), the stored saves and README lines (into the save library). The last line
names the machine, host and compiler and the seconds. A leg's `--repeat` / `add` runs all happen in that one run on one machine; HG/SS
frames differ between clang 18 and 21 machines, so compare a leg's frames only with runs on the same one. `$BWPORT_ROUTE_RUNNER` is the
slot runner; `$ROUTE_REMOTE_WAIT` (default 900 s) how long it queues for a slot. HG/SS have one slot per machine: behind a chain
the wait can be long (exit 3 when it ran out: run it here).

## The one command: `route.py GAME add`
    python3 tools/route.py b add "gym-out | b-badge3 | Pa G:warp:31 Wf S W30 | ? | b-r4-0" \
        --desc "Black: Castelia, out of the gym" --comment "why this leg is written the way it is"
Fields: `name | start save (or NEWGAME) | tokens | end | new save (- for none) [| flags]`. End `?` means: learn it (the position
the leg reached; check that is where you meant). It writes the leg (and its `--comment` lines) into the route file (an existing
leg of that name is replaced), runs it, repeats it twice, and when the three runs are SAME stores the save and its
`README` line (`--desc` is the text: where the player is and what just happened, one line, no game text). It prints at most
8 lines:

    b tw1-gym: ok zone 31 21,48 3s v=1654 (added)
    story: 5 changed: flag 0x0320 = 1; work 0x40af = 1 (was 0); ...      <- what the leg changed (story flags/work)
    repeat x3 SAME zone 31 21,48 v=1654 save=32a858b2b2
    stored b-tw1-b.bin + README line
    end dump: .../mapdump-01-end.txt                                     <- read this to write the next leg

A failure prints the verdict line, the pilot's one-line reason and `FAILED (verdict): dump PATH`. `--from-file SPEC` runs
many legs in order and stops at the first failure: one leg a line (same syntax; `#` lines above a leg become its comments,
a `desc: text` line above it is its README text). A leg whose end you already know: write it instead of `?` (checked, not
learned). An existing save is never overwritten (`NOT STORED: ... exists`: use a new name, `-` while you iterate, or
`--replace` for b2-*/w2-* test saves). Save names: prefix `b-`, `w-`, `b2-`, `w2-`.

Gen 4 (Platinum, `route.py pt`; a talk by hand: `G:npc:UID:nota Wf:20 Ka:5` then `Pn`/`Py`/`Pa`: the A right after the walk is
lost without the settle; G:npc without :nota waits the whole event out, past a battle if one follows); a leg from a save starts in the field (route.py's SAVE_START passes the title first: never
write it in a leg); S, Py and Pn work as on Gen 5 (docs/platinum.md "Route chain": what is not done yet, Pl:N, Pk, battles).
`route.py pt look X.bin "TOKENS"` explores from a leg's save (RUN_WORK/pt-LEG.save.bin); story.py has no Gen 4 state yet: the
map's scripts are build/pokeplatinum/res/field/maps/<map>/scripts.s (their names say the scene; never quote their text).
Exploring a new Gen 4 stretch is cheapest from the previous leg's save: `route.py pt look /path/save.bin "TOKENS"` (a .bin path, tokens as one
quoted argument) and the map's scripts.s labels together tell the story, before a leg is written (j7). Porting S (SaveStep) to another game
needs no new pilot code: the game publishes PortRouteState's `save` (-1 no save running, 0 at the questions, 11 written; route_test.c SaveStep
reads st->save and nothing else of it) and gPortRouteMenu (input, the cursor and the SAVE slot); Platinum's hooks are in docs/platinum.md "Route chain".
Platinum battles play as Gen 5's (A through, the move rule, No at the forget-move / switch questions; the battle menus take
keys, no taps): docs/platinum.md "Battle pilot". Platinum has Gen 5's test cheat (on unless --no-cheat: the player moves first,
always hits, uses no PP, one-shots, takes no damage) and the no-encounter switch (on unless the leg's `encounters` flag; E:on /
E:off): docs/platinum.md "Test cheats". C:ball / Cc not yet.
HeartGold / SoulSilver (`route.py hg`, `route.py ss`: one exe, tools/routes/hg.txt and ss.txt): the same adapter, S, Py / Pn and
SAVE_START (docs/hgss.md "Route tool"); zones are pret's MAP_* numbers (build/pokeheartgold/include/constants/maps.h); a new
game's intro passes with `Pa Mz:64` (frame-timed A's fall behind the text); the X menu has no SAVE before the mother's scene
downstairs; no battle pilot, test cheats or no-encounter hook yet.
Gen 4 field notes (ptleg14): a deep-mud tile is left by the pilot by itself (PortRouteState.mud: it turns until the task ends, ~42 frames); a `G:` to a target outside the loaded
chunks wanders (hop inside the grid; `look --full-map SAVE TOKENS` shows `?` where nothing is loaded, the option goes before the save); route trainers' battles run ~4000 frames, so give
the walk `To:12000`; a scene that starts one tile before its trigger is walked on to the tile (docs/platinum.md, the Celestic Town scenes).
Gen 4 field notes (ptleg15): `F:surf` works on Platinum (the adapter reports the surfing state; `G:` then walks the water and lands on a shore: Route 218's lake is leg cel14); a walker cannot climb a bike slope but walks down it: `G:` to its top, then `KD:40` (slopes are drawn as walls); a long `G:` hop plans round a beaten trainer who still stands in a lane, so go back by the hops the forward legs took (their `waypoint` lines in the leg's log); the Canalave gym's collision is not in the grid yet (docs/platinum.md, ptleg15).
Gen 4 field notes (ptleg16): a dynamic feature's collision (the gyms with moving parts) is in the grid now (route_gen4.c Pr4_DynTile); a moving platform is a PORT_MAP_STEPON cell: `G:x,z` onto its tile rides the player, `Wf:40` waits the ride out, the next `G:` plans on the new floor; work out the order of rides with a small search over the platform tables first (docs/platinum.md, ptleg16); a long single scene with the player standing trips the hang check at 3000 frames: put `To:15000` before its `Wf`.
Gen 4 field notes (ptleg17): a leg that passes from a library save can fail only in the full chain: the chain's own saves differ in the RNG, so a trainer who turns at random may see the player (and then stands in a one-tile door: "no path ... blocked by uid N"); replay it with `route.py pt look COPY_OF_CHAIN_SAVE "TOKENS"` (the save from the run's chain-saves directory) and shift the arrival with an extra `Wf:N` (docs/platinum.md, ptleg17); a bike slope needs the bicycle at speed 3 (`Bk Ky:4`, a run-up): not solved yet.
Gen 4 field notes (ptleg18): the bicycle climbs a bike slope only in its fast gear (B toggles it; the default gear never builds the speed): the planner has a slope edge and the pilot presses B itself, so `Bk Ky:4 Wf:60` then `G:warp:N` / `G:X,Z` through a slope just works (a walker: "no path ... a bike slope at X,Z"); a trainer who turns at random is waited out at the edge of his line of sight (the pilot logs "waiting: the next tile is on a trainer's line of sight"), no extra Wf needed; the map dump's actors line shows "sees <way> N tiles" (docs/platinum.md, ptleg18).
Gen 4 field notes (ptleg19): a rider on the bicycle is refused the Route 212 mud (a step into it never happens: "G: no step", then a long wandering plan): put the bicycle down (`Ky:4 Wf:60`) before it; a hop's target east of x+36 or south of z+36 is outside the loaded window and wanders (look first with `--full-map`: the dump's ',' is tall grass, the mud shows as '.', `BWPORT_MAPDUMP_H=x0,z0,x1,z1` prints heights and tile behaviors); a floor tile 32 higher than its neighbor is "unreachable"; a warp whose tile behavior faces a direction needs the step that way (`G:x,z KL:30` onto the shore tile of Lake Valor's door, not a G to the warp tile); until the Galactic leave Lake Valor its door leads to the DRAINED map (zone 314, walked on foot), not Surf (zone 315).
Gen 4 field notes (ptleg20): a rider is put down by the pilot itself before a tile the bicycle is refused (mud, snow, very tall grass: the planner says so, the pilot presses Y once the player stands still; the leg then walks on foot: no `Ky:4 Wf:60` by hand); the way back west from the Valor Cavern to Lake Verity is the chain's own road reversed (legs wst1 .. wst9: the same hops and `G:warp:N` doors); a lake's low-water or full map is picked by a flag in its lakefront's OnLoad (read the SetWarpEventPos pairs: which warps stay), and the zone ids of the dump's "D0>zN" lines are what `G:warp:N` takes (docs/platinum.md, ptleg20).
Gen 4 field notes (ptleg21): a walk UP the Cycling Road keeps a key down at every corner (the bridge pushes a rider south in any step decision with no key; the pilot does it by itself on the bicycle: Bk Ky:4 first); the road is entered by its gate hall (`G:warp:351` then `G:door:2` for the north door, `G:warp:80`); a cave rock and boulders are F:smash:UID / F:strength:UID:DIR:N (Mt. Coronet, zone 218: legs ver5, ver6); a G: to a target outside the loaded grid says "unreachable ... in the loaded grid" or wanders: hop inside it (B1F x 2-3, Route 217 290,287 / 290,272 / 304,268); Lake Acuity needs Rock Climb (the Icicle badge, Snowpoint gym) and the pilot has no F:climb yet (docs/platinum.md, ptleg21).
Gen 4 field notes (ptleg23): a slide with speed smashes a snowball and goes on (the planner knows it, slide by slide: plan a sliding puzzle whose way needs earlier smashes as one `G:X,Z` hop per slide, from a small search over (tile, broken set)); `F:climb` climbs a Rock Climb wall (the dump's C) like F:surf: walk, A, Yes (docs/platinum.md, ptleg23).
Gen 4 field notes (ptleg24): `F:fly:ZONE` works on Platinum (route_fly.h; ZONE = the map header id, Veilstone 132): the lead or whoever knows Fly, through the field menu, the party menu, the Town Map (the test cheat adds the Fly entry for slot 0 when nobody has the move); it lands on the town's fly spot and the next token can walk at once (`F:fly:132 Wf:60 S`); only towns the save has visited are places (docs/platinum.md, ptleg24). HG/SS (hgfly1): `F:fly:ZONE` the same way through the touch menu's icons, the party menu and the Pokegear's fly map (ZONE = the map id, Violet 73), but the Fly user needs Fly in a real move slot (no cheat entry); `F:surf`, `F:smash`, `F:strength`, `F:waterfall` play there too (docs/hgss.md "F:fly on HG/SS"); a talk with a yes/no that you want answered Yes is `G:npc:N` after a leading `Pa` (no Py).
Gen 4 field notes (ptleg31): a town of bridges (Sunyshore) is two or three height layers; the planner reads each tile at the height of the player, so a `G:` stays on its layer and the ramps between layers (steps of 16) are hops of their own (`BWPORT_HDUMP=x0,z0,x1,z1,V` prints a box's heights at ten asked heights: a tile with two layers changes between asks); a gear gym (Sunyshore) needs `python3 tools/pt_sunyshore_gym.py ROOM STATE SX,SZ GX,GZ [NPC tiles]` for the hops (docs/platinum.md, ptleg31); a trainer's lane the planner refuses is crossed by `G:npc:UID` him first.
Gen 4 field notes (ptleg32): a surfing plan stays on water (a land patch inside a lake ends the swim: hop to its shore, `F:surf` again, hop on; a `G:warp:N` / far hop toward a target outside the window picks the reachable tile nearest it, and that may be a land patch); `F:waterfall` works on Platinum (the adapter marks the cascade tiles F): from the water in front of it, no coordinates; a pocket of water shut in by walls is a trap for a far hop (read `look --full-map` and flood the water first).
Gen 4 field notes (ptleg34): a cave the layer dump calls closed may be joined by water: check the floors' behaviors for sea (0x15) and cascades (0x13) before a search (Victory Road B1F's two cascades are the way north); a game that resets itself (the end of the credits) is waited out by `Wr`, then `Mz:ZONE` passes the new boot's title and CONTINUE (`To:60000 G:warp:185 To:100000 Wr To:30000 Mz:415`; docs/platinum.md, ptleg34).

Planner note (pil2): the tile a `G:npc` / `G:warp` leg ends on is a tie-break between equal-cost goal tiles (the search's heap is not stable), and the legs after it start from that tile's save: a planner change that only adds or re-keys a heap push (a costed warp tile the search merely touches) can end a leg a tile over (lobbyfan, 22,10 -> 21,9) without any path changing. Compare the leg's end tile and frames with the build before when you touch `Search` (`route.py pt` all legs, frames per leg); port/tests/route_plan_test.c "a warp the search touches" pins one. An equal-length path round a trainer's sight line or through it differs by a whole battle (fantina 17348 vs 22996 frames, r209a 20314 vs 13194): not a retry, the other of two equal paths.

Waypoint note (pil3): a `G:X,Z` waypoint is never a tile a trainer can step onto to meet the player. A trainer who looks along a line (movement look-north-and-south, the dump's "sees ... N tiles") turns on the game's RNG, and a chain's save carries an RNG that depends on every earlier leg's frames: when an earlier leg's path changes, the same leg meets him facing the other way. Platinum's ver53 (Sunyshore gym room 3) had `G:16,22` on the tile just above the trainer at 16,23: when he saw the player at 16,21 he walked to 16,22 and fought there, and the leg stood at 16,21 with its waypoint occupied ("no step" x12, stuck), in the chain but not from the library save. The planner was not at fault (the same save sticks with the warp cost off): the hop now ends on 17,22, off his line, reachable with the trainer beside it or not. A leg passes a trainer's line of sight in both outcomes (fight or no fight), and it may not stop on a tile he reaches.

Exit-column note (bk1): a multi-tile exit (a gate's door row, three tiles wide) lands the player on the column it was entered, and which tile of the warp the planner walked into follows the actors in the way, i.e. the chain's RNG: Black's town16-route4 ended one tile off in the chain (430,598) and on the target from its library save (431,598). A leg that ends right after such a warp finishes with a plain `G:X,Z` to the end tile (after the settle), so the end and the next save are the same either way.

## Looking first, and the touch screen
`python3 tools/route.py GAME look SAVE [TOKENS]` (or `route.py GAME add --look SAVE [TOKENS]`; tools/route_add.py): a look leg that is
never written to the routes file. It starts from SAVE (a library name or a .bin path), plays TOKENS (none: just the start), writes the
map dump where it stops and prints it (the window around the player, warps, triggers, BG events, actors, the matrix's zones, the story
section) and `story.py GAME next` for that zone. story.py reads a save's flags, so it reads SAVE's: `--save-end` ends the look with the
field menu's S (in the run's own copy; the library is untouched) to read the state the tokens left. Several tokens: one quoted argument.
`T:X,Y[:N]` taps the DS bottom screen at pixel X,Y (0..255, 0..191) for N frames (default 4; then 2 frames up so the game sees the lift);
`T:X,Y>X2,Y2:N` drags from the first point to the second over N frames. It presses nothing else, whatever runs. It is for buttons only the
stylus presses (a scene's OK button on the touch screen): `route.py GAME LEG --shot N` (N = a V-blank; the PNG is
build/route-cache/route-work/GAME-LEG.png) shows the screen to read the button's pixel from (the touch screen is the lower 256x192
half; picture y minus 192). The tap sets libntr's `bwport_touch_force` (libntr-touchforce.patch) which replaces the mouse in the input
tick; checked with BWPORT_TOUCHLOG (tp lines: touch 1 X Y for N V-blanks, then 0).

## Leg tokens (full list: the header of port/src/route_test.c)
Start every leg with `Pa` (pulse key A through text; the default B cancels script menus) and end with `S W30` (save, wait).
- `G:warp:Z` walk to the door/edge leading to zone Z; `G:X,Z` to a tile; `G:zone:N` the warp hops from here to zone N
- `G:door:I` walk to door I of this zone (the dump's `DI`, its index in the warp list) and through it; done when the zone
  changes. For caves whose floors are several rooms of one zone (Twist Mountain, Victory Road): `G:warp:Z` takes the nearest
  door to Z, often in the wrong room. A "no path" waits up to 4 s while an actor walks (a trainer pacing a one-tile lane)
  (atlas path; after any other movement token write `G:zone:A-N`). All plan around actors/walls, across zone edges.
- `G:npc:UID` walk to the actor, face it, A, wait out the talk/battle (a trainer is one token; a yes/no after it is
  already Yes; `:nota` = walk and face only). `G:item:UID`. `G:bg:N` sign/PC/hidden item + A. `G:trig:N` onto scene trigger
  TN. A target beyond the planner's 128 x 128 window (Driftveil Gym's Clay, 70 tiles on) is walked to in hops
  (G:npc/item/bg/trig/X,Z; proven 11-13: `G:npc:7 Wf:600` from the gym's entrance). A failed one says "unreachable" when
  the planner saw the target and no way leads there, "no tile nearer reachable" when a hop ends in a dead end.
  `G:wall:UID` through a pass-through actor (gym walls: the cell before it, key held past it). `G:rail:L,F,S` rail place.
  `G:ride:N` ride N of the game's ride table (Black 2/White 2 Castelia Gym cocoons: N = sRides' index in pokebw2's
  ov091/gym_insect.c): its start tile on its floor, the key held into it, the ride; logs `landed at X,Z y Y` (the next G plans there).
  Humilau Gym (Black 2/White 2, zone 473; N = pokebw2's gym_humilau.c sRides index, the spot is the start tile, the game wants a
  platform two tiles ahead): the whole gym from the door (zone 465's trigger has set work 0x411f) is
  `G:ride:35 G:ride:30 G:ride:25 G:ride:23 G:ride:26 G:ride:15 G:ride:11 G:ride:9 G:ride:2 G:ride:8 G:ride:9 G:ride:36 G:npc:0 Wf:900`
  (proven 11-13 from a test warp: 3 Ace Trainers fought on the way, lands at 16,13, Marlon's script runs); a platform that is
  not where the table expects just bumps ("ride N did not start").
  Mistralton Gym (Black 2/White 2, zone 108; a gust about every 120 field frames blows the player south to the next wall): any
  G token there walks in hops from shelter to shelter (a tile the gust does not move him off) and waits in one for the gust
  (logs `wind: to the shelter X,Z` / `wind: in the shelter`; port/src/route_slide.c). Proven 11-13 from b2-gym6-1 and
  w2-gym6-1: `Pa G:trig:0 Wf:60 G:npc:3 Wf:900` (5 gusts sat out, one gym trainer fought on the way, Skyla beaten, ~11800 V-blanks).
  Opelucid Gym (Black 2/White 2, zone 121; statues move only by script: each step onto a live trigger tile T0..T31 lifts the
  player to the next platform, no table needed): the whole gym from the door (proven 11-13 on Black 2, ~25000 frames):
  `G:warp:121 Wf:300 G:15,46 KU:300 Wf:30 G:npc:1 G:trig:32 Wf:60 G:trig:2 Wf:60 G:trig:22 Wf:60 G:npc:3 G:trig:11 Wf:60
  G:trig:20 Wf:60 G:trig:26 Wf:60 G:npc:4 G:trig:13 Wf:60 G:trig:24 Wf:60 KU:40 Ka:4 Wb Wf:300` (the corridor's rail needs the
  held Up from 15,46; Veteran 1 fight, button T32, up to tier 1, right: Veteran 3 comes, back left, up to tier 2, right: Veteran 4,
  back left, up to the Leader's tier; a G:npc:6 does not plan there: the planner blocks on Veteran 1 at 15,19 on another
  floor, so the last walk is Up held and A). A veteran beaten on a platform that comes to the player counts for the tier
  only after the move back; G:trig:N finishes when its scene (the lift) has ended, wherever it left the player. A scene that
  starts one tile before the trigger (the walk's waypoint is the trigger tile) first gets the older behaviour: the pilot walks on
  to the trigger tile; only when no plan reaches it (the lift put the player on another platform) the token ends in place.
  only after the move back; G:trig:N finishes when its scene (the lift) has ended, wherever it left the player.
  The lifts are planned too (pz7, 11-13): the adapter lists each platform's open lifts as levers that move the player to the
  next platform's centre (port_route_map_game.h PortMap_OpelucidLifts: the gym's state tier * 3 + column is the switches'
  state, a lift runs only in its platform's state), so any G token rides them: out of the gym from the Leader's tier is
  `Pa Wf:300 G:warp:120 Wf:300` (three lifts down, the corridor rail, the door; leg b2 gym7-out). The platform centres are
  a table by tier and column (kOpelucidHub), from the climb. The climb itself is still written by triggers (the buttons and
  the Veterans are story, not lifts).
  Pokemon League (Black 2/White 2, zones 136..146; proven 11-13 on Black 2 from b2-r5-4 up to Iris's win, ~43000 frames): `Pa` first
  (the guard's yes/no loops with the default B). `X:badge:7 X:mon:94:70:1 X:warp:136:30:49:0 Wf:60 Pa G:warp:137 Wf:200`, then per room in the
  story's order 140, 142, 141, 143: `G:warp:R Wf:200 G:15,26 KU:400 Wf:30 G:npc:0 Wf:600 G:trig:1 Wf:600` (the exit scene puts the player
  on the hall's grid at 31,48; the four doors are on four rail lanes entered from the grid's switch tiles, and G:warp:R picks the switch tile
  whose lane has door R). The statue `G:bg:0 Wf:900` (all four beaten: the lift to zone 138), `G:warp:144 Wf:300 G:trig:4 Wf:900 G:npc:0 Wf:900`
  = Iris. The route's legs (vr-north, lg-gate .. lg-iris, fx5 11-13) play it with the real party (no X:mon lead: bt1's move choice
  picks Ember against the Ghost room), through the Hall of Fame and the credits to the reset (zone 428, b2-champ).
  Victory Road's zone 592/596 scenes (work 0x4123) are POST-GAME: only global script 9601 (FieldScript_CallPlayerPostHOFSetup, the
  Hall of Fame setup; it also sets the game-clear flag 0x960) sets 0x4123 = 1. Before the League the way is zone 592's north door
  W5 -> 228 -> 214 (leg vr-north). Before you trust "script N sets it", check who RUNS the global script (grep its id in hex,
  0x2581, in the decomp's C), not a guess from the flags it sets.
  Mistralton to Undella to Opelucid (Black 2 / White 2, gm14, proven 11-13 from b2-gym6-1 / w2-gym6-1; a test save of Skyla's win is `Pa G:trig:0 Wf:60 G:npc:3 Wf:900 Pa Wf:300`):
  `Pa G:warp:107 Wf:300 G:zone:107-338 Wf:60 G:npc:0 Wf:600` (Route 7; the Celestial Tower's first-floor actor is the only step there: it makes work 0x40c2 = 4), then
  `Pa G:zone:338-111 Wf:60 G:npc:4 Wf:900` (the plane, lands in Lentimas 458), Reversal Mountain `Pa G:zone:458-412 Wf:600` on Black 2 AND White 2 (White 2's interior is the
  other zone set 537/539/542: the entity file has a warp into each version's interior on the same door tile, a zone's init script moves the other version's off it, and the atlas
  marks those warps dead for the game it was built for: tools/story.py version_dead_warps, `atlas.py w2 zone 461` shows DEAD; tools/route.py WAYPOINTS adds the two waypoints
  `G:7,52 G:40,57` White 2's 539 needs; proven 11-13 on both, ~16000 / ~24000 frames). Undella
  `Pa G:trig:0 Wf:600` (Hugh), Route 13 `Pa G:zone:412-370 Wf:60 G:trig:1 Wf:600`, Lacunosa `Pa G:zone:370-406 Wf:60 G:trig:0 Wf:600`, Route 12 (a lake: the walk loops
  without Surf) `Pa G:warp:406 Wf:300 G:zone:406-255 Wf:60 F:surf G:zone:255-120 Wf:60`. The Marine Tube (zone 414) stays shut until Opelucid sets work 0x411e. A `G:warp`
  that loops 20000 frames between two tiles needs a waypoint `G:X,Z` (White 2 Reversal Mountain) or `F:surf` first. Details: C:\stress\gimmicks\b2w2.md "gm14".
- `To:N` before a token: that token may run N frames before "timeout" (default 20000, BWPORT_PILOT_TIMEOUT): one long walk
  or scene need not be split (`To:60000 G:npc:11`). Only the next token: before a `G:zone` only its first hop gets it.
- `Wf[:N]` the field is free (usually after a warp/trigger/npc token). `Wz:N` in zone N. `Ws:FLAG` / `Ws:!FLAG` /
  `Ws:0x408a=3` a story flag or work value (hex numbers from the dump). `W30` frames (avoid).
- `Pa`/`Pb` pulse key A/B through events; `Py`/`Pn` answer a script's yes/no (or two-item list); `Ka:2`, `KU:70` hold keys N
  frames (a b e s R L U D); `Mz:N` mash until the field is up in zone N (NEWGAME's intro); `S` the field menu's SAVE.
  During a scripted ride (Nimbasa's coaster, Castelia's cocoons) the ride is an event: `W` and `G` press A through it, only `K..:N`
  counts frames (hold Down in a `KD:N` once the car stands at a station to get off).
- `Pl:N` answers a script's list menu with row N (1-based, the order the script adds them; `Pl:0` is B): it presses through the
  event until the list is up, moves the cursor, A, and logs the answer's value (`pilot: Pl:2 done ...: the answer is 1`). `Pk` answers the
  naming screen (nickname or player name): waits for it, then A, Start, A until it closes (the name is "A"; never an empty one).
  `Pk:pw` is White 2's Plasma Frigate password (the terminal in zone 563, after its yes/no: `R:script:563:2 Py Pk:pw`): the naming
  screen's stand-in types the password the game asked for, then Start, A (Start first: an A before it types a letter after it).
  Both wait for a question the running event asks, so after `R:script:ZONE:ID` (which then only starts the script and goes on) or
  a `G:npc:N:nota` that you start another way. Proven 11-12: Black, `X:flag:0x73:1 R:script:10:6 Pl:2 Pl:3` (Striaton, Fennel's
  house, a bookshelf's list loop) and `NEWGAME: Pa Pk Mz:391`; Black 2: `NEWGAME: Pa Pk M12000` ok (Pl compiles there, the C-Gear
  leg cgear-yes2 still passes through the changed list wrapper; no Black 2 Pl proof yet).
- `X:money:N` `X:item:ID:N` `X:mon:SPECIES:LEVEL[:1[:MOVE]]` `X:badge:N` test cheats (set money, add items, add a party member (`:1` = as the
  lead, `:0` last; `:MOVE` = move id in its first move slot, e.g. 19 Fly for `F:fly`; route legs never use X:mon since bt1's move choice: the pilot picks a move that hurts the target), add a badge); `X:heal` heals the
  party as a Pokemon Center does (HP, status, PP: the move menu refuses a move with no PP, and the pilot only presses A, so a lead
  whose first move ran dry hangs the battle; the battle cheat keeps PP since 11-13, older saves can have one); see
  "What a save holds". `X:warp:ZONE:X:Z[:DIR]` (a TEST save only, for standing at a mechanic: never a route leg): the game's own map
  change to a grid zone's tile (DIR 0 up 1 down 2 left 3 right, default down; follow it with `Wf:60`); rail zones (city plazas) are not
  supported. Driftveil Gym (Black 2 / White 2): the slide pads are planned as jumps (`G:X,Z` across a pit goes over the pad: the dump
  shows pads as `p`); the elevators and freight lifts are the gym's own scripts (`G:bg`, `G:npc`).
- `E:on` / `E:off` wild encounters on/off from here (legs start off; the leg flag `encounters` turns them on for the whole
  leg); `Cc` before a `G:npc` / `G:item` / `G:bg`: the battle that talk starts (a scripted wild battle: Black / White's legendary in
  N's Castle) is a catch with the Bag's first ball, not a knock-out (the test cheat's one-shot would end it before the script sets
  its flag), and the nickname question is No; `C:ball` catch a wild Pokemon with the first Poke Ball (grass; it turns encounters on itself); `C:ball:ITEM` the same with
  the ball ITEM (a number: 3 Great Ball; it must be in the Items pocket, on the Bag's first page of balls: add it with `X:item:ID:N`
  first; the pilot keys to its slot (2 columns of 3) and the leg fails with "another ball" when a different one goes);
  `X:time:MONTH:DAY:HOUR` moves the game's clock (the RTC is pinned by BWPORT_BENCH_CLOCK, which route.py sets: the season follows
  the month, (month-1)%4, and the game's cache within 11 frames; the log says what the game reads).
- Field moves (the test cheat gives the party the move; the pulse key is A inside the token, so no `Pa` or `Py` needed):
  `F:cut:UID` / `F:smash:UID` (a tree or rock actor: walk next to it, face it, A, Yes, the event's end; = `G:npc:UID` with A),
  `F:strength:UID[:DIR[:N]]` (the boulder; with DIR 0 up 1 down 2 left 3 right it is pushed N tiles, default 1, from the far
  side: the key is held until the BOULDER's tile is N on, since a push leaves the player where he stands; a big actor's dump
  line says `size WxH`: it covers x .. x+W-1, z-H+1 .. z from its tile, and G:npc, F:strength and F:push stand next to that
  footprint, on whichever tile of the far side the player reaches first: Dragonspiral Tower's 2x2 boulder, Black zone 208 uid 0,
  is `F:strength:0:L` from b-dragon3; Black 2's Victory Road ruins room, zone 587, `F:strength:0:L F:push:0:U:3`, both legs),
  `F:push:UID:DIR[:N]` (the same, Strength already on). Strength holes are the dump's `O` (a wall); one a boulder fills is `o`
  (floor: the planner walks over it, as the game does where the save keeps a boulder), `F:stone:X,Z:DIR` (Chargestone Cave's crystal on tile X,Z: all share
  uid 253, so the tile names it; the dump's `E` tiles are the electric rocks it slides up to; no Strength; the crystal slides to the
  tile in front of the next E in DIR and does not move if there is none; `python3 tools/crystal_solve.py --start Z,X,Z --target Z,X,Z DUMP...`
  searches the push list from `--full-map` dumps made with `BWPORT_MAPDUMP_H=x0,z0,x1,z1`: all four games, see its header; it also
  prints the `G:X,Z G:warp:Z Wf:60` hops where the walk to the next crystal leads through another zone; make the dumps in the
  story state of the leg: Black 2 / White 2 need the Frigate done, work 0x40f0 = 4 and flag 0x3c2 set, else Hugh stands in the
  corridor at 37,37 and the dumps' blockers are wrong). The whole Black 2 / White 2 cave from Route 6's door to Mistralton,
  one token line, proven live on both (11-13, ~15000 frames): `F:stone:15,39:2 F:stone:9,26:3 F:stone:24,16:2 F:stone:24,17:1
  F:stone:60,42:1 G:67,50 G:warp:196 Wf:60 F:stone:45,14:2 F:stone:43,12:0 G:52,5 G:warp:195 Wf:60 G:warp:107 Wf:60` (from
  the arrival tile 7,59 inside the door: the scene trigger at 8,50 runs by itself, start with `Pa`; a test save needs
  `X:work:0x40f0:4 X:flag:0x3c2:1 X:warp:195:7:59:0`), `F:surf:X,Z:DIR` (stand on the shore tile X,Z, face DIR toward the
  water or the shore's edge tile, A, Yes; the player ends on the water, 1 tile on or 2 over an edge tile), `F:surf` (the same
  from the nearest land tile that faces water: start the leg's tokens with `Wf:60` so the terrain is loaded). While the player
  surfs, a plain `G:X,Z` walks over the water (the dump's `~`) and back onto a shore: a step onto an edge tile (`e` in the dump,
  surf edge classes 0x41, 0x44) hops 2 tiles onto the land beyond it, a shore tile next to the water is stepped onto. The plan
  never goes from land into the water: that is `F:surf`'s start (proved on Black: from b-cave1, `F:surf:13,8:2`, `G:7,12`,
  `G:5,12` crosses Wellspring Cave's lake and ends on the other shore). `F` in the dump is a waterfall tile (class 0x40).
  `F:waterfall` (surfing; the water tile in front of the fall, A, the climb ends on the water above it; `F:waterfall:X,Z:DIR` names the tile, no coordinates = the nearest
  water tile with a fall in front over the surf-walkable tiles; the decompiled check (fieldmap_event.c, hm_event.c) tests only the party's move, no badge: the test cheat gives the move, else X:mon): proven 11-13 on a
  test save in Victory Road (Black 2 zone 589, White 2 zone 595, from the south door tile 65,26): `X:warp:589:65:26:0 Wf:60 F:surf F:waterfall` ends at 37,16 (two tiles up the fall).
  `F:whirlpool` (HeartGold / SoulSilver; surfing): `W` in the dump is a whirlpool tile (blocked); the token walks to the water tile
  in front of one (`F:whirlpool:X,Z:DIR` names it, no coordinates = the nearest over the surf-walkable tiles), A, Yes (std 10016,
  the test cheat gives the move; the badge is real), and the crossing ends on the first water tile past the whirlpool.
  HG/SS planner notes (hgpil1): sliding ice (Mahogany's gym, the Ice Path) is planned as Platinum's (the dump's `i`, the slide's
  end and its speed on slopes); a live trigger (its work = its value) that shares its script with another trigger of the zone
  (a hazard field: Ecruteak gym's 15 pit rectangles, Victory Road 3F's holes) is walked round when there is a way round (the
  dump's trigger line says `live: walked round`); a story scene (a trigger with its own script: Cherrygrove's rival) is
  walked over as on every other game, so put the G: goal past it. A trigger that is the goal (`G:trig:N`,
  or a `G:X,Z` on it) is reached as before; one with no way round is crossed. A slide over a trigger tile is not checked.
  A script's list menu on HG/SS (the shrine's quiz, an elevator's floors: MenuItemAdd in the script) is `Pl:N` as on Black / White:
  row N in the order the script adds them (1-based); the pilot log says the answer's value.
  `F:fly:ZONE` (Black, White; port/src/route_fly.h): Fly the way the player does: X, the Pokemon entry, the first party member
  that knows Fly, its Fly entry in the submenu (item 16 + the move slot), the Town Map's cursor steered by the D-pad to the
  place whose first parameter is ZONE (the zone id story.py and the dump print; a town's place is its town zone, not a
  building), A, the flight; done when the player is in ZONE and the field has been free 60 frames. The party needs a Pokemon
  that really knows Fly (the list reads real move slots, no badge check in the field): `X:mon:SPECIES:LEVEL:LEAD:19` adds one
  (LEAD 1 = first slot, 0 = last). ZONE no place on the map: the line lists every place as zone:canFly; canFly 0 (not
  visited) or no Town Map (cannot fly here) end the leg `stuck: F:fly: ...`. Proven 11-13 on a copy of b-post1 (Nuvema,
  zone 389): `Pa X:mon:519:20:1:19 F:fly:16 S W30` ends zone 16 637,599, `F:fly:136 F:fly:62` ends zone 62 406,469,
  `X:mon:519:20:0:19 F:fly:6` (the Fly Pokemon in slot 3) zone 6 781,589, each repeat x3 SAME. Black 2/White 2: not yet.
- `F:teach:ITEM:SPECIES[:FORGET]` (Black, White; port/src/route_teach.h): teach a TM or HM from the Bag the way the player does: X,
  Bag, the TMs and HMs pocket, the cursor to ITEM (an item id: 421 = HM02 Fly), Use, A through its messages, Yes; the Pokemon list:
  the first party member of species SPECIES; when it knows 4 moves, Yes to forgetting one and, on the summary's move page, the move
  FORGET (a move id: required then, the pilot never picks one), A, A; B out of the Bag and the menu; done when the move is in its
  slots (the party data, checked). Errors end the leg `stuck: F:teach: ...` (no such Pokemon, 4 moves and no FORGET, it does not
  know FORGET, no such item in the pocket, it already knows the move, it cannot learn it, an HM move cannot be forgotten). The game
  side: hooks in port/extern/src/ov070/itemmenu.c (the Bag's proc table), ov131/p_sta_skill.c (the move page), ov091 (the list's
  question). Proven 11-13 on a copy of b-post1: `Pa F:teach:421:643:163 F:fly:136 F:fly:389 S W30` (Reshiram forgets Slash for
  Fly, flies away and back) ends zone 389 782,750, repeat x3 SAME.
  Not yet: Flash, Whirlpool (port/src/route_fieldmove.h header: what is left).
- `Md` map dump now, `C` end the run here: a look leg is `look | SAVE | Md C | zone 0 0,0 | -` (verdict `look`, the dump is
  the product). Remove look legs from the route file before committing (`sed -i '/^look | /d' tools/routes/GAME.txt`).
- Flags (6th field): `encounters`, `noskip`, `realtime`, `quick`, `defaultopts`. Prefer targets (npc/warp/trig/Ws) over coordinates
  and fixed waits. A battle is pressed through with A (forget-a-move and switch questions: No).

## What a save holds (blockers that are game state)
- `python3 tools/route.py GAME state SAVE`: zone + tile, badges, money, party (`#species Llevel hp/max`), bag per pocket (`item x count`), key
  items, HMs held, story counts. Numbers only. `python3 tools/route.py GAME diff SAVE_A SAVE_B`: what changed between two saves (place,
  money, badges, party, bag, works, flags). Offsets: tools/story.py SAVE_BLOCK / SAVE_BAG; decoder: tools/route_state.py.
- Cheat tokens (test cheats under the pilot, like the battle cheat; they run on the next field frame with the game's own functions, one
  `cheat:` line in the game log each; all four games): `X:money:N` sets the money, `X:item:ID:N` adds N of an item (a number) to the
  bag, `X:mon:SPECIES:LEVEL` adds a party member (no wild catch needed: the Twins want 2 party Pokemon), `X:badge:N` adds badge N (0..7).
  Example (White): `twins-cheat | w-r3a | Pa X:mon:504:4 X:money:5000 G:trig:8 Wf S W30`; then `route.py w state w-tw5-cheat`.
- State shortcuts (tw10): `X:work:ID:VAL` sets an event work value (ID as story.py prints it, `0x40ad`; VAL decimal or 0x), `X:flag:ID:0|1`
  clears or sets an event flag (the game's EventWork functions; the `cheat:` line logs the old and the new value). `R:script:ZONE:ID` runs
  the zone's script ID (story.py `zone Z` / `script Z ID` list them) the way the field starts a trigger's script (no actor), once no event
  runs, then presses through its event until the field is free for 8 frames; "script zone" and the end when the player is not in ZONE.
  Use them as a repeatable fallback for a puzzle a token cannot drive yet, and SAY SO in the leg's comment (a leg that skips the real
  step is not a playthrough). Checked on b-cas3: `X:flag:0x281:1 X:work:0x40ad:7 R:script:38:8` ran the scene script, which put both back
  (the dump's `changed since the snapshot (0)`); without the R token both stayed changed.

## Finding the next step
1. The `end dump:` path the last leg printed (or a look leg). `grep -n "^warps\|^triggers\|^bg events\|^actors\|^story\|^changed"`
   it, then `sed -n` the sections: warps `D n ... -> zone Z`, actors `N uid U`, triggers `T n` with their work gate, the story
   section's LIVE trigger / DUE scene = what runs next where the player stands, `changed since` = what the last leg did.
2. Why a door/blocker is closed: `python3 tools/story.py GAME who-sets flag 0x.. [=0]` / `who-sets work 0x.... =N` (what
   sets it, who runs it, the checks on the way), `story.py GAME zone Z` (everything runnable in the zone),
   `story.py GAME who-warps Z` (scripted doors: gyms and buildings have no warp; an actor's script changes the map: talk
   to it), `story.py GAME script Z ID` (one script). `story.py GAME next` (to come: the next scene from a save) when it exists.
   `story.py GAME who-uses CMD` (a command's name in any case, or its id): each script that runs it, as zone (constant name), script id, arguments.
   `python3 tools/atlas.py GAME path A B` gives the zone path between two zones.
3. Story order and puzzle solutions: a game guide (WebSearch/WebFetch) to plan, then confirm in the dump and story.py.
   Never copy guide or game text into the repo; describe it ("the nickname question").
4. Blocked by a field move or key item you do not have: follow story.py to where it is given, or report a TOOL GAP.

## When it fails
A failed leg prints its verdict line and, under it, 1-3 triage lines (`  > no path: ...`, `  > try: ...`; tools/route_triage.py reads the
pilot log, the game log and the end dump): read them first, they name the blocker (uid and tile), the unanswered yes/no (add Py/Pn),
the trigger whose work does not match (and the `story.py who-sets` query), or a port bug. The notes below say more per verdict.
- `crash`: a port bug. Do not work around it: report the leg line, the verdict line and the game log
  (`build/route-cache/route-work/GAME-LEG.game.log`), continue on another branch if there is one.
- `stuck` / `pilot: no path`: a target cannot be reached: read the dump (grid, walls `#`, the actor/warp numbers), fix the
  token (right zone number? a wall actor to pass with G:wall? a trigger or switch first?).
- `hang` / `timeout`: a prompt the pilot does not answer, a scene waiting for a trigger, a Ws that never comes true: the
  dump's story section says what the scene waits for; `--shot N` on a manual run shows the screen.
- `off-route` / `look`+wrong end: it ended elsewhere; the dump shows where. `DIFFERENT` repeat: not stored; report it.
- Two genuinely different tries on one problem, then stop and report (leg line, verdict line, dump facts).

## Hard rules
- `Pa` first, `S W30` last. One leg = one area or scene (a few thousand frames).
- Never overwrite a save. Never quote game text (comments, README, docs): describe it. Never commit ROM bytes.
- `$BWPORT_ROUTES_DIR` (a directory holding GAME.txt files) makes `route.py` and `route_add.py` read and write legs there instead
  of `tools/routes/`: use it (with `$BWPORT_SAVES` on a scratch library) for throwaway test legs and test saves.
- Never print `build/story/cmds_*.json` or whole dumps (query with story.py / atlas.py / `sed -n`).
- Before a commit: the repo scrub gate your brief names must find nothing (no machine paths, worker ids, private names);
  the route file has no look legs; commit `tools/routes/GAME.txt` every 2-3 legs: `git add tools/routes/GAME.txt`,
  `git commit -F msgfile` with the trailer your brief gives. Saves and README stay in the library (not in git).
- Report: legs (name, start, end, `repeat` result), saves stored, commits, where you stopped and why (exact lines), `LESSONS:`, `COST:`.

## Where a gym's puzzle code is (Black / White)
A map's gimmick id (the zone's header; `src/ov021/ov021_021bddc0.c`: init table `data_ov021_021d9cbc[55]`, set up from
`fieldmap_event.c`) names its overlay: 3 Nacrene, 5 ov041 (Mistralton cannons), 8 ov044 (Driftveil lift), 9 ov045
(Icirrus: `src/ov045/gym_icirrus.c`), 10 ov046 (Opelucid: `src/ov046/ov046_021f36c0.c`), 4 ov042, 6 ov038, 7 ov043; ids 12-24 etc. ov048..ov065.
The overlay's generated tables are `port/src/asset/*.inc` (its data); the zone's scripts: `tools/story.py b script ZONE ID`.

## Hybrid zones (a grid with rails: Icirrus, Opelucid; Black 2 / White 2 Castelia plaza)
`G:` plans grid -> rail -> grid chains: the planner (route_map.c's Search) takes a switch tile's hand-off onto a rail, along
it and off it onto the grid as one edge (the dump's "hand-offs" list shows the switch tiles), and from a rail it plans to the
hand-off that leads to the goal, not the nearest one. Proven on Black: from the Icirrus Gym's west room
`X:warp:114:28:48:0 Wf:60 G:npc:13 Wf:300` walks the ice room -> the rail band -> the east room and talks to uid 13.
The Icirrus Gym's levers are planned too: the adapter's fill lists the zone's switches (the dump's `switches now` line: levers,
which flip a switch and swing its two arm actors; places, trigger tiles whose key hands the player onto a rail when the switch
is in the state `gym_icirrus.c`'s sPlacePreconditions says), and the planner searches over the switch states (LeverPlan:
the fewest levers that open a way). From the entrance `X:warp:114:16:62:0 Wf:60 G:npc:11 Wf:900` walks, rides and flips
the three levers to Brycen and wins the badge (Black, ~60 s). A lever tile is never walked across by accident (the search
ends there).

## Details
`python3 tools/route.py -h` (every option), docs/port-worker.md "Routes" (how a run is pinned, battles, rails, Mac), the
header of port/src/route_test.c (every token), port/src/route_map.c (the dump's legend), tools/story.py -h, tools/atlas.py -h,
docs/port-runtime.md "Test cheat".
