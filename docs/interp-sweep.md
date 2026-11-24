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
- `--repair 0|1`: the particle-run re-pairing (vc_ip_repair_runs, BWPORT_INTERP_REPAIR) off / on, for before/after counts.
- `--strips N`: the N worst events per class as 3x crops OLD keyframe | first 120 Hz in-between | KEY keyframe (vcore_interp,
  which applies the live texcoord hold) in OUT/strips/.

## Classes (port/vcore/tools/vcore_ipsweep.c's header has the exact rules)

err / vis in screen px at 256 x 192 (texels for B_*); vis120 = err x (1 - the first in-between's f) at 120 Hz, vis60 at 60 Hz
(the 30 fps field: gap 2, f = .25 / .5; a 60 fps scene has no in-between at 60 Hz). "ref" = the median motion of the paired lists
within 64 px (the camera).

| class | what | why it shows |
|---|---|---|
| A_MISPAIR | a paired list whose partner is not the same-key list that moves with ref | it sweeps toward another object (one tile off for a run of identical quads) |
| A_OFFMOTION | moves > thr (6 px) from ref, no better candidate | NPC motion, or a loose (shape-only) mispair |
| B_TEXSTEP | blended texcoords change >= 8 texels, not one uniform step | atlas cells mixed (the hold misses it) |
| B_TEXSCROLL | the same by one uniform step | a fast scroll (info: right to blend) |
| B_HELD / B_HELDRUN | lv3's big-list axis hold; RUN = the same list held by the same step in the pair before | a steady 8-texel scroll would be stepped |
| C_MISSED | a KEY list with no partner although OLD has one where ref puts it | drawn at KEY's place: leads by |ref| |
| C_APPEAR | a KEY list with no partner, no candidate | new: drawn at KEY's place for the interval |
| C_VANISH | an OLD list with no partner, its centre on screen | gone one keyframe early |
| E_SPEED / E_CUT | camera px per tick changes by > max(1, half) / a cut pair | judder / a 60 Hz-style jump |

## Findings

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
