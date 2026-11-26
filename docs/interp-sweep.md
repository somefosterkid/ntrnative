# Smooth motion in-between sweep

A systemic check of Smooth motion's in-betweens (docs/interp.md): every-frame captures of walks, door/camera moves and battles in
each game are replayed offline through the live pairing code (vc_ip_pair, vc_ip_texhold, the keyframe/gap rules of
port/bridge/ip_common.h IpGather) and every in-between artifact is scored from the display lists, by class, at 60 Hz and 120 Hz.
Numbers come from the lists; pictures only confirm the top cases.

## Run

    python3 tools/interp_sweep.py run --set tools/interp_sweep_set.txt [--shard I/N] --out build/ipsw --strips 3 --drop --caps DIR
    python3 tools/interp_sweep.py report --out build/ipsw --md report.md

- `tools/interp_sweep_set.txt`: one `PLAY SPEC` per line (a tools/plays name and a BWPORT_VCAPTURE spec, every frame: `A-B/1`).
  Shards split the lines (every N-th): one shard per slot. An every-frame capture is ~1.2 MB a frame: give `--caps` a private
  scratch directory and `--drop` so the captures are deleted after scoring.
- Needs build/vcore (`meson setup build/vcore port/vcore; ninja -C build/vcore`: vcore_ipsweep, vcore_interp) and the port
  (the plays record through tools/caps.py).
- `--strips N`: the N worst events per class as 3x crops OLD keyframe | first 120 Hz in-between | KEY keyframe (vcore_interp,
  which applies the live texcoord hold) in OUT/strips/.

## The app's own pairs: Record diagnostics

The sweep pairs every captured keyframe with the one before. What the app pairs while it runs (and on the test phone) is recorded by
Record diagnostics: iOS Settings > Developer > Record diagnostics (20 s), macOS Debug > Record diagnostics (20 s), or the cfgscript's
`diagrec=S` (`quit=1` writes one still running). After a second's settle it keeps S seconds in memory, with REC and the seconds left
above the screens, and writes one file at the end: `bwdiag-YYYYMMDD-HHMMSS.bwdiag` (iOS: Documents, and the share sheet opens; macOS:
next to save.bin; `BWPORT_DIAGREC_DIR` overrides). It holds every present (its slot, sent / kept / dropped / not presented, the
picture: which renderer call and slot), every renderer call (V-blank and 3D swap counts, IpGather's outcome, the keyframe numbers it
pairs, each slot's f) and every new keyframe's state (about 3.5 MB a second in the HG/SS field: the VRAM pages changed since the
keyframe before). Layout: port/bridge/diag_rec.h. Nothing runs while it is not recording; the Mac's pacing counters were the same
with and without it (beach walk, 14 s: in-betweens 828 / 824, late 6 / 8, dropped 1 / 1, shown lag 13.4 ms both).

    python3 tools/diagrec.py FILE.bwdiag [--strides] [--events] [--keep DIR]

prints one line an item: the manifest (build, device, settings, panel Hz, heat state, Low Power Mode), the renderer calls and the 3D
swaps the pairing saw or missed, keyframe intervals, the presents against their slots, the pairing distance of every in-between shown
(3D swaps and game frames between the two keyframes it blends), in-betweens per keyframe, and the keyframes rebuilt as .vcs files
(byte for byte what BWPORT_VCAPTURE writes, but LSHT / L3D / HDMA) scored by `vcore_ipsweep --oracle` in the app's own order: the
oracle's V_ and the per-palette P_ classes on the app's pairs (`--strides`: also with keyframes 2 and 4 apart, every phase).

## Classes (port/vcore/tools/vcore_ipsweep.c's header has the exact rules)

err / vis in screen px at 256 x 192 (texels for B_*); vis120 = err x (1 - the first in-between's f) at 120 Hz, vis60 at 60 Hz
(the 30 fps field: gap 2, f = .25 / .5; a 60 fps scene has no in-between at 60 Hz). "ref" (with --oracle, as the sweep runs) = the
motion of its tag pair for a list the tags pair (the game's truth), else the median motion of the oracle's pairs within 64 px (the world);
without --oracle the median motion of the live pairs within 64 px, as before M5. (M5 measured every list the oracle pairs against its
own pair, identity's too: identity's pairs, the live pairing's rules, could not be wrong; m5fix measures only the tags' that way.)

| class | what | why it shows |
|---|---|---|
| A_MISPAIR | a paired list whose partner is not the same-key list that moves with ref (with the oracle: only lists the tags pair, not the tag's partner) | it sweeps toward another object (one tile off for a run of identical quads) |
| A_OFFMOTION | moves > thr (6 px) from ref, no better candidate (with the oracle: only lists the tags pair) | own motion, or a mispair |
| AL_MISPAIR / AL_OFFMOTION | (with the oracle) the same for every list the tags do not pair, measured against the old ref (the live pairs within 64 px): what A_ is without --oracle, for them | as A_; own motion of untagged things (debris, the follower untagged) counts |
| B_TEXSTEP | blended texcoords change >= 8 texels, not one uniform step | atlas cells mixed (the hold misses it) |
| B_TEXSCROLL | the same by one uniform step | a fast scroll (info: right to blend) |
| B_HELD / B_HELDRUN | lv3's big-list axis hold; RUN = the same list held by the same step in the pair before | a steady 8-texel scroll would be stepped |
| B_WRAP | a blended axis of a texture that repeats alike in both keyframes whose step (the blend's, vc_ip_tex_step) is over half its period; err = the slide beyond the shortest step (B_TEXSTEP / B_TEXSCROLL take the blend's steps too) | the in-betweens slide the texture back across itself (a scroll that wrapped) |
| C_MISSED | a KEY list with no partner although OLD has one where ref puts it | drawn at KEY's place: leads by |ref| |
| C_APPEAR | a KEY list with no partner, no candidate | new: drawn at KEY's place for the interval |
| C_VANISH | an OLD list with no partner, its centre on screen, not kept | gone one keyframe early |
| C_NEW | a KEY list marked new (VC_IP_NEW: vc_ip_windows, inside OLD's view) | info: not drawn before its keyframe, as without Smooth motion (vis 0) |
| C_BLINK | a C_NEW list whose key vanished where ref puts it in the pair before | a list drawn every other keyframe would show only at its keyframes |
| C_KEPT | an OLD list with no partner drawn until KEY (vc_ip_windows' gone) | info: drawn in the in-betweens moved by its motion, as without Smooth motion (vis 0) |
| C_EDGE | a list with no partner moved by the projection (vc_ip_windows): KEY's entering, OLD's leaving; err = its motion vs ref | slides in / off with the world: off by err |
| E_SPEED / E_CUT | camera px per tick changes by > max(1, half) / a cut pair | judder / a 60 Hz-style jump |
| O_DISAGREE / O_MISSED | (--oracle) a live pair that contradicts the oracle's identity / a list the oracle pairs and the live code leaves alone | a true mispair / a lead |
| V_NONE / V_TWICE | (--oracle) a thing in both keyframes drawn 0 / 2 times in the in-betweens (the oracle's identity, else the form-change proxy: one palette, 2026-11-26) | the maintainer's "never both, never neither" |
| V_STALE | (--oracle) a kept list whose texture or palette bytes differ between OLD's and KEY's VRAM; M4: also a pair drawn with OLD's texture state ("state") | drawn with the wrong image |
| P_NONE / P_TWICE / P_CROSS | (--oracle) per palette, sprite-like quads on KEY's screen (OLD's moved there by ref, 2026-11-26) that no tag decides (g5): drawn fewer / more times than each keyframe has; paired across palettes | vanish / double / a sprite blended from an effect's place |

## Findings

2026-11-26 (m5fix, scratch/m5fix/NOTES.md; branch m5): the M5 review's findings.
- The rest's pairs are measured on the screen (finding 1, 8d32fd503; BWPORT_INTERP_RESTPX=0 = M5's rest): no rest pair that moved
  more than 48 px plainly, beyond the world's motion and beyond its own D (rest_jumped); round 0 takes the nearest on the screen beyond
  the pair's own motion first (a run of same-looking quads placed on the CPU under one matrix, one gone, shifted by one: 13 units, 33
  px). With no matrices the world's motion is the median shift on the screen of identity's pairs, and a tie goes to it (da360e6e5:
  Cherrygrove's mailbox test failed without it).
- Equal counts fly on their own (finding 2, e9626d2c5): round 2's alike lists pair in draw order past the 12 units too, under the
  screen limit (vcore does not know the keyframe gap). ss-smash C_NEW 33 / 32 / 28 -> 22 / 13 / 13 (the debris pairs, each piece
  with itself, in-app no doubles), ss-cut stride 4 16 -> 12, the untagged phone file 113 / 235 / 842 -> 107 / 201 / 375 and stride 4
  V_NONE 42 -> 33. The AL_ rise beside it is the newly paired things' own motion measured against the world's (debris; the grass over
  the sprites' feet following the follower).
- The untagged phone file's A_MISPAIR 16 -> 24 at stride 1 was M5's scorer: scored the old way, m5base and M5 both give 16. The 16 are
  footprints (two a tile) while the camera moves exactly a tile a keyframe: the camera's motion and none both fit exactly. A true
  periodic ambiguity, there before M5; not fixed.
- The scorer (finding 5, 3f75cb4d7): with --oracle only a tag pair is its own truth (A_); every other list is measured against the
  live pairs around it, as before M5 (AL_). Fair re-measure, one scorer, the same captures, m5base's pairing -> M5's, 68 plays at
  strides 1 / 2 / 4: A_MISPAIR / A_OFFMOTION 0 -> 0; AL_MISPAIR 0 / 2 / 14 -> 0 / 1 / 13, AL_OFFMOTION 161 / 681 / 1559 -> 163 /
  678 / 1555, C_NEW 1603 / 1016 / 906 -> 1603 / 1018 / 908, V_TWICE 0 / 1 / 2 -> 0, O_MISSED 0 / 2 / 7 -> 0. M5's headline below
  (A_MISPAIR 68 / 186 / 428 -> 0 / 1 / 8, A_OFFMOTION 598 / 1971 / 4227 -> 0 / 28 / 216) was its scorer measuring identity's and the
  tags' pairs against themselves: m5base's were as right. M5's gains: V_TWICE / O_MISSED at strides 2 / 4, the untagged phone file at
  stride 4 (AL_MISPAIR 210 -> 188, AL_OFFMOTION 1850 -> 1565, V_TWICE 4 -> 0, O_MISSED 11 -> 0; C_NEW 592 -> 842 and V_NONE 41 -> 42
  until m5fix) and the code. Battle windows and the tagged phone file: the same for all three.
- pt-ledge's V_NONE 7 is no picture problem: scored by one scorer m5base's pairing gives 0 too (the old scorer's far test skipped the
  vertices far off the screen). The NPCs' sprites, shadows and effects are drawn millions of units away in keyframe 7005 (w 4.4M-30M:
  not on the DS's screen either) and at their place in 7007; in-app the in-between after 7005 shows no NPCs, 7007 and 7009 both
  (scratch/m5fix/fin-ledge.png).
- BWPORT_INTERP_STATE=0 is M3 again (finding 6, 7e3dde439): the tagged sprite's hold runs after the 8-texel rule, gated by it.
  Tests (finding 4, da360e6e5): the four deleted tests that still hold are back, and round 0's (own matrices, and placed on the CPU
  under one matrix, which M5's round 0 got wrong). interp_stats.py reads the stats lines again; stale text (73873bb8a).
- m5fix against M5 (one scorer): only ss-smash (AL_MISPAIR 0 / 0 / 0 -> 2 / 1 / 0, AL_OFFMOTION 15 / 9 / 5 -> 23 / 27 / 20, C_NEW 33 /
  32 / 28 -> 22 / 13 / 13) and ss-cut stride 4 (AL_OFFMOTION 5 -> 9, C_NEW 16 -> 12) move of the 68 plays; V_ / P_ 0 at every stride
  (= sweep-m5.txt); battle windows and the tagged phone file the same. REPLAYCHECK 0 differ (route1-battle, ss-cherry-run4, pt-door),
  vcore_same 2070 / 0 differ, vcore_tests 450 (1 todo), --time on the untagged phone file 195 us median a pair (M5 193), check.sh 9 /
  10 (b2 reset-save: the save). In-app mid shots (scratch/m5fix/fin-beach.png, fin-smash.png): the follower at Run 4x with the player
  in every shot (as M5), Rock Smash's debris whole.

2026-11-26 (m5, scratch/m5/NOTES.md; branch m5 = m5base + ocean): one pairing system. p23_interp.c 2579 -> 1718 lines (port/vcore:
-1947 / +579).
- Measured first (vc_ip_why and vcore_ipsweep's ORACLEP lines, 2af42bc57; 68 plays tags on, stride 1): after identity and the tags
  the old passes made prefix / suffix 192685 pairs (1625 on the screen), LCS 124, the camera alignment 45, the loose gap pass 834;
  repair_runs, the reorder pass, the jump drop, rule 5 on the rest and the scene rule decided nothing. The final 48 px filter dropped
  only pairs the oracle makes (30 tag pairs at stride 1; 61 tag and 34 identity pairs at stride 4). mark_new, vc_ip_keep's vanish
  history, APPEAR, VANISH and CUTSEEN ran only with WINDOWS=0. On tagged sprites M4's state rule replaced texhold's bits (4261 of
  124001 pairs). The untagged phone file: loose 390 pairs, 216 on the screen.
- Deleted: the order passes (prefix / suffix, LCS, cam_motion / shift_median, align_strict / band_xy, the loose gap pass,
  repair_runs, pair_reordered, the jump drop), mark_new, vc_ip_keep, the scene rule, the 48 px filter over every pair, vcore_ipscan;
  switches BWPORT_INTERP_IDENT, _REPAIR, _REORDER, _CAMREL, _APPEAR, _VANISH, _CUTSEEN (and --repair). Kept: TAGS, STATE, CUTID,
  WINDOWS, TEXHOLD, TEXWRAP; the camera-plane rule. A tagged sprite's cell hold is texhold's tag rule (state or texcoords differ).
- Now: the tags, identity, then the rest (rest_pair, INTERP-ARCH 2.2 step 5): within a loose key the motions identity would take
  first (a chosen D or none), then within a key and a loose key draw order for equal counts, nearest after the camera's D otherwise,
  only within 12 world units. Identity's pairs have no jump limit; a tag pair moving more than half the screen plainly and beyond the
  world's motion is no pair. how: identity / tag / rest / jumped / behind; the identity cut counts identity and tag pairs.
- The scorer with --oracle measures against identity: ref = the oracle pair's motion, else the median of the oracle's pairs around
  it; its far test sees lists far off the screen. A_ classes now count only disagreements with the oracle and pairs it does not know.
- Open items: pt-ledge's V_NONE 7 (stride 1, keyframe 7007): NPC shadows and effects drawn millions of units off the screen in 7005,
  at their place in 7007; the 48 px filter dropped the tag pairs and the oracle's far test could not see the jump. Now the tag pairs
  jump (no pair) and the oracle calls them far: 0. route1-wild-faint's V_TWICE at stride 4 (1325): the 48 px filter dropped the wild
  Pokemon's identity pair and kept its older list: 0. hugh-fight's 18 C_EDGE -> C_NEW: newborn particles (kind 3, age 1) at the
  screen's edge, which M4's kinds rule (a particle is never entering) calls new: right, a newborn was not in the older keyframe.
- Against m5base, 68 plays at strides 1 / 2 / 4: V_ / P_ 0 everywhere (pt-ledge 7, pt-r202 and route1-wild-faint at stride 4 1 -> 0).
  Live classes per stride (M5's scorer, self-referenced: restated by one scorer in m5fix above): A_MISPAIR 68 / 186 / 428 -> 0 / 1 /
  8, A_OFFMOTION 598 / 1971 / 4227 -> 0 / 28 / 216, B_TEXSCROLL to 0 / 2 /
  1524 (ocean). Worse: ss-smash A_OFFMOTION 13 -> 16 (stride 1) and C_NEW / C_KEPT +7 (2) and +10 (4): untagged rock debris rising on
  its own, paired by the rest at stride 1 (A_OFFMOTION: a pair the oracle cannot judge), too far for rule 5 at strides 2 / 4 (drawn
  as without Smooth motion); ss-cut stride 4 A_OFFMOTION 4 -> 5; route1-wild-faint stride 4 E_SPEED 11 -> 12 (the scorer's camera
  speed now from the oracle's pairs). Draw order without the 12-unit limit pairs the debris but brings back HeartGold's rings
  mispair (p23_identity_rings_a_phase_apart_are_two_things); as a last round it raised ss-smash A_OFFMOTION to 25 and A_MISPAIR to 2.
  Battle windows: nothing worse. The tagged phone file (ed810): V_ / P_ 0 at 1 / 2 / 4. The untagged one (M1's): V_NONE 2 / 6 / 42
  (the old passes 2 / 6 / 39), C_NEW / C_KEPT at stride 4 592 / 583 -> 842 / 842: open (untagged sprites moving past rule 5's 12
  units at stride 4; the old order passes paired them with a 48 px screen limit).
- BWPORT_IP_REPLAYCHECK=1: route1-battle 5400 geometries, pt-door 600, 0 differ; ss-cherry-run4 253 in-betweens, no mismatch.
  vcore_same 2070 scenes, 0 differ; vcore_tests 441.
  The live path's time a keyframe pair (vcore_ipsweep --time over the phone files' keyframes, Mac): tagged 153 / 203 us median / p95
  (2a6e5b495: 597 / 954), untagged 194 / 237 (554 / 741). In-app mids (scratch/m5/shots): the beach at walk 4x with the follower,
  Route 1's grass in Black, a send-out: whole, nothing doubled or missing.

2026-11-26 (ocean, scratch/ocean/NOTES.md): the test phone's sea flickered for one keyframe interval every now and then with Smooth motion on.
HG/SS move each sea layer with a texture matrix that jumps back by two or three periods of its 32- (or 16-) texel repeating texture every
12 s (Cherrygrove's beach: keyframes 11261 and 11981; s / t +63.81 / -63.88 and -95.75 / +95.69 texels, 16-texel layers -31.94); the blend took
those steps as they are, so every layer slid back across itself, each by its own amount. Old: the texcoord blend since c5a27a1c7 (a05aeed49 and
74019379f show it; Smooth motion off, STATE / CUTID / WINDOWS / TAGS = 0 do not change it). The blend now takes the shortest step that samples
the same texels (vc_ip_tex_step; BWPORT_INTERP_TEXWRAP=0 = before). In-app mid shots at that pair, water crop difference to the shots around
it: 7.7 / 7.6 -> 0.9 / 1.3 (Smooth motion off 0.9 / 1.0). The sweep had counted these as B_TEXSCROLL (info: "a fast scroll, right to blend"),
3-5 in 11 HG/SS plays, never compared with the texture's period; they are B_WRAP now: all 68 plays, TEXWRAP=0 / on at strides 1 / 2 / 4:
13 HG/SS plays 1-5 / 0 at every stride (B_TEXSCROLL -> 0 in 11, ss-grass-back / -east keep 1, below); at strides 2 and 4 also the Gen 5 battle and field backgrounds (route1-battle, white-route1 43; b2 /
w2-docks 14; town-lab, white-lab 10) and pt-lake 30 / 0. Every other class line as m5base's. Open: ss-grass-back / -east's shore lists
(64 x 64 A5I3) scroll t by -1.06 texels a keyframe and jump +85 every 240 frames (21 mod 64: a restart, not a wrap): now a 21-texel slide
(was 85); a hold for such restarts is not in.

2026-11-26 (m5base, scratch/m5base; branch m5 at ed810f4ab = M3 + M4 + G5 + the fishing fix): the two rows together, rerun in full, then the
baseline stored (kits/interp/baseline/sweep-m5base.txt). The HG patch was regenerated (the fishing fix is in build/pokeheartgold/src), bwport and
the vcore tools rebuilt; vcore_tests 448 passed. All 68 plays (the set now includes ss-fish-bite, which runs past the bite) recorded tagged at
every frame and scored at strides 1 / 2 / 4, plus G5's two battle windows (route1-battle 2150-2550, hugh-fight 1300-1700).

- Gen 4 (41 plays) = M4 at all three strides (V_TWICE 1 at stride 4 = pt-r202, as before); the 12 Gen 5 plays = G5: all 13 score files (with
  r202-wild) are byte for byte G5's, V_ / P_ 0 at every stride; r202-wild P_NONE 0; E_CUT = M3 (r202-wild 7839, lab-out 521); both cut rules agree
  on every pair (differ 0 in all 68); STATE=0 CUTID=0 = M3's live lines except ss-bike and ss-follower (below).
- The 14 plays added since M4: pt-ledge V_NONE 7 at stride 1 (= its baseline); route1-wild-faint (Gen 5, tagged) V_NONE / V_TWICE 11 / 3 -> 0 at
  stride 1 and 2 / 5 -> 0 / 1 at stride 4: one static 48-vertex list at x -220 off the screen edge drawn twice at keyframe 1325 (not
  looked into); ss-fish-bite: 350 captures, V_ / P_ 0, A_OFFMOTION 2, C_NEW 1.
- Battle windows: V_ / P_ 0 at strides 1 / 2 / 4 (M2's pairing, --alt with TAGS=0: 81,59 / 28,26 / 52,33 and 246,139 / 87,31 / 71,31). One
  difference from G5's file: with M4's state rules hugh-fight's 18 C_EDGE lists are C_NEW (alpha-blended lists, 1388; STATE=0 gives G5's 18 back).
- M4's swap, 0 in the unit tests only, fires in two plays: ss-bike and ss-follower, 1 pair each (keyframe 12819); live class counts unchanged.
- route1-battle's list 44 (M4's open item): MIX 0 with BWPORT_INTERP_STATE on and off (was 1 on, 0 off); pictures 5581 / 5577.
- BWPORT_IP_REPLAYCHECK=1: route1-battle 10800 geometries, 0 differ; ss-cherry-run4 no mismatch (254 in-betweens).

2026-11-26 (g5, Gen 5's tags; scratch/g5/NOTES.md). Black / White and Black 2 / White 2 now tag what they draw, as Gen 4 does since M3:
the field's billboard actors (kind 1: the player, NPCs, and the shaking grass, dust, ripples and shadows that Gen 5 draws as billboards in
their own scenes), the battle and musical sprites (kind 4, each cell of the multi-cell sprite: inst = the node's entry in the multi-cell
it shows, so a pose change is new cells; kind 2 its shadow), lib/spl's particles (kind 3) and the 3D field objects (kind 5). All through
port stand-ins (port/extern/<path>, port/extern_b2w2/<path>; scratch/m3a/TAGS.md "Sites, Gen 5"): the game C is untouched, so the DS
ROMs cannot change. The pairing needed no change.

- The 12 Gen 5 plays and the two battle windows below, every frame: the pre-change exe's captures = BWPORT_INTERP_TAGS=0's byte for byte
  (INFO's libntr= aside), and tagged minus its tags = TAGS=0's; with TAGS=0 every play "= baseline".
- Tagged, all 54 plays are "same" as M3's scorer and = baseline but r202-wild (below). The oracle in the 12: ambig 304 -> 56, moved 624
  -> 0, tag pairs 2492; hugh-fight's 56 are four zero-size (invisible) copies of static model lists. V_ / P_ at strides 1 / 2 / 4: 0 in
  every Gen 5 window, with M2's pairing too: the set's windows hold field walking only (b2-r19's play has no battle).
- Battles (tags as truth, M2 = --alt with TAGS=0; V_NONE, V_TWICE at strides 1 / 2 / 4, P_ 0 both ways; baseline/sweep-g5-battles.txt):

| window | M2 | G5 |
|---|---|---|
| route1-battle f2150-2550 (the send-out) | 81,59 / 28,26 / 43,33 | 0,0 / 0,0 / 0,0 |
| hugh-fight f1300-1700 | 246,139 / 87,31 / 68,35 | 0,0 / 0,0 / 0,0 |

  O_DISAGREE 142 / 408 -> 0 and O_MISSED 463 / 665 -> 0; A_MISPAIR 8 / 15 -> 0 / 1 and A_OFFMOTION 144 / 405 -> 71 / 25, all paired by
  the tags (particles bursting from one point). hugh-fight's C_EDGE 0 -> 18 (up to 12 px): particles that enter at the screen's top edge
  with their emitter's motion, which the class measures against the camera's.
- First try, inst = the node's number: the trainer's throw changes pose (another multi-cell) and the oracle paired a cell with the other
  pose's cell in the same node (O_MISSED 2 / 1); the node's entry in the multi-cell fixes it.
- The scorer: the P_ census leaves out the lists the tags decide (vcore_ipsweep.c); the oracle's V_ judges those by identity. In the
  battles it called the thrown ball (one particle whose palette changes each keyframe as it spins) P_CROSS, and a particle dying and one
  born at the same spot P_TWICE (26 / 12 / 6 and 56 / 22 / 9 at strides 1 / 2 / 4). r202-wild's P_NONE 3 (newborn leaves) -> 0 the same
  way; no other play changes.
- In-app, the GPU path, mid shots with tags and with TAGS=0 (scratch/g5/shots): a grass step on Route 1 and the battle's send-out (the
  ball, the burst) look the same both ways but for the shots' phase. BWPORT_IP_REPLAYCHECK: 3600 + 1800 in-betweens compared, 0 differ
  (route1-battle f300-2350, b2-r19 f300-1200). vcore_tests 444 passed; tools/check.sh 9 / 10 legs (b2 reset-save: no b2-champ save).
- Not covered: the musical's sites (no play reaches a musical) are built, not run.

2026-11-26 (m4, M4 of the identity plan: state and kinds; scratch/m4/NOTES.md). A pair whose texture address or palette base differs
switches to the older keyframe's at the middle (f >= .5) when that texture's bytes did not change between the keyframes (vc_ip_texsig
hashes each list's texels and palette span at its keyframe; vc_ip_state compares with the newer VRAM), its texcoords with it; a tagged
sprite's texcoords always switch at the middle when its state or texcoords differ; alpha (attr bits 16-20) blends; a kept list whose
bytes changed is not drawn. An owner whose lists differ in the two keyframes and pair nowhere (VC_IP_SWAP) draws the old lists in [0, .5)
and the new in [.5, 1]. Particles are never entering and overlays never glide. The cut (vc_ip_cut_id): fewer than half of the newer
keyframe's on-screen vertices paired through identity or a tag, or the proven pairs moving more than half the screen width (128 px).
BWPORT_INTERP_STATE=0 turns off the state, swap and kinds rules, BWPORT_INTERP_CUTID=0 the cut rule; both off = M3 (all 54 plays score
M3's scorer's lines byte for byte).
- 54-play sweep (scratch/m4/sweep-all.txt, recorded once, the new scorer vs M3's on the same captures): live classes = baseline and
  "same" in 54/54; strides 1, 2 and 4: no class changed in any play (162 play-strides; scratch/m4/strcmp.py). E_CUT 3 / 2 / 2 = M3:
  hg-violet, pt-r202, town-lab and b2-r19 stay 0 at every stride. Over the 9513 keyframe pairs (scratch/m4/m4stat.c): the cut
  rules agree on every pair; 5683 lists switch state at the middle (all Gen 4 plays, Gen 5 only 2-5 a play; counted before empty
  lists were skipped), 1013 blend alpha (14
  HG/SS plays: footprints fading 31 -> 0 by 2 a keyframe; r202-wild's battle-intro leaves); swap, half and dropped kept lists: 0 (the
  swap is in the unit tests only). B_TEXSTEP 0 everywhere; B_HELD = M3 (1328 / 1059 / 531), all big untagged lists: a tagged sprite's
  hold is not the big-list rule's. V_STALE has a state check (a pair drawn with OLD's texture state whose bytes changed): 0.
- r202-wild's cuts, one by one: 7835 -> 7839: OLD has only two untagged lists off the left edge, KEY adds the intro's first particle
  (861, 367 px): 0 of 4 on-screen vertices proven. 7843 -> 7845: particle 861 paired by its tag (state 4 -> 5, 18 px), 863 and 865
  new: 4 of 12 proven. Both cut by M3's rule too; the new particles are new either way, the cut stops only 861's 18 px blend.
  lab-out 521 (the door) = M3. hugh-fight 262-300: no cut, C_KEPT 5 / C_NEW 1 as M3; the Aspertia City banner whole and still in the
  in-app mids at 268-298, M4 and M3 alike.
- The phone file (scratch/p0/phone1, 597 pairs): every stride's line = M3's scorer's (V_NONE 2 / 6 / 39); 659 state switches (the
  untagged walk cells' texture addresses; 144 more on empty lists before they were skipped), 558 alpha blends (a fading quad); cut 0 both rules.
- tools/interp_metric.py, in-app gpu Metal 120 Hz, BWPORT_INTERP_STATE on / off: white-route1 cell steps 760 / 759 MIX 0 / 0; b2-town
  55 / 53 MIX 0 / 0; route1-battle 924 / 925 MIX 1 / 0: list 44 at keyframe 1098 (a 66 x 32 px quad paired with a 30 x 10 px one, over
  the hold's 64 px limit) blends unheld in both builds (the off run's charlog shows the same texels at f .48 and .98); the metric's
  12-40 px box takes it only at the on run's f .80. Gen 4 (ss-cherry-run4, town-walk, pt-door): cell steps 0, MIX 0 (their cells change
  by texture address: the state switch, which the metric does not see).
- BWPORT_IP_REPLAYCHECK=1: white-route1, route1-battle, b2-town, town-walk, r202-wild, pt-door (600-5400 geometries each) and
  ss-cherry-run4: 0 differ. vcore_tests 448 (4 new: the switch, the tape's texture and alpha, kinds and swap, the cut); vcore_same 2070
  scenes 0 differ. vcore_interp cherry-door-east 11047 / 11045: at f .75 the footprint's 15 px sit between the keyframes' alphas, at
  .25 they are the newer's. In-app mids (Run 4x follower walk, the r202-wild intro, footprints fading, the hugh-fight banner): M4 and
  M3 alike, nothing doubled or missing. The keyframe's texture test: 5-37 us a keyframe (mean per play), 880 us at most; vc_ip_state 2-4 us.
- Left: route1-battle's list 44 (Gen 5, untagged: for the g5 tags); a tagged sprite whose texture changed in place and whose texcoords
  differ would show OLD's texcoords on the newer texture after the middle (0 in five capture sets); 7845's cut counts two new particles
  against a paired one (the rule as written).

2026-11-26 (m3p, M3's proof on tagged plays; scratch/m3p/NOTES.md). The bridges' SIM_G3TagEnable is libntr's now (the weak stand-in is
gone), so with Smooth motion on every Gen 4 capture is tagged; Gen 5 has no tag sites yet. Before = M2's pairing (BWPORT_INTERP_TAGS=0)
and after = M3, both scored on the same tagged captures with the tags as the oracle's truth (42 Gen 4 plays, BWPORT_VCAPTURE_TAGS=1,
mplay --alt; V_NONE, V_TWICE, P_NONE, P_TWICE, P_CROSS summed over the plays):

| keyframes apart | M2 | M3 |
|---|---|---|
| 1 | 39, 4, 3, 0, 0 | 0, 0, 3, 0, 0 |
| 2 | 40, 32, 1, 0, 0 | 0, 0, 1, 0, 0 |
| 4 | 62, 70, 0, 0, 0 | 0, 1, 0, 0, 0 |

- The oracle's ambig: 4637 -> 0 in the 40 field plays; look (tier 1) 3481 -> 900 over all 42. Left: hg-door / ss-door 1528 ambig and
  pt-door 900 look, all untagged map lists (the interiors' identical static props and the door model): tags do not cover them, and
  identical static copies blend the same whichever pairs.
- HG/SS grass at strides 2 / 4: ss-grass-run2 V_NONE 9 / 11 -> 0, ss-grass-east-w2 10 / 24 -> 0. Each rustle is its own instance: one
  that ends and one made again on the same tile switch at the keyframe (moved with the world) instead of blending into each other.
- Run 4x shadows: ss-cherry-run4 keeps A_MISPAIR 31 (ss-cherry-run2 20, ss-grass-run2 2), but the OA audit calls all of them and every
  A_OFFMOTION "tag right": the player's and the follower's shadows stand still on the screen while the world scrolls, so the heuristic's
  "the same-key list that moves with ref" is another NPC's shadow a tile away. Mispairs by identity: 0 (O_DISAGREE 0).
- The follower at Run 4x (ss-cherry-run4, sprite inst 1807): M2 made it new + gone at 11009 and 11019 (its image changed; the old one
  slid with the world, half a tile into the player at f = 0.5); M3 blends it (vcore_interp 11007 / 11009: strict 462, removed 0).
- Live classes against M2's baseline: the 12 Gen 5 plays and pt-lake "same"; the other Gen 4 plays' counts = baseline except fewer
  new + gone (C_NEW / C_KEPT) where M2 switched one thing (ss-cherry-run4 5 -> 3 / 2 -> 0, ss-beach-up 17 -> 15 / 14 -> 12, pt-r201
  2 -> 1 / 2 -> 1) and more where the tags say two things (ss-beach-up-w2 12 -> 14 / 8 -> 10: the player's shadow hidden at the water
  while the follower's appears, which M2 slid into each other; ss-grass-east-w2 26 -> 28 / 26 -> 28: a rustle made again on its tile;
  ss-beach-in-w2 14 -> 15 / 9 -> 10: an effect under the player, the next under the follower; r202-wild 29 -> 28 / 20 -> 21);
  A_OFFMOTION -1 to +2 in six plays and A_MISPAIR 1 -> 0 (ss-beach-up-w2), all tag right; r202-wild V_NONE 6 -> 0.
- --repair 0 = --repair 1 with tags on the 9 tagged sets at strides 1, 2 and 4 (repair_runs is dead for tagged plays); without tags it
  still changes ss-grass-run2 at stride 4 (O_DISAGREE 14 / 22).
- BWPORT_INTERP_TAGS=0: the bridges ask for no tags (captures untagged) and all 54 plays are "same" as M2's scorer, = baseline, and
  every stride count = M2's sweep. The live switch (cfgscript smooth=0 at f11030, smooth=1 at f11060, ss-cherry-run2): every frame
  is whole (14 tagged lists or none; the change shows 3 frames later); the pairs across it are M2's.
- M3a's tagged sets (BWPORT_VCAPTURE_TAGS=1) score byte for byte as the new build's own captures (Smooth motion on, no env).
- BWPORT_IP_REPLAYCHECK=1 with tags: ss-grass-east-w2, ss-beach-walk, route1-battle 600 geometries each, 0 differ (ss-cherry-run4: no
  mismatch). The keyframe's gather (the I line, median us, tags on / off): Run 4x 672 / 753 and 732 / 702, grass 585 / 645 (noise).
  vcore_tests 444; vcore_same 2070 scenes, 0 differ. In-app mids (gpu Metal, 120 Hz) at Run 4x with the follower and in tall grass at
  walk 2x look the same both ways (nothing missing or doubled). tools/check.sh: 8 Gen 5 legs ok, the rest need extract/ and saves.
- Left: pt-r202 V_TWICE 1 at stride 4 (an untagged 8-vertex prop off the left edge, new + kept: M2 has it too); r202-wild P_NONE 3
  (the newborn leaves, the census's 64 px cut); the Gen 5 sites (TAGS.md's list); A_ classes restated on identity (M5). The effects'
  state 0 is enough: a ring or splash is its own instance, so no animation phase is needed to pair it.

2026-11-26 (m3b, M3 of the identity plan: the vcore side of tags; the game and libntr side is M3a's, its interface in its TAGS.md).
- The tag is a GX3D entry, op 0 with 3 parameters and flag 8: owner = kind << 24 | id, inst, state; (0, 0, 0) ends one. vc_gx_command
  hands it to vc_ip_tag (RECORD only) and vc_ip_begin copies the current tag into each list (VC_IpList owner / inst / state,
  VC_IpRec.ntag). The DS's NOP has no parameter, and every reader steps over n of them, so old tools read tagged captures and new ones
  old captures. The renderers never see a tag (vcore_same: 2070 scenes, 0 differ).
- Pairing (p23_interp.c tag_pair, before identity). When both keyframes have tags, the tags decide every tagged list with vertices.
  It pairs with OLD's list of the same (owner, inst): an instance's lists in draw order, same type and vertex count, and the same shape
  too when the instance drew another number of lists. An overlay (kind 6) never pairs, and neither does a particle whose age went
  down (a new particle in its slot). The decided lists still vote in identity's rule 2, so its Ds are M2's, but no rule pairs them.
  Their pairs are locked with identity's (they anchor the gaps). A decided list with no pair is left out of the order passes: it is
  new or gone. The 48 px jump limit still applies to tag pairs.
- Windows: a decided list with no partner is tested and moved with its owner group (tag_group_d): the D most of its owner tag
  pairs have; for a shadow (kind 2), its sprite's first (the tag pair of another kind with the shadow's inst); for an overlay, none.
  Entering ones get enter slots after the projections' (VC_IP_NENTER = VC_IP_NPROJ + 24). With no group pair: the world's, as in M2.
- Switch: BWPORT_INTERP_TAGS=0 (vc_ip_tags) means the pairing reads no tag (M2's) and the bridges ask the game for none. RECORD
  keeps tags either way, for the oracle. Frame pairs where a keyframe has no tag are M2's.
- Bridges: ip_common.h IpTagsWanted runs each frame and calls libntr's SIM_G3TagEnable(Smooth motion on and tags on).
- Scorer: the oracle takes the tags as its truth (orc_tags, written apart from the live pass). New class "tag" in the ORACLE line (only
  when some pair is tagged) and " tag N" on a tagged pair's ORC line. Decided lists vote, no other step pairs them, and the
  form-change proxy leaves them.
- Checks: t22 p23_tags_recorded_from_the_stream, p23_tags_copies_swap_places, p23_tags_shadow_follows_its_sprite and
  p23_tags_pose_change_keeps_its_pair (vcore_tests 444). Untagged: the 54 plays recorded again and scored by the new scorer were
  "same" as M2's scorer in all 54, and all matched the baseline. tools/check.sh: 8 Gen 5 legs ok; b2 reset-save and the Gen 4 legs
  fail for missing extract/ and saves, as before.
- The proof on tagged plays: the m3p entry above.

2026-11-26 (m2, M2 of the identity plan: visibility windows). First the M1 review's fixes (counts = M1's in 54/54 plays after each):
gap anchors are the longest run of identity's pairs that keeps the draw order; a copy group of more than 1024 candidate pairs (or past
16384 in the frame) takes only candidates through a plausible motion, found by an x-sorted lookup, and does not vote; gap_cost has a
budget (32768 cells, then first fit); the run re-pairing keeps the gap pass's rules, stops at locked lists and has 4096 cells with
identity; rule 2's ties go by votes, unique votes, pairs, |t|, hash; BWPORT_INTERP_IDENT is clamped to 0..2; a lock needs equal vertex
counts. 256 particles (an image a cell on, both frames): pairing 11.65 -> 0.78 ms.
- vc_ip_windows (BWPORT_INTERP_WINDOWS, 1 default; 0 = mark_new and vc_ip_keep, M1's: the live classes of all 54 plays byte for byte
  as M1's scorer) places every list with no partner by projecting it into the other keyframe's camera with the world's D of its
  projection (Dc = inverse(P) x D x P'). KEY's: outside OLD's view volume = entering, blended toward that place (it slides in with the
  world); inside = new (window [1, 1]: the game would have drawn it and did not). OLD's: outside KEY's view = leaving, kept and moved by
  the projection; inside = gone, kept until KEY and moved with the rigid group it moved with in the pair before, else the world's.
  Never both, never neither; no pixel constants. The world's D of a projection is the strongest D identity chose that a list drawn with
  it moved by; none: no motion. (Platinum's battle intro, r202-wild: an ortho layer's two lists sliding 8 px were the only D chosen,
  and as "the camera" it moved the perspective shards' lists with no partner 192 px: C_EDGE 143 px; with the D by projection the
  shards have none and switch at their keyframe.)
- Sweep (scratch/m2/sweep-m2b.txt, 54 plays, strides 1 / 2 / 4, both ways): DISAGREE and MISSED 0 everywhere; C_APPEAR and C_VANISH 0
  in every play. C_EDGE 67 lists, none >= 1 px vis120, max 0.50 vis120 = 0.67 px (ss-cherry-run4 11013: 188 vertices entering at the
  right edge); ref itself scatters as much there (paired lists within 40 px of the side edges: p95 0.58 px, max 2.57). Every change
  (M1 -> M2): route1-battle and white-route1 C_APPEAR 12 / C_VANISH 9 -> C_NEW 9 / C_KEPT 9, C_EDGE 3 (Gen 5's grass over the player:
  the old one kept until the keyframe, the new one from it; the in-app mids show the new grass a keyframe later than M1, at its
  keyframe); b2-r19 C_APPEAR 6 / C_VANISH 6 -> C_NEW 1 -> 7, C_KEPT 1 -> 7; r202-wild C_APPEAR 10 / C_VANISH 2 -> C_NEW 19 -> 29,
  C_KEPT 18 -> 20; ss-cherry-run4 C_APPEAR 10 / C_VANISH 2 -> C_NEW 3 -> 5, C_KEPT 1 -> 2, C_EDGE 9 (the follower now switches at its
  keyframe; the edge lists slide in); hg-trainer / ss-trainer C_APPEAR 3 / C_VANISH 2 -> C_NEW 4 -> 6, C_KEPT 4 -> 6, C_EDGE 1;
  ss-beach-up C_APPEAR 2 / C_VANISH 2 -> C_NEW 15 -> 17, C_KEPT 12 -> 14; ss-beach-in-w2 C_APPEAR 3 / C_VANISH 1 -> C_NEW 13 -> 14,
  C_KEPT 8 -> 9, C_EDGE 2; cherry-door-east C_APPEAR 9 -> C_NEW 7 -> 10, C_EDGE 6; town-lab C_APPEAR 3 -> C_NEW 0 -> 2, C_EDGE 1;
  pt-r202 C_APPEAR 3 -> C_NEW 0 -> 2, C_EDGE 1; C_APPEAR n -> C_EDGE n (edge lists at Run 2x / 4x and doors): white-lab 7,
  ss-cherry-run2 5, ss-cherry-night 5, ss-run-toggle 5, b2-docks 3, w2-docks 3, lab-out 2, ss-beach-walk 2, ss-beach-walk-w2 2,
  town-walk, b2-town, w2-town, cherry-door-west, ss-water, ss-beach-in 1. The other 27 plays: no change.
- Strides (V_ / P_, windows vs WINDOWS=0): stride 1 the same in every play; stride 2 / 4 totals V_NONE 11 -> 12 / 13 -> 13, V_TWICE
  0 -> 0 / 9 -> 2, P_NONE 1 -> 1 / 2 -> 0, P_TWICE 0 -> 0 / 7 -> 0. Worse: ss-grass-run2 V_NONE 1 -> 2 at stride 2, ss-grass-east-w2
  6 -> 8 at stride 4 (its V_TWICE 3 -> 0): HeartGold's grass quads on a tile grid spaced by the camera's motion at that stride;
  identity pairs the copies of the same image standing still and leaves the true partners (the next animation frame, at their place
  in the world) unpaired inside the view, so new. M1 drew those from the in-between's start at KEY's place (no V_ event), M2 from KEY:
  one mispair either way, tags' (M3).
- The P_ census counts OLD's quads where ref puts them on KEY's screen: route1-battle / white-route1's P_TWICE 1 was a grass sprite
  0.3 px on the screen's top in KEY, counted, and its partner 0.6 px off it in OLD, not counted (one each, two drawn).
- The test phone's file (scratch/p0/phone1, the rebuilt keyframes scored directly): V_ / P_ / DISAGREE as M1 (V_NONE 2, the rest 0);
  C_APPEAR 15 -> 0, C_VANISH 6 -> 0, C_MISSED 3 -> 0, C_NEW 105 -> 113, C_KEPT 108 -> 114, C_EDGE 10. Keyframes 4 apart: V_NONE
  26 -> 39 (all the grass quads above), V_TWICE 17 -> 4, P_NONE 7 -> 0, P_TWICE 5 -> 0.
- vcore_tests 440 (t22: entering and new; leaving and gone with its group; the world's D by projection); vcore_same 2070 scenes, 0
  differ; REPLAYCHECK 600 geometries, 0 differ (beach, tall grass, Gen 5 grass); in-app mids (gpu Metal, 120 Hz) at walk 2x in tall
  grass and on the beach the same both ways.
- Left: copies spaced by the camera's motion (the grass above, Run 4x A_MISPAIR) and the follower at Run 4x (switches at its keyframe
  now; blending it needs tags): M3. "Already moving by the same D keeps its pair" needs history (M3 / M4). Identity's rule 5 still
  measures every projection against one camera D (no play shows it).

2026-11-26 (m1, M1 of the identity plan: identity first): vc_ip_pair locks the pairs identity proves and runs the old passes on the
rest only. BWPORT_INTERP_IDENT=0 is the pairing before (the live classes of all 54 plays byte for byte as M0's scorer, every stride);
1 = identity with the old first-fit gap pass; 2 (default) = identity plus a gap pass that pairs within a texture family and palette.
- Identity (p23_interp.c ident_pair, the oracle's rules, vcore_ipsweep orc_pair the same): signature = key + shape (geo) + projection;
  D = inverse(KEY's matrix) x OLD's, voted per signature; the camera's D chosen over all projections, a rigid group's only from two
  signatures unique in both frames; copies pair through the chosen Ds unless a list has candidates through two plausible motions (a
  chosen D or none: at Run 4x the follower's shadow stands where the player's stood in the world, the player's where it stood on the
  screen); a unique or last pair more than 12 world units from where the camera's D puts it, measured in KEY's camera space (billboards
  are scaled: the rings' 15-unit swap measured 8 in their own units), is two things. Tier 1, on what is left: the signature without the
  texture image's address and the palette (Platinum's door scene redraws a sprite at its place each keyframe with the next image and
  palette: 900 C_APPEAR / C_VANISH with the palette rule alone), through a plausible motion only, a group left alone when any list has
  two plausible candidates (HeartGold's grass over a sprite's feet is a quad of the sprite's shape at its place).
- The rest: a compacted record (the vertex arrays shared), the camera's motion measured over the whole frames; the rest's gap pass
  pairs b's lists only between identity's partners of the nearest locked lists (the draw order's gaps: without them the player's grass
  cover met the next tile's rustle, 12 px slides in ss-trainer), never two lists of one signature identity says are two things, and
  (IDENT 2) one texture family and palette (paletted formats), by cost (list_cost, nearest first).
- The sweep (54 plays: 7 new walk-2x plays, the test phone's WalkSpeed=2, tools/plays/soulsilver/*-w2.txt), IDENT 0 -> 2: O_DISAGREE
  and O_MISSED 0 everywhere; V_NONE / V_TWICE / P_NONE / P_TWICE / P_CROSS totals at strides 1 / 2 / 4: 18,1,3,1,18 -> 6,0,3,0,0;
  63,21,45,10,95 -> 11,0,1,0,0; 90,59,76,53,241 -> 13,9,2,7,0. A_MISPAIR 56 -> 54, C_MISSED 0 -> 0 (M0's ambig: 8053 lists).
  Class changes (stride 1, vs the kit's baseline): ss-beach-up A_OFFMOTION 10 -> 5, C_APPEAR 7 -> 2, C_VANISH 7 -> 2, P_CROSS 7 -> 0;
  ss-beach-walk C_APPEAR 8 -> 2, C_VANISH 7 -> 0, C_KEPT 27 -> 28; ss-beach-in P_NONE, P_CROSS 1 -> 0; ss-water C_APPEAR 2 -> 1,
  C_VANISH 1 -> 0; ss-ground-turn C_APPEAR 2 -> 0, C_VANISH 1 -> 0; ss-grass-east / -back A_MISPAIR 1 -> 0, A_OFFMOTION 1 -> 0, C_NEW
  and C_KEPT +2 each (a switch at the keyframe); pt-r201 A_OFFMOTION 1 -> 0, C_NEW / C_KEPT 1 -> 2. Worse, each understood:
  r202-wild A_OFFMOTION 61 -> 91, C_NEW 9 -> 19, V_NONE 1 -> 6, P_NONE 0 -> 3 with E_CUT 12 -> 2, C_APPEAR 19 -> 10, C_VANISH 5 -> 2,
  C_KEPT 22 -> 18: the battle intro's polygon IDs count lists, so every key changes; first-fit by shape paired each grass bar with the
  one before, the jumps made 10 frames cuts; within one palette the bars pair with themselves and those frames are interpolated (OA:
  44 right, 47 unknown, 0 wrong); P_NONE is the census's 64 px cut (a leaf of 58 px paired with itself at 66 px), V_NONE the emitter's
  newborn leaf (C_NEW). route1-battle / white-route1 / b2-r19 A_OFFMOTION 9 / 9 / 6 -> 0, C_APPEAR / C_VANISH +9..12: the grass
  over the player steps 13 px a tile; identity calls it two things, so it no longer slides, but mark_new / vc_ip_keep's 16 px near test
  takes it for the pairing's miss and shows the new one a keyframe early (M2's windows time it). ss-cherry-run4 C_APPEAR 8 -> 10,
  C_VANISH 0 -> 2: the follower at Run 4x, 16 units from the camera's D with a bob (tags, M3).
- The test phone's recording (scratch/p0/phone1: beach water and tall grass, walk 2x; scratch/p0's tools/diagrec.py with this build):
  at its own pairs V_NONE 41 -> 29, V_TWICE 21 -> 0, P_ 18 / 12 / 49 -> 0, oracle disagree 109 -> 0, missed 16 -> 0; the 29 were the
  proxy matching a splash, ring or rustle born at the player's feet with the player's or the follower's sprite of another palette, so
  the proxy now needs one palette (its precision on pairs the oracle knows: 466 false of 15575 -> 197 of 15562); with it V_NONE 29 ->
  2. A_OFFMOTION 70 -> 34, A_MISPAIR 12 -> 16 (Run 4x grass a tile apart, the camera moving a tile: two plausible motions, left to the
  order passes as before; tags, M3).
- In the app (Mac, the test phone's settings, gpu Metal, BWPORT_PANELHZ=120 BWPORT_SHOTMID=1): ss-beach-up's in-betweens at 11189 and
  11197 draw the player 10 px low with 0 (paired with the follower), in place with 2; tall grass (ss-grass-run2) the same both ways.
  BWPORT_IP_REPLAYCHECK=1: 1200 geometries, 0 differ. Pairing time (vc_ip_pair alone, best of 200): Cherrygrove 475 lists 1.71 ->
  0.46 ms, the block load (151 / 466) 0.80 -> 0.39 ms, 498 lists 2.17 -> 0.70 ms; the present log's I line (the whole gather) at
  Cherrygrove Run 2x mean 3.3 -> 0.87 ms, max 5.8 -> 1.3 ms. vcore_tests 433 (t22: copies under pan, zoom and turn with a culled copy,
  the beach rings a phase apart, Run 4x shadows), vcore_same 2070 scenes 0 differ.
- Left for later steps: switch timing (Gen 5's grass, the leaves, the rings a phase apart that identity pairs crosswise when they are
  5 px apart): M2; copies whose spacing is the camera's motion and the follower at Run 4x: tags (M3).

2026-11-25 (ipid, M0 of the identity plan; the test phone's water and grass reports): the oracle and two "never both, never neither"
checks, no behaviour change (the live classes of all 47 plays byte for byte as c20b14dff's scorer; vcore_tests 430; vcore_same 2070
scenes, 0 differ, the Mac has no corpus).
- Recording: VC_IpList gains proj (the projection's index in the frame), geo (a hash of the object-space vertices), tex (of the
  TEXCOORD parameters) and mv (the position matrix at the first vertex), in RECORD mode (p23_interp.c ident_vertex). Nothing reads
  them in the app yet.
- vcore_ipsweep --oracle (interp_sweep.py passes it): signature = (key, geo, proj); D = inverse(mv_key) x mv_old voted per signature;
  the camera's D is the most voted (over all projections: billboards use another projection with the same camera), a rigid-group D
  needs two signatures unique in both frames (shadow twins vote as much for swapping as for standing still), and a unique pair more
  than 12 world units from the camera's D that fits no chosen D is not identity (the grass's last image of one tile and the next
  tile's; ring phases). --orcdump FRAME prints the oracle's view of one pair; OA lines judge every A_ event (right / wrong / unknown).
- 47 plays, 2.95 M lists: uniq 92.1 %, static 7.3 %, moved 0.09 %, ambig 0.26 % (6-vertex shadows), new 0.24 %; DISAGREE 106 (run4's
  shadows at 11005-11025, the beach rings), MISSED 16. A_OFFMOTION 339 = 206 real motion by the oracle, 133 unknown, 0 mispairs.
- New checks: V_NONE 36 (the shadow copies at the start of a run and at Run 4x: unverifiable without tags), V_TWICE 0, V_STALE 0;
  P_NONE 1, P_CROSS 10 at the game's rate, 28 / 4 / 51 with every 2nd keyframe paired, 62 / 40 / 188 with every 4th: the loose gap
  pass's cross pairs (scratch/ipform/DIAG.md) grow with the distance between paired keyframes.
- Plays added: ss-water, ss-sand-turn, ss-grass-thru, ss-run-toggle (scratch/ipid) and ss-beach-in, -walk, -up, -turn, ss-grass-east,
  -back, ss-ground-turn (scratch/ipform). The audit and the revised plan: scratch/ipid/AUDIT.md.

2026-11-25 (sw11), lists that vanish (sw10's other side; the test phone's settings, Smooth motion on at 120 Hz: the grass's last image and
the player's shadow as a step into tall grass starts were gone up to one keyframe early; where the shadow goes as the shake starts, neither was
drawn):
- Cause: an in-between is the newer keyframe's command stream with its vertices blended toward the older keyframe's. A list of the older
  keyframe with no partner in the newer one (C_VANISH) is not in that stream at all, so it was gone from the first in-between after the
  older keyframe, while Smooth motion off shows it until the newer one. Since sw10 the shake waits for its keyframe, so in ss-grass-run2's
  11027 -> 11029 (the shadow goes, the shake starts) the player stood on bare ground for those in-betweens.
- Whose motion: in ss-r29's 11211 -> 11213 the player's shadow (two 6-vertex lists, in the group of every object's shadow, drawn before
  the sprites) stands still on the screen with the player while the camera moves 2 px a keyframe; the grass's last image (a 32 px quad)
  moves with the camera. Both lie over the player's sprite, and the list before each in the order is a map piece or another shadow:
  neither the place nor the order says whose they are. Their own motion in the pair before does: the shadow 0 px (the camera 2), the
  grass's image 2 px (the camera 2).
- Fix (p23_interp.c vc_ip_keep, p14_polygon.c vc_ip_draw_kept; BWPORT_INTERP_VANISH=0 = the old rule): IpGather keeps the pair before's
  map. After vc_ip_pair, vc_ip_keep keeps each of the older keyframe's unpaired lists that may be on the screen and has no vertex behind
  the camera, unless an unpaired, not new list of the newer keyframe of its shape is within 16 px of where the camera's motion puts it
  (the pairing missed it: mark_new's other side, and that list is drawn at its place as before). Its motion: the camera's at it now when
  it moved with the camera in the pair before or had no partner there, else its own motion then (the shadow: 0). It is anchored after
  the newer keyframe's partner of the nearest paired list before it in the older keyframe's order. The geometry engine draws the kept
  lists in the in-betweens (blend > 0) at their anchor (the next BEGIN_VTXS, or SWAP_BUFFERS) from the older keyframe's recorded vertices
  (clip coordinates, colours, texcoords) with its polygon attributes, texture and palette, each vertex moved by its share of the motion
  (clip x, y + motion x w), p14's state restored after: in the full run (the vcore path) and the tape replay (the GPU path) alike. A list
  leaving across the screen's edge slides off with the camera instead of going a keyframe early (ss-cherry-run4: a house culled at the
  left edge, -15 px a keyframe). The texture is read from the newer keyframe's VRAM: one the game frees and reuses within a keyframe
  would show wrong in those in-betweens (none seen). vcore_ipsweep: C_KEPT (info, vis 0), C_VANISH the rest, `kept` on its PAIR lines;
  vcore_interp --prev (the keyframe before B, for the motion).
- The cut (vc_ip_cut; the pairing's architecture review): a new list (VC_IP_NEW) counted as unpaired, so 30 new lists on the screen
  that are 1/12 of them (weather starting, an effect's particles) made a scene cut, a hitch, while a keyframe that only lost lists never
  was one. With lists kept, neither side jumps: the scene rule no longer counts new lists (with BWPORT_INTERP_VANISH on). They still count
  as unpaired vertices, so a picture mostly new is a cut: Platinum's wild battle intro (r202-wild 7855 -> 7861, a dozen quads of a new key
  each keyframe) lost two of its cuts when new lists were left out of the vertices too, and its mispaired quads glided (A_OFFMOTION
  +8). The only cut of the sweep set with new lists in lab-out (519 -> 521, the map after the door's fade, 39 new) stays one.
- The sweep, 36 plays recorded on the Mac, vcore_ipsweep from c4a61d2b8 against this: C_VANISH 115 -> 9 (C_KEPT 106); every other class
  of every play the same (E_CUT 13). hg-r29 / ss-r29 / ss-grass-run2 13 -> 0 each (the shadow 4 with motion 0, the grass's last image 9
  with the camera's), r202-wild 27 -> 5, hugh-fight 8 -> 0, hg-door / ss-door 6 -> 0, hg-trainer / ss-trainer 6 -> 2, cherry-door-east /
  west 4 / 5 -> 0, pt-door 3 -> 0, lab-out 2 -> 0, b2-r19, pt-r201, ss-cherry-run4 1 -> 0. Left: the trainer plays' 2 are a shake's last
  image with the next tile's shake within 16 px of where the camera puts it (taken as the pairing's miss on both sides, as before); the
  5 of r202-wild are the battle intro's 600-700 px quads (a vertex behind the camera).
- In the app (Mac, the phone's settings, ss-grass-run2 at Run 2x, BWPORT_INTERP_VANISH=1 / 0 on one build): gpu Metal with
  BWPORT_PANELHZ=120 BWPORT_SHOTMID=1, the in-between before keyframe 11029 shows the player's shadow under the player with 1, not with 0;
  Renderer=vcore with BWPORT_INTERP_DUMP, the same in its in-between pictures; vcore_interp renders of 11027 -> 11029 at f .75 / .5 / .25
  agree. BWPORT_IP_REPLAYCHECK=1: the replayed geometry equals the full run's in every in-between. Cost: vcore_ipsweep over lab-out
  (270 captures) 0.28 s and ss-r29 (121) 0.06 s, the same before and after. vcore_tests 430 (t22 p23_old_list_kept_until_its_keyframe,
  p23_burst_of_new_or_gone_lists_is_no_cut).

2026-11-25 (sw10), tall grass (the test iPhone 16 Pro Max, HeartGold, gpu 3x, Metal, Smooth motion on at 120 Hz: walking through tall grass the
grass's shake appeared before the player touched the tile, then moved along with the player for about a frame before it settled):
- The game's timing (Smooth motion off, BWPORT_MOVELOG4): HeartGold starts the shake at the tile a step goes to as the step starts. It is in
  the first frame after the step's start (Run 2x: step at V-blank 11028, the shake in frame 11029; Walk 1x: 11212, 11213), while the player
  still stands on the tile before. That is the DS's behaviour; the follower starts its own shake two frames later.
- Smooth motion on: the shake (a 4-vertex quad, 32 px, its key the animation's first image) is a list of the newer keyframe with no partner
  (C_APPEAR; it is never paired with the old tile's shake, which keeps its own partner). An unpaired list was drawn at its keyframe's
  place in every in-between: it came up from the first in-between before its keyframe (f .25 at 120 Hz: up to 25 ms before the game shows
  it), standing still on the screen while the camera scrolled under it, off its tile by the camera's motion times (1 - f): Walk 1x 2 px a
  keyframe (1.5 px at the first in-between), Run 2x 7.4-8.2 px (5.6 px; about 29 of the phone's pixels in portrait). At its keyframe it is paired and moves with
  the map: the "moves with the player, then ends up in the right spot".
- Fix (p23_interp.c mark_new, vc_ip_blend_list; BWPORT_INTERP_APPEAR=0 = the old rule, the same output): at the end of vc_ip_pair a list of
  the newer keyframe with no partner is marked new (map VC_IP_NEW) when it lies wholly inside the screen (not entering it) and no list of
  the older keyframe of its shape (type, vertex count) is left alone within 16 px of where the camera's motion (cam_motion) puts it (one
  there means the pairing missed it). In the in-betweens (blend > 0) a new list's vertices are moved past the view volume's right side, so
  clipping drops it; it appears with its keyframe, as without Smooth motion. One rule for anything the game starts: grass shakes,
  footprints, the follower coming out, battle effects. IpGather counts them (BWPORT_INTERP_REPORT `new`); vcore_ipsweep has the classes
  C_NEW (info) and C_BLINK (a C_NEW list whose key vanished where ref puts it the pair before: a list drawn every other keyframe would
  show only at its keyframes) and `new` on its PAIR lines.
- New play ss-grass-run2 (ss-r30 warped to Route 29's grass, B + left at Run 2x; ss-r29 / hg-r29 walk into the same grass at 1x):
  C_APPEAR 7 -> 0, C_MISSED 7 -> 0 (the follower's shake on the player's tile), C_NEW 14; ss-r29 and hg-r29 C_APPEAR 11 -> 0.
- The sweep, 36 plays recorded on the Mac, vcore_ipsweep from 74019379f against this: only C_APPEAR / C_MISSED moved to C_NEW, C_BLINK 0,
  every other class of every play the same. cherry-door-east C_APPEAR 14 -> 9, C_MISSED 2 -> 0; cherry-door-west 7 -> 1, 2 -> 0;
  ss-cherry-night 6 -> 5, 2 -> 0; ss-cherry-run2 6 -> 5, 2 -> 0; ss-cherry-run4 9 -> 8, 2 -> 0 (sw8's two C_MISSED left there were the
  follower and its shadow coming out, scored against the player's shadow); hg-door / ss-door 4 -> 0; hg-violet / ss-violet 3 -> 0,
  4 -> 0; hg-trainer / ss-trainer 7 -> 3; r202-wild 28 -> 19; hugh-fight 3 -> 0; b2-r19, pt-r201 1 -> 0.
- In the app (Mac, the phone's settings, BWPORT_PANELHZ=120 BWPORT_SHOTMID=1, Run 2x, one build with BWPORT_INTERP_APPEAR=0 / 1): the
  in-between before keyframe 11029 shows the shake with the old rule, not with the new one nor with Smooth motion off; from 11029 on all
  three draw it at the same place. vcore_interp renders of 11027 -> 11029 at f .25 / .5 / .75 agree. Cost: vcore_ipsweep over
  hugh-fight 0.89 -> 0.88 s, cherry-door-west 0.51 -> 0.51 s. vcore_tests 428 (t22 p23_new_list_waits_for_its_keyframe).
- Left: the other side. A list of the older keyframe alone (C_VANISH: the grass's last image, the shadow as the player steps into the
  grass) is still gone from the first in-between, up to one keyframe early; where a shadow goes as a shake starts (6 of the sweep's 90
  C_NEW lists), neither is drawn in those in-betweens. Drawing such lists until their keyframe, moved by the camera's motion, needs lists
  that are not in the newer keyframe's stream drawn in both paths (vc_geo_replay, the full run) with the older keyframe's texture state.

2026-11-25 (blo1), Black lab-out A_OFFMOTION 1 -> 41 after the Mac push: not cam_motion. BWPORT_INTERP_CAMREL=0 has the same 40 events; the cut rule
that counts only the lists on the screen (vc_ip_cut, sw9) is what removed the scene cut at pair 575/573: the lab's lists stay while the map behind
the door loads under a black screen, nothing on the screen is unpaired, and the camera jumps 20-60 px a keyframe, each part its own way
(cam_motion's local reference follows it). Fix: vc_ip_pair leaves the pair unpaired (a cut) when 24 or more lists on the screen move > 6 px
off the camera's motion at them (IP_WORLDMOVE_N / _PX; the next most in the 35 plays: 12). BWPORT_INTERP_WORLDCUT=0 / sweep variant `noworld` = off.
35 plays, noworld -> now: only lab-out changes (A_OFFMOTION 40 -> 0, E_CUT 1 -> 2); every class of every game equal elsewhere (A_OFFMOTION
568 -> 528 in all). The 2 extra C_APPEAR of town-lab (1.1 px, off-screen block load at f452) are the price of sw9's no-cut rule, left as is.
(2026-11-26, the ios merge: the paragraph above is main's, written for the pairing before M5 (cam_motion, align_strict, vc_ip_cut's
on-screen count); those passes are gone. IP_WORLDMOVE / BWPORT_INTERP_WORLDCUT were not carried: with ios's code Black's lab-out
(300-700/1) is one cut (the identity cut, vc_ip_cut_id) and A_OFFMOTION 0, so the case it fixed is a cut there already.)

2026-11-25 (sw9), the hitch at a map block load (the test iPhone 16 Pro Max, HeartGold, build b34f5fddb, Smooth motion on: walking
west from Cherrygrove's Pokemon Center (door 564,391) to the Mart (555,391), a hitch exactly as the player passes the middle of the two trees):
- The cause: the cut rules counted every list. x 560 is the middle of map block 17; there HeartGold's MapLoadManager loads the next block
  column, and its models reach the display lists in one keyframe: 248 -> 551 lists, 303 of them new and none of them on the screen (101 lists
  on it before and after). 303 >= 30 and >= 1/12 of the lists: vc_ip_scene_cut called it a scene cut, so that pair had no in-between and the
  new keyframe went up at once: on the glass the game time jumped +35..41 ms in one refresh and stood for the next two (Mac, pretended
  120 Hz link, the phone's settings, BWPORT_PRESENTLOG). The same at Walk 1x / 2x and Run 1x / 2x / 4x, Exact and Fast. Not the CPU or the
  GPU: the game's tick <= 1.3 ms, Stage2 0.2 ms, no late present or drop at the spot. Smooth motion off has no hitch there (the glass steps
  stay at one tick, 16.7 ms; the worker's stage 1 <= 2.8 ms).
- Every game streams its map so: each scene cut left in the sweep set was such a load, off the screen (PAIR: key lists new / new on the
  screen): town-lab 452 230 / 2, b2-r19 712 258 / 0 (not a transition, as sw6 read it), hg-violet and ss-violet 166 / 0, pt-r202.
- The fix: vc_ip_cut (p23_interp.c), the two rules (fewer than half the vertices pair; the scene rule) over the key's lists that may be on the
  screen (vc_ip_list_seen: not all of a list's vertices past one plane of the view volume). Lists past it draw nothing, so they can neither
  glide nor jump. IpGather and vcore_ipsweep use it; BWPORT_INTERP_CUTSEEN=0 is the old rule (A/B on one build); PAIR lines add `onscreen`
  (the unpaired lists on the screen); t22 p23_cut_counts_lists_on_the_screen.
- Before -> after (CUTSEEN=0 -> 1, the walk f11400-11800, cuts / largest game-time step on the glass): Walk 1x 1 / 41.6 -> 0 / 16.7 ms
  (Fast 1 / 37.1 -> 0 / 16.7), Walk 2x 1 / 39.9 -> 0 / 16.7, Run 1x 1 / 39.3 -> 0 / 16.7, Run 2x 1 / 34.9 -> 0 / 25.0 (a late mid), Run 4x
  1 / 39.9 -> 0 / 16.7 (Fast 1 / 33.9 -> 0 / 16.7). Violet City walking east (ss-violet, f11491, 166 new lists, none on the screen):
  1 cut, +34.6 ms -> 0, every in-between shown. Route 29 west (21 new lists) and Cherrygrove east (a block dropped) had no cut either way.
- The sweep, 8 plays recorded again and scored both ways: E_CUT hg-violet, pt-r202, town-lab, b2-r19 1 -> 0, every other class the same
  but town-lab C_APPEAR 1 -> 3 (the 2 new lists on the screen, at its edge, drawn at the key's place); r202-wild E_CUT 11 -> 12 (a battle
  pair whose lists on the screen pair under half their vertices), A_OFFMOTION 64 -> 61, C_APPEAR 30 -> 28, C_VANISH 28 -> 27; lab-out,
  cherry-door-west, ss-cherry-run4 unchanged. tools/mac_fftoggle.py (BWPORT_FFMAX=0) PASS.
- Left: the pairing costs more while the block is loaded. align_strict (the na x nb alignment, ~11600 list_rel a keyframe at 551 lists) is
  1.2 -> 3.9 ms on the M2's worker, so the first mid of a paired keyframe settles 6.4 ms (p50) after the worker starts instead of 4.4, and
  on this loaded Mac 6 of 140 such mids were late (0 of 58 before the load), each held one more tick. band_xy (list_rel's coordinates once
  a frame; the same results, checked on every pair of 400 keyframes) takes it to 3.4 ms. The rest is the alignment of lists that are off
  the screen; leaving them out would change pairs (a sweep's work).
- Also seen: pass S's third self-check (gpu_bridge.c Stage2, the 1800th 3D frame, ~30 s after the title) costs one renderer call 17-23 ms
  on the Mac: a one-off stall of its own.
- BWPORT_PRESENTLOG has the GPU path's CPU halves now, each line stamped at its end (us, the pacing clock): W due 3D slots us (the worker's
  stage 1), I (the keyframe: due tick why lists verts unpaired vertices-paired gap slots texdata before-pairing record+pairing
  unpaired-on-screen lists-on-screen, each mid settled in us from the start, negative = none, the end; why 0 repeat 1 pair 2 cut 3 none),
  G due us (Stage2).

2026-11-24 (sw8), the Cherrygrove window, mailbox and NPC shadow at Run 2x / 4x (the test iPhone, HeartGold by day, Exact, build 9ddec9c54: in
in-betweens the house's front window drew apart, the mailbox's flag and post and a man's shadow beside him, half a keyframe's scroll ahead).
New plays ss-cherry-run2 / ss-cherry-run4 (SoulSilver ss-r30, the road below the mailbox house, B + right at BWPORT_RUNSPEED 2 / 4, Exact; the
Fast renderer records the same lists). The cause is one rule in three places: vc_ip_pair judged "the same object" by a pair's plain screen
displacement, as if the camera stood still. At Walk 1x it moves 2-4 px a keyframe; at Run 2x / 4x 8-16 px, as far as copies of one model drawn
side by side are apart:
- repair_runs (Run 4x, every keyframe 11005-11015): a run of one key, the mailbox's flag and post (three quads 4-9 px apart) or the two halves of
  the man's shadow (10 px apart), was aligned again to its cheapest plain displacement: each copy with its neighbour, two lists alone and drawn at
  the new keyframe's place. A_MISPAIR + C_MISSED + C_VANISH.
- pair_reordered's camera motion was the median of list_shift over every paired list, and most of a town's map pieces lie off the screen,
  clamped to the band with no motion (310 of 438): it said 0 while the camera moved 16.4 px, over IP_REORDER_PX, so the lists of the house whose
  culled copy shifted the order (the window's frame, its glass) stayed alone (Run 4x 11015: C_MISSED).
- the LCS's first alignment pairs one copy off when a copy is culled; at 44 px that stayed under the 48 px jump limit (Run 2x 11027: the front
  window's glass paired with the culled copy's roof window, its own glass left alone: A_MISPAIR 39 px).
Fix (p23_interp.c, vc_ip_camrel; BWPORT_INTERP_CAMREL=0 and the sweep variant `nocam` = the old rules, the same output byte for byte):
cam_motion gives the camera's screen motion at each list (the median motion of the paired lists of another key within 64 px, else of the whole
screen's, on vertices inside the band only); align_strict makes the strict pairs again as the order-keeping alignment of least motion beyond
the camera's (lists <= 64 px; with every cost 0 it is the LCS); repair_runs, the jump drop before pair_reordered and the jump filter take
list_cost = the least of the motion beyond the camera's and the plain displacement (free for what stands still in the world and for what
stands still on the screen, the player's and the follower's shadows); pair_reordered takes the camera's motion at the list.
Before / after (35 plays recorded on the Mac: every play of the set; vcore_ipsweep from 9ddec9c54 against this one):
ss-cherry-run4 A_MISPAIR 53 -> 31, C_MISSED 22 -> 2, C_VANISH 21 -> 1; ss-cherry-run2 A_MISPAIR 21 -> 20 (max vis120 39.3 -> 4.6), C_MISSED 3 -> 2,
C_VANISH 1 -> 0; cherry-door-west A_MISPAIR 1 -> 0, C_MISSED 6 -> 2, C_VANISH 12 -> 5, C_APPEAR 10 -> 7; every event of the 32 other plays
unchanged (Black, White, Black 2, White 2, Platinum, the HG / SS plays, the battles). Left in the run plays: the player's and the follower's
shadows (A_MISPAIR 31 / 20, C_MISSED 2, the same events before and after): they stay put on the screen, and at Run 4x the camera moves one
tile a keyframe, so the follower's shadow stands where the player's stood and the scorer's ref (the map's motion) calls the right pairing a
mispair; C_VANISH 1 is a list the game stops drawing at x 2. In the app (Exact and Fast, 120 Hz in-betweens, base 9ddec9c54 against this):
the detached flag, the shadow beside the man and the broken window (Run 4x 11005-11015, Run 2x 11027) are gone. vcore_tests 426 (a new
test: p23_camera_motion_keeps_copies_paired). Cost: vcore_ipsweep over hugh-fight's 501 captures 0.94 -> 0.99 s.
Not done: an exact identity per list from the game. NNS_G3dDraw's render object is not one: HeartGold's map props share one per model
(field/map_prop_manager.c, GetOrAllocRenderObjByID), so it would take a tag per game object (map props, field objects, shadows, effects, in
four engines) carried into the GX stream (a pseudo-command libntr's G3, the vcore tape and the captures know). Without it the pairing stays a
judgement of order and motion, and a coincidence of both (a copy exactly where another stood, moving like it) cannot be told apart.

2026-11-24 (sw7), the Cherrygrove "building jump" (the test iPhone, HeartGold at night walking east at Walk 2x: in single in-betweens the
house's window frame drew apart from its glass, then the flower box a whole keyframe ahead of the house): C_MISSED. When the game stops
drawing one copy of a model at the screen's left edge (ss-cherry-night: a house at x -30 at 11049, a flower box at x -8 at 11053), the order
passes paired each later copy with the copy before it (64-164 px away); the displacement filter dropped those pairs and nothing paired the
lists again, so the copy in view drew at the new keyframe's place (7 lists of the house front, the window's frame among them; 8 flower-box
lists). vc_ip_pair now drops a jumping pair before pair_reordered, which pairs such a list by key and position. ss-cherry-night (new play,
SoulSilver: the same map and code) C_MISSED 17 -> 2, C_VANISH 15 -> 0; cherry-door-west 15 -> 6 / 21 -> 12 (the same house model walking
west at 1x, 11061); Black route1-battle C_MISSED 3 -> 0; every other count unchanged (Mac recordings of cherry-door-east/west, the HG / SS
plays on SoulSilver saves, town-lab, lab-out, route1-battle, ss-trainer). In the app (Fast 4 samples, 120 Hz, tracked shots): the window
6 px (window px, 3x) ahead of the house at the in-between before 11049 and the flower box 5-7 px before 11053 -> both move with the house.

2026-11-23 (sw5), the Black battle-intro gray band and the scene cut:
- The band (Route 1 frame 468, in the in-between only) was the particle-run re-pairing: the swirl is nine 34-vertex rings 150-350 px big
  (one key), each ring one step from the next, and repair_runs paired ring i with ring i+1. repair_runs now skips a run with a list over 64 px
  (route1 468: all 24 lists pair by order, B_TEXSTEP 48 -> 0 in Black and White, A_MISPAIR 1 -> 0, C_VANISH 18 -> 6).
- vc_ip_scene_cut (p23_interp.c, used by IpGather and vcore_ipsweep): a pair leaving >= 30 lists alone and >= 1/12 of them is a cut: E_CUT
  "scene", not scored. STAT has `scene` and `unpaired_max`; `vcore_ipsweep --pairs` prints a PAIR line per keyframe pair. A_OFFMOTION is not
  scored for a loose-paired list over 512 px. Before/after (32 plays): C_MISSED B2 478 -> 0, W2 21 -> 0; C_VANISH B2 141 -> 8; A_OFFMOTION
  Black 93 -> 60, HG/SS 151 -> 132; scene cuts: B2 80 (hugh-fight 10, r19 58, docks 12), W2 8.
- The B2 / W2 overworld and battle scenes have a STEADY ~170-320 of ~470-590 lists unpaired every keyframe (removed = added = 169 for 60 pairs in
  a row): the same lists in another order, and most of them (a 3D scene's hundreds of lists) have no vertex in front of the camera at all, so no
  position. The scene rule cut those whole scenes to 30 fps (sweep, same captures: B2 93 scene cuts, W2 68).
  sw6: vc_ip_pair's pair_reordered (p23_interp.c, BWPORT_INTERP_REORDER=0 = off, sweep variant `noreorder`) pairs what the order passes left alone: same
  key, nearest position after the median camera motion of the paired lists (<= 16 px from it), greedy by distance; lists with no vertex in front
  in both frames pair by key in draw order. B2 93 -> 1 scene cut (b2-r19 712, a real transition), W2 68 -> 0; unpaired_max hugh-fight 195 -> 21,
  w2-r19 278 -> 8, docks 97 -> 22. Every other class and game is unchanged (Black band still fixed: route1-battle identical).

2026-11-23 (sw4), the full 32-play set rerun (3 shards; `--variants nohold,norep` scores the same captures three ways, each shard has its own
caps subdirectory: the earlier race, one shard's `--drop` deleting another's captures, is gone):
- heartgold/cherry-door-west: the walk started on a blocked tile (557), now warps to 556: the camera moves (160 pairs moving, was 0).
- A_MISPAIR in HG / SS, the footprint / grass puffs (a run of 4-vertex 32 px quads whose texture cell steps with the age): the re-pairing
  now keys its runs without the texture (rkey_of) and a list left alone costs min(limit/2, half its size) instead of limit/2:
  HG 21 -> 1, SS 15 -> 0 events (Route 29 west 7 -> 0, the trainer battle 8 -> 0); A_OFFMOTION -19 / -19, no class got worse by more than a handful
  (Black A_MISPAIR 0 -> 1 at its lab-out door cut pair, HG 1 left at Violet).
- Looked at, not fixed: Black 2 hugh-fight 266-298 is the area-name banner sliding off plus a cutscene (lists the displacement filter drops: C_MISSED / C_VANISH
  of 400 px lists whose pair would be a banner jump), HG Route 29 11429-11441 and Black Route 1 462-468 are the battle-intro swirl (big loose-paired or fast lists, a gray
  band in the first in-between at Black 468). Next: a scene pair (>= 30 unpaired lists and >= 1/12 of the lists) counts as E_CUT "scene" and is not scored;
  A_OFFMOTION of a loose-paired list over 512 px is an overlay, not scored. Not done yet.

2026-11-23, the first set (HeartGold Cherrygrove east/west past the house door at 547,399 and the block edge at x=544, Black's
town to the lab, Platinum's Twinleaf walk; 1544 every-frame captures, 53 s on one slot with the recording):
- A_MISPAIR: only HeartGold walking EAST: the sand footprints behind the player (a run of identical fading 16 px quads) pair one
  tile off when the oldest dies, 12 px in the first 120 Hz in-between (6 events; walking west 0). The particle-run re-pairing
  (vc_ip_repair_runs) removes all 6 and changes no other count in the set: on by default since (BWPORT_INTERP_REPAIR=0 = old).
- No event on the house, its windows or the flowers in either direction, and none at the block edge (lists 99.9-100 % paired, 0 cuts).
- B_HELD: Black's town, 10 big-list axis holds of 64 texels (atlas steps), no B_HELDRUN (no steady scroll held), no B_TEXSTEP.
- C_APPEAR / C_MISSED / C_VANISH: <= 1.7 px leads of lists entering at the screen edge; the footprints fading out vanish one
  keyframe early (17 px quads, 4 events): as at 30 fps, not a jump.

2026-11-23 (sw3), the set grown to 32 plays over all seven games (Black, White, Black 2, White 2: town, door + scene, Route 1 / 19 grass with the wild battle, docks;
HeartGold and SoulSilver: New Bark door, Routes 29 / 30, Violet, a trainer battle, Cherrygrove; Platinum: Twinleaf door, Routes 201 / 202, Oreburgh, a lake, a wild
battle); 9,500 every-frame captures; the report columns `camera moving` / `camera px` (STAT moving / campath) say whether a play's camera moved at all.
- lv3's hold (the axis hold of a big list): B_TEXSCROLL 102 / 285 / 70 / 30 / 101 / 35 (b2 / black / HG / Pt / w2 / white) -> 0, held instead (BWPORT_INTERP_TEXHOLD=0 = off).
  Verified on HeartGold / SoulSilver: the New Bark door play held a list 55 times on the same axis by the same 8 texels every keyframe (B_HELDRUN: a steady scroll of
  a 500-700 px list, stepped instead of blended). vc_ip_texhold now takes the pair before's steps (pstep / nstep): the same step of the same list as in the pair before is a
  steady scroll and is not held (HG door B_HELDRUN 55 -> 0, the 55 are B_TEXSCROLL now; Black / Platinum counts unchanged: their holds are single steps).
- Open: A_MISPAIR 15 events left in HeartGold / SoulSilver (Route 29 west: 7, the trainer battle: 8) that the run re-pairing does not remove; black / white Route 1 field:
  A_OFFMOTION 60 / 19 (NPC motion?); the battle intros of Black 2 (hugh-fight: C_MISSED 2716, C_VANISH 928) and White 2 (w2-r19: 664 / 132) list whole scenes that
  do not pair (a transition, probably not an in-between artifact: check at the keyframe where the count starts); heartgold/cherry-door-west's camera does not move
  (the walk is blocked, 0 pairs moving). Several shards on one machine must not share one --caps directory (a finished shard's rm -rf removes the other's captures).
