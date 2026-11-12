# Booting Black on the PC build: the crash log

How far the Linux exe gets and what each stop was, in order (the run: `tools/port_run.py`,
docs/port-build.md "Running"). One line each: where it stopped, the cause, the fix and where it is.

## Milestones

| reached | frame | screenshot (C:\stress\shots\) |
|---|---|---|
| the copyright screen | 303 | b44a_01_copyright.png |
| the Game Freak logo | 729 | b44a_02_gamefreak.png |
| the opening movie | 2458 | b44a_03_titledemo.png |
| the title screen | 912 (Start at 600) | b44a_04_title.png |
| the start menu: its four items on BG1, but garbage around them | 1458 | b44a_05_startmenu_garbled.png |
| the start menu drawn right (libntr-vram.patch) | 1164 | c45b_02_startmenu_after.png |
| NEW GAME: the professor's intro runs (no crash to frame 6000+), but its script waits at step 7 and the screen shows only the background gradient | 2099 | b44a_06_newgame_intro.png |
| NEW GAME's intro talks: the professor's greeting (the BGM loads into the bigger sound heap) | 4709 | c45c_01_intro_hi_there.png |
| the intro's boy-or-girl question (A pressed every 100 frames from frame 2400) | 4712 | c45c_02_intro_boy_or_girl.png |
| the boy chosen, the line before the name entry; then exit 1 at frame 5331 (stop 14) | 5302 | c45c_03_intro_please_tell_me.png |
| the name entry (the touch keyboard's screen, typed with A: seven As, then the confirm key) | 5358 | c45a_04_name_entry.png |
| the intro after the name: Cheren and Bianca introduced (the dotted lines over the top screen are open) | 6623 | c45a_05_intro_after_name.png |
| the field: the player's room, the first event (Cheren, Bianca, the present; speech bubbles) | 11912 | c45a_07_field_room_event.png |
| walking in the room (arrow keys; the room's 3D map isn't drawn: white) | 18404 | c45a_09_field_walking.png |
| the field menu (X): its items (bag, trainer card, save, options) on the bottom screen | 21678 | c45a_08_field_menu_walked.png |
| the intro's professor and Minccino drawn (MCSS: libntr-g3regs.patch) | 3161 | d46a_01_intro_professor_minccino.png |
| the room's map drawn with its textures (libntr-gxasync.patch, libntrsystem-geflush.patch: stop 20) | 11999 | d47a_01_room_textured.png |
| the gift box opened (down, then A) and the starters: Tepig chosen | 13507, 14054 | d47a_02_starter_select.png, d47a_03_chose_tepig.png |
| the first battle (Bianca's Snivy): the moves hit (stop 21), Bianca beaten, the wrecked room | 18535, 20989, 22447 | d47a_06_battle_damage.png, d47a_07_battle_won.png, d47a_08_room_after_battle.png |
| the second battle (Cheren's Oshawott, its VS intro) won; the line sending the player to the mother; the player alone in the room | 25744-37815 | d47a_m3.png |
| the field menu's save item: the saved-the-game message (stops 22, 23) | 40167 | d48a_01_saved.png |
| from that save (CONTINUE): down the stairs (the first map change), the mother's event with Cheren and Bianca | 3359 | d48a_02_downstairs_mom.png |
| a save downstairs after the event (<scratch>) | 11149 | d48a_03_saving_downstairs.png |
| from downstairs.bin: out of the door at the bottom (down 10, right 3, down held): the first map change (the plain fade), Nuvema Town with the first-exit scene (the sun's flare, Pidove flying off, Bianca walking away); the way back in through a door is the capture fade (field_transition.c, libntr-capture.patch): it runs | 3500 | d49a_01_town_first_exit.png |
| Cheren at the lab's door (left 5, up 8, A): his request to go to Bianca's house; Bianca's house (the bottom left one) and her father's scene, its map untextured (stop 24) | 3513, 5883 | d49a_02_cheren_at_lab.png, d49a_03_bianca_house_untextured.png |
| a save in Nuvema Town outside the player's door (<saves>/town.bin) | 3600 | d49a_05_saving_town.png |
| from town.bin: walking south through Nuvema Town, the map and houses stay drawn (libntr-boxtest.patch, stop 24); back into the player's house and out again: the house's map is drawn but in vertex colours, with no sprites and no props (stop 24b) | 4983, 7038 | d50a_01_town_walk_south.png, d50a_02_house_again.png |
| from town.bin, back into the player's house (down 2, up 2 held): textured, Mom, the player and the props drawn (stop 24b fixed) | 4320 | d51a_01_house_textured.png |
| from town.bin (TZ=Asia/Tokyo for daytime at 03:00-08:00 UTC): Bianca's house (down 9, left 6, up held), textured, her father's scene, the talk balloon on Bianca; saved outside her door (<saves>/bianca.bin) | 2755 | d52a_01_bianca_house.png |
| from bianca.bin (right 6, up 9, left 5, up 7): Cheren at the lab, Juniper's tutorial, the starter's nickname (stop 25), the Pokedex; saved in the lab (lab.bin) | 3600-13300 | d52a_03_lab_saved.png |
| out of the lab: Bianca catches up, Mom gives the Town Map; saved outside the lab (townmap.bin). The trees by the lab drawn as spikes under the scene camera (rendering, below stop 26) | 12700 | d52a_02_lab_exit_spikes.png |
| from townmap.bin (right 11, up 12, A every 90 frames): the three step onto Route 1, Juniper's catching demo (stop 26): the caught message, the Poke Balls; saved on Route 1 below the grass (route1.bin) | 7654-15200 | |
| from route1.bin with the seed fixed (<scratch>): the first wild battle (Lillipup), Tackle, the bag's Poke Ball: Lillipup caught, registered, nickname screen (START, A); saved in the grass (caught.bin) | 3900, 6400, 11500 | d52a_04_wild_battle.png, d52a_05_caught_lillipup.png, d52a_06_saved_route1.png |
| from caught.bin with rng.gdb (up 30, right 4, up 16, A 3, B 3, left 1, up 20, A every 90 frames): north on Route 1, Cheren in the grass, Bianca's Pokedex talk, the Xtransceiver call (its faces garbled: rendering, below stop 26), Cheren's parting line; the player free at the north end of Route 1 (not saved) | 8000, 10000 | d52a_07_xtransceiver_garbled.png, d52a_08_route1_north.png |

Keys for the run: `tools/port_run.py -t 70 --keys 'start@f600 start@f900 start@f1300 a@f1700'` (the movie skipped, the
title, the start menu, new game).
To the field: `--fast --fresh -t 105` and those keys, then `a@f2200` and an A every 90 frames to frame
15000 (the intro's texts, the boy, the name of seven As and the confirm key, the room's first event), `down*0.5@f16000
left*0.5@f17000 up*0.5@f18000 right*0.5@f19000 x@f21000` (walking, the field menu); <scratch>
writes that list. `--fast` (no 60 fps cap) runs it at about 210 frames a second here.
To the battles: <scratch> (d46a's: the intro's keys, an A every 90 frames from
2200; EXTRA= adds keys): `EXTRA="down@f12700" runk.sh 400 40000 --fast` faces the gift box after the room's event
(frame ~12600), and the A presses choose Tepig, fight both battles with Tackle and end with the player alone in the
room at frame ~38000 (about 190 frames a second).

From a save: the run dir's save.bin persists; keep copies (<scratch>: the wrecked
room after both battles; downstairs.bin: downstairs after the mother's event) and copy one to build/port/run/save.bin
before a run. `BWPORT_KEYS="start@f600 start@f900 start@f1300 a@f1700 ..." python3 tools/port_run.py --fast` is
CONTINUE (the field from ~f2000); the exe presses the keys frame-exact (port/src/keys_script.c: x, y, and holds in
frames, `right*f30@f3000`). From room.bin: up 2, right 5 (the plant stops it), up 2 (to the wall), right 2: the
stairs (<scratch> and walk.py write the taps). The field menu: X, then a direction shows the cursor
on POKEMON; right, down is SAVE; A, A (YES).

## Stops and fixes

1. SIGSEGV in OS_CreateExtraHeap (os_alloc.c:709) from GFLAppInit (gfl_use.c:112): libntr put the TWL parameter
   buffer, an NTR game's extra heap, at the DS's raw 0x02000000. p43c's libntr-x86-parameter-buf.patch puts it at
   s_HW_MAIN_MEM's start (below the main arena, which libntr starts at 0xe3e0); 32 KB, not 16, for heap 9's doubled
   WORK_AREA_SIZE(0x3f00).
2. SIGSEGV in OS_InitAlloc(OS_ARENA_DTCM) from GFL_HeapDTCMInit (heap_dtcm.c:32): the DTCM arena's bounds were empty
   functions' addresses cut to 32 bits. p43c's port/src/linker_syms.c gives the DTCM and ITCM arenas host buffers.
3. SIGSEGV in libntrsystem's AllocFromTail (expheap.c:339) from GameInit's save init (func_0200c058 ->
   GFL_MsgSysLoadData): heap 7's free list overwritten. The 69 save blocks overran the 0x24000-byte save area by 156
   bytes: TimeSigSave (0x660, Black 0x658), AdventureSave (0x68, 0x5c) and TrainerGameInfoSave (0xf0, 0xec) have an
   s64 at a 4-byte boundary, which x86_64 aligns to 8 and MWCC to 4; RivalDataSave (0x100, 0xfc) has an `unsigned
   long`, 8 bytes on Linux. Fix: port/include/types.h aligns the game's s64 and u64 to 4 (as include/types.h does for
   the tools' clang), and `sdk_u32` (include/types.h: unsigned long on the DS, u32 on PC) for the count. Every block's
   size is now Black's (checked against the ROM's getSize functions).
4. exit(1) in GFL_HeapDumpOnFailure from GFL_NetErrInit (net_err.c:201): heap 8 full. p43f's heaps twice their size on
   PC (WORK_AREA_SIZE in heapsys.c, gfl_use.c, fieldmap_proc.c, pokemontrade_proc.c).
5. Stub GFL_G3DSysCheckSwapBuffers from main.c:76: g3d_system.c was excluded. b44c's commit compiles the 7 todo files.
6. Hang at frame 1 in libntr's OS_WakeupThreadDirect from main.c:88: the loop wakes the BGM loader g_GFLSndThread every
   frame, made or not, and libntr's wakeup waits for a host thread that a thread never made doesn't have. Fix:
   port/src/main_threads.c, PORT_WakeupThreadDirect and PORT_SleepThreadDirect change a never-made thread's state only
   (as on the DS, where it is in no run queue).
7. SIGSEGV at frame 355 in libntr's DrawBGLine_Extended (sim_g2.cpp:1133), the Game Freak logo's direct-colour bitmap
   BG: the pixel, a colour, was used as a palette index. Fixed in b44d's libntr-hw.patch (the same line).
8. Hang at frame 4650 (the title screen) in G3X_GetVectorMtx from libntrsystem's NNSi_G3dFuncSbc_NODEMIX: GXSTAT read
   0xfffffffd, its busy bit set by G3_MultMtx33's 36-byte copy to the 4-byte FIFO variable. Fixed in b44d's
   libntr-hw.patch (the matrix sends as simulator commands); libntr-boot.patch makes G3X_GetVectorMtx return the
   simulator's current vector matrix (nothing wrote the VECMTX_RESULT registers: the envelope's normal matrices were
   zeros).
9. (Not a stop: the start menu's garbage.) BG1's empty tiles and BG0 showed the opening movie's bitmap, left in BG
   VRAM: the menu's own layers (BG1 text, BG2 frames, BG3 Poke Ball) were right when shown alone. GFL_BGSysInitVRAM
   clears VRAM through the LCDC mapping, and libntr kept s_HW_LCDC_VRAM apart from s_HW_BG_VRAM/OBJ/sub arrays, so
   the clear never reached the BG array. Fixed in (c45b): libntr-vram.patch moves a bank's data between its
   LCDC part and the BG/OBJ arrays when VRAMCNT maps it elsewhere (docs/port-runtime.md); the start menu is drawn
   right (c45b_02_startmenu_after.png; before: c45b_01_startmenu_before.png).
10. SIGSEGV at frame 1732 in GFL_HeapFree(NULL) from intro_particle.c:44 (GFL_G3DCameraFree of the particle system's
   camera): the SPL manager's emitters and particles (0x88 + 24 x 0xd0 + 200 x 0x50 = 21128 bytes on x86_64, 17420 on
   the DS) overran the intro's 0x4800-byte work and zeroed the ParticleSystem after it. Fix: the work
   WORK_AREA_SIZE(0x4800) in intro_particle.c and fld_particle.c (matching); the other 9 callers' works too
   ("Fixed buffers" below).
11. SIGSEGV at frame 1735 in libntrsystem's NNS_G2dUnpackNMC (g2d_NMC_load.c:74) from GFL_G2DIOReadNMCR (mcss.c:1030,
   the intro's first Pokemon): libntrsystem unpacks a multi-cell bank (NMCR) in place, its 4-byte file offsets read as
   8-byte pointers. libntrsystem has PC converters for NANR and NCER (host structs, WIN_* file views), none for NMCR.
   Fix: libntrsystem-boot.patch (the wrap applies it) builds a host bank and multi-cells pointing into the file's
   hierarchy data, user cell attributes as NNS_G2dUnpackNCE's PC path copies them. The copy (one malloc) is recorded
   with the file's address and freed with the heap block that holds the file: NNS_FndFreeToExpHeap calls
   WIN_CheckAndFreeMultiCellBank, as it does libntrsystem's NANR/NCER/NCGR checks (a child heap's memory going back
   to its parent, GFL_HeapDelete, frees the copies in it too); a bank unpacked again at the same address replaces
   the old copy. Test: port/tests/run_nmc_test.sh (real NMCR files of a/0/0/4, user attributes, the frees).
12. SIGSEGV at frame 1852 in intro_mcss.c:148 (the MCSS animation-end callback) from libntrsystem's
   SequenceEdgeHandleCommon_: NitroSystem's animation callback parameter is u32, and the game passes a pointer. Fix:
   libntrsystem-boot.patch makes it NNSiG2dAnmCallBackParam (uintptr_t on PC) in NNSG2dAnmCallBackPtr, the functor
   and NNS_G2dSetAnimCtrlCallBackFunctor(AtAnimFrame); egg_demo_view.c and zukan_detail_form.c's callbacks take
   uintptr_t and pass the cast as mcss.c does (matching).
13. Stall (not a crash) after NEW GAME: the intro's script (intro_cmd.c, cmd->pc 7) waits forever in its 7th command
   (data_ov204_021c0248+144), INTRO_CMD_WAIT_BGM_PLAYING; the screen keeps the background gradient. The BGM
   (SEQ_BGM_STARTING2) never started: the sound heap (snd_sys.c, 0x95ad0) had 174144 bytes left after the start-up's
   loads, too few for the BGM's sequence, bank and wave archives (with them 639424 bytes are in use): its block
   headers hold pointers (0x60 bytes each on x86_64, 0x20 on the DS). Fix: the sound heap
   WORK_AREA_SIZE (matching); the intro goes on to the professor's first text box.
14. Fixed (c45a, libntr-vcount.patch): exit 1 at frame 5331 (keys: the run above, then `a@f2400` ... every 100 frames) in GFL_HeapDumpOnFailure
   from GFL_HeapDelete(111) at the intro's end (func_ov204_021bdae4): heap 111 still has one block, the MCSS
   system's shadow palette file (arc 11 file 88, GFL_G2DIOReadNCLR from mcss.c:200). Cause: libntr never advances
   the V-count (s_reg_GX_VCOUNT is only ever set to 0, sim_main.cpp's SIM_PostRenderVBlank), and mcss.c's load tasks
   (func_02016880, func_0201691c) wait for GX_GetVCount() in 192..200, so no MCSS graphics ever load (the professor
   and Minccino are missing in the intro's shots) and their files are never freed. Next: a V-count that reads as
   the V-blank's lines while GameUpdateVBlank runs (a libntr patch: GX_GetVCount from the time since the V-blank
   interrupt, or 192 from the interrupt until the main loop's next OS_WaitIrq); 7 more game files read it
   (wipe_sub.c, field_g3d_mapper.c, zknlist_main.c, zknsearch_main.c, gsync_disp.c, un_select.c, pmsiv_edit.c).
   The fix: the V-count runs with the time from the V-blank, 192 at the V-blank interrupt and again where OS_WaitIrq
   returns (the game's V-blank work starts), a line every 63.555 us (263 lines at 59.8261 Hz), 0 after 262, held at
   191 when a frame runs long (sim_main.cpp's SIM_GetVCountReg; reg_GX_VCOUNT reads through it in both X86
   ioreg_GX.h). The intro's heap is empty at its end now. Also (port/src/boot_exit.c, port_prefix.h): the game's
   exit() is MSL's, which only records the status and returns, as Black's; the host's exit ended the program at the
   heap's debug dump, where Black goes on (the dump itself is compiled out of Black).
15. SIGSEGV at frame 9540 in func_ov010_02161f14 (city_state.c:167) from EventFieldFirst_Create (event_mapchange.c:233),
   the first field entry: the city's resident info pointer cut to 32 bits by resh_zek_savedata.c's func_0200a898,
   defined `s32 f(s32)` where game_data.c declares `void *f(void *)`. Fix: both (and func_0200a890) take and return
   pointers (matching).
16. SIGSEGV at frame 9762 in libntr's GetWaveData (snd_bank.c:523) from the sequencer in SDL's audio callback, entering
   the field: a player went on playing with a bank the BGM change had freed (its memory then held wave data). libntr's
   ARM7 side (sim7/src/pxi.c) dropped SND_COMMAND_INVALIDATE_SEQ/BANK/WAVE, which NitroSystem's heap sends before it
   frees (the players and channels that use the memory stop), and ran the sound commands in the PXI thread while the
   sequencer runs in the audio callback. Fix: libntr-snd.patch runs the invalidations (sim7 has
   SND_InvalidateSeq7/Bank7, SND_InvalidateWave), skip, player and track parameters, track mute and allocatable
   channels, all under SDL_LockAudio.
17. (Not a stop: the field's 3D every other frame.) The field runs at 30 fps and swaps the 3D buffers every other
   frame; libntr composited its render target, which it clears after a swap, so every other frame had no 3D (the
   actors, billboards in 3D, gone in about half the screenshots). Fix: libntr-3dswap.patch keeps a copy of the render
   target from the last swap and composites that (the DS shows the last swapped frame until the next).
18. Open (rendering, not a stop): the room's 3D map isn't drawn in the field (the top screen is the clear colour, the
   actors on it), and the intro's MCSS sprites (the professor, Minccino) aren't drawn though they load now (stop 14);
   the 3D models (the boy and girl, the title's) are. MCSS draws its cells as textured 3D quads with its images
   loaded by NNS_G2dLoadImage2DMapping to NNS_G2D_VRAM_TYPE_3DMAIN (texture VRAM) and palettes to the texture palette
   slots; the field map is field_g3d_map.c's NSBMD blocks. Also: dotted vertical lines over the intro's top screen
   after the name entry (from c45a_05 on), and the field's bottom screen (dark teal with lines) before the menu.

19. Fixed (d46a, libntr-g3regs.patch; the MCSS half of 18): the intro's professor and Minccino loaded but showed
   nothing. mcss.c draws each cell as a textured quad and writes the texture coordinates straight to the register
   (`reg_G3_TEXCOORD = st0;` before each G3_Vtx, as the DS's geometry engine takes them); libntr's registers are plain
   variables, and only G3_TexCoord sends a command, so every vertex of every MCSS quad had the last coordinates sent
   (one transparent texel). The fix: sim_g3_CommandProcessor.cpp's vertex command first takes a TEXCOORD or NORMAL
   register value that differs from the last one sent (the TEXCOORD and NORMAL commands write the register too).
   Other direct writes in the game: pokemontrade_3d.c (reg_G3_TEXCOORD, reg_G3_NORMAL), covered the same way.
20. Fixed (d47a; the map half of 18; the cause at the end, d46a's measurements first): the room's map IS drawn:
   FieldChunk_RenderMap runs for its one chunk (active, its model loaded, the box test passes,
   GFL_G3DSysDispatchDraw called), the area's shared textures (mapTextures,
   GFL_G3DResUploadAndReleaseTexData) are bound (NNS_G3dBindMdlTex and NNS_G3dBindMdlPltt return TRUE; texInfo and
   plttInfo LOADED; the 19 materials' texImageParam hold their VRAM offsets, e.g. 0x08930b08). Fog is off
   (DISP3DCNT 0x3039) and the clear colour black, so the white is the map's own polygons drawn without a texture
   (their vertex colours). Measured in libntr: of the TEXIMAGE_PARAM commands that reach the simulator while the map
   draws, most are 0 (no texture) and a few are real values (0x04030b36, 0x08930b3e, 0x2c930ba6); the textured draws
   all use one 8x8 A3I5 texture. But libntrsystem's sbc.c (NNSi_G3dFuncSbc_MAT_InternalDefault) packs the right
   values for all 19 materials (prmTexImage == the material's texImageParam, not cached, no material animation; only
   material 14 has 0): the values are lost between NNS_G3dGeBufferData_N and the simulator. Look there next:
   gecom.c's SDK_PORT NNS_G3dGeFlushBuffer never sets NNS_G3dGeBuffer->idx back to 0 (the DS branch does), and
   NNS_G3dGeBufferOP_N appends to the buffer while NNS_G3dFlagGXDmaAsync is set (NNS_G3dGeSendDL sets it for display
   lists of 256 bytes or more, the map's shapes; MI_SendGXCommandAsync's callback clears it); then the DRAW_CMD_G3_CMD_LIST
   parsing of packed commands in sim_g3_CommandProcessor.cpp. Also found in libntr's simulator, not yet fixed:
   G3SIM_TexPlttBase shifts the base by the format current when the command arrives (the DS applies the format of
   the polygon), getTextureVramBank returns bank A for GX_VRAM_TEX_0_D and can't do the non-contiguous slot sets
   (AC, AD, BD, ABD, ACD), GX_TEXFMT_COMP4x4 isn't decoded (SIM_assert_always, the texture left transparent: many
   field maps use 4x4 textures), and the fog shader's interpolation between table entries is backwards.
   The cause: MI_SendGXCommandAsync on PC sent the list to the simulator at once but left the end of the
   transfer to the DS's GXFIFO interrupt and DMA callback, which never come, so its callback (gecom.c's simpleUnlock_)
   never ran and NNS_G3dFlagGXDmaAsync stayed set after the first display list of 256 bytes or more. NNS_G3dGeBufferOP_N
   then appended every command to the G3D command buffer, and the PC branch of NNS_G3dGeFlushBuffer sent the buffer
   without emptying it: once it was full, every later command and display list went after the whole stale buffer, whose
   TEXIMAGE_PARAM commands replaced each material's texture. Only the field sets up a command buffer (GFL_G3DSysInit's
   DTCM block, as the title does; the fly-over passes 0), so only those lost their textures. Fixes: libntr-gxasync.patch
   (MI_SendGXCommandAsync and ...AsyncFast on PC send with MI_SendGXCommand and run the callback then) and
   libntrsystem-geflush.patch (the PC flush sets the buffer index back to 0). The title's 3D legendary now draws, but as
   a sliver from the screen's centre (another bug, open).
21. Fixed (d47a): every move of the first battle showed the move-failed message (func_ov093_021bec58's empty-targets check
   after the target set was filled right). BtlFlowWorkFrame kept its 7 mon sets as `u8 monSets[7][0x48]`, BtlMonSet's
   DS size; its 6 BattleMon pointers make it 96 bytes on a PC target, so each set's tail (count, countMax, getIndex,
   numTargets) lay in the next set, and clearing a set emptied the one before. BtlServerFlow's unk1A68 (the mons
   switched in this turn) had the same size, and its clear overran into unk1AB0, unk1AB4 and posPoke. Fix
   (btl_handler.h, matching): both are BtlMonSet. Also seen, not checked: the win message's wording (it names the player and the trainer).
22. Fixed (d48a): SIGSEGV at frame 33830 in GFL_HeapAllocate (a garbage heap ID) from no_gear.c's func_ov030_021efecc,
   YES on the field menu's save prompt. field_subscreen.c's func_ov021_021a1524 and
   func_ov021_021a1554 call the report screen (overlay 34) at 0x021f0058 and 0x021f009c, but named overlay 30's
   (no_gear.c) and 29's (c_gear_sw.c) functions at the same addresses: overlays share load addresses, the DS branch
   goes to whichever is loaded, so both names build the same ROM, while on PC each name is its own function. Fix: the
   overlay-34 names (matching). The same mistake in encounter_effect_ripple.c (overlay 73's names for overlay 75's
   capture events). A scan for it: <scratch> (calls to a function whose address another overlay
   shares, where the file also calls that overlay); the rest it lists looked right (gym script commands, union room,
   battle video, mystery gift beacons, DWC), not proven.
23. Fixed (d48a, libntr-waitintr.patch): hang at frame 33832 in OS_WaitInterrupt from FldActSys_ForceFullSync (the save
   changes the player's sprite and waits a V-blank at a time for its textures). libntr's V-blank is the frame
   OS_WaitIrq draws (SIM_Render runs the V-blank interrupt and sets the check flag); OS_WaitInterrupt had no PC branch
   and spun forever. It draws the frame now too when it waits for a V-blank (pokemontrade_3d.c waits there as well).
24. Fixed (d50a, libntr-boxtest.patch): Nuvema Town's map vanished a step south of the player's door (from town.bin,
   `bash <scratch> town 25 down6`: frame 1502, the first step). The field culls each map chunk and each
   building with the geometry engine's box test (field_g3d_map.c FieldChunk_CheckModelBBox, g3d_system.c
   GFL_G3DActorTestBBox); libntr's G3SIM_BoxTest is melonDS's face clipping with the ClipSegment calls commented out,
   so a clipped vertex was counted but left as whatever the stack held: the answer changed with the stack. The first
   step culled the chunk in view (and all 8 of its houses) and let an empty one through. Found by counting, per
   chunk dispatch, the polygons sent and those on screen (a temporary flag around GFL_G3DSysDispatchDraw read in
   libntr's G3SIM_SubmitPolygon). Now the clipped vertices are computed, and a face past the far plane is outside
   unless the polygon attributes latched at BEGIN set far clipping (DS/melonDS; both Black callers set it).
24b. Fixed (d51a, port/src/sdk_mi.c): every building entered from Nuvema Town drew in vertex colours, without the
   player, NPCs or props (from town.bin `r.sh town 30 down2 up2:f20 w600`). The renderer got its textures (every polygon
   had a format) but read zeros from bank A: a hardware watchpoint on the house's texture in VRAM caught
   field_transition.c's building-entry fade (func_ov021_021bef8c, state 1) clearing HW_LCDC_VRAM, 0x20000 bytes, with
   only bank D set for LCDC. The ROM has 0x06800000 too (bank A's part of the LCDC view; bank D's is 0x06860000): on the
   DS a store to a bank not set for LCDC is lost, so it does nothing there. libntr's LCDC view is the banks' own memory,
   so it wiped the textures loaded for the house just before (map, props, the sprites' billboards). MIi_CpuClearFast
   (port/src/sdk_mi.c) now skips the parts of the LCDC view whose banks aren't set for LCDC (GX_GetBankForLCDC). The
   other clears of HW_LCDC_VRAM in src/ and lib/ set all banks for LCDC first (GFL_BGSysInitVRAM's callers all pass 0).
   Other writers (libntr's MI_Cpu*/MI_Dma*, plain stores) still land in any bank: a texture or BG that goes blank after
   a scene's clear -> check the banks set for LCDC at that write.
25. Fixed (d52a, scrcmd_vm.c): SIGSEGV in VM_Read8 from VM_Run in Juniper's lab, right after the starter's
   nickname (frame 6961 from bianca.bin). s001E_VMJump (and s0004_VMCall, s001F_VMJumpIf, s0020_VMCallIf) added a u32
   offset to the script PC: on the DS a backward jump wraps, on a 64-bit target it lands 4 GB past the script. Found by
   a gdb breakpoint on vm.c's command dispatch printing cmd, pc - script base and the command's function (the last
   command was a jump). The offsets are s32 now, also the actor command list's (scrcmd_fldmmdl.c) and the elevator
   table's (ov021_021b69b0.c); tr_ai.c and btlv_effvm.c had s32/int already. ROM unchanged.
26. Fixed (d52a, btlv_clact.c): SIGSEGV in func_0204b570 (clact.c) from the battle view's scale task during Juniper's
   catching demo on Route 1 (frame 7654 from townmap.bin; the ball's actor). func_ov094_02201b20 reached entry index as
   chars[index * 4] / actor[index * 4] from entry 0's fields: 16 bytes on the DS, but the entry (a ClActor pointer and
   three u32s) is 24 bytes on a 64-bit target, so a second actor's resources and pointer landed in the next entries.
   The stride is the entry's size in u32s / pointers now (4 on the DS: ROM unchanged). The same trick elsewhere: grep
   `= &x[0].field;` followed by `[index * N]` (none else in src/ and lib/ today).
   Open, rendering (libntr's renderer is retired for port/vcore; not fixed there): G3SIM_SubmitPolygon divides by W
   without clipping, so a polygon with a vertex behind the camera (W <= 0, cut to 24 bits) draws as spikes across the
   screen: the trees by the lab under the scene camera after the lab (frame 12700 from bianca.bin, d52a_02), Route 1's
   entrance scene. Clipping in clip space against the near, left/right and top/bottom planes before the divide fixed it
   (draft: C:\stress\d52a\clip.py, d52a_02b_lab_exit_clipped_draft.png); port/vcore should clip as the DS does. In
   the battles, the camera's idle pan shows a black rectangle over the left of the top screen for a few frames
   (route1.bin battle, frames ~3800 and ~4800).
   The Xtransceiver call on Route 1 (frame ~8000 from caught.bin, d52a_07): the four faces on the top screen are drawn as
   coloured outlines and noise, not pictures (the bottom screen's device is right).

## Fixed buffers handed to libraries

A work buffer of a fixed size that the game gives a library (NitroSystem, lib/spl, the particle system, libntr's WM and
MB) outgrows its DS size on a PC target when the library's structs in it hold pointers. (c45c) sized them
with WORK_AREA_SIZE (include/work_area.h: twice Black's size on a PC target, Black's on the DS; the ROM is unchanged):

| buffer | library | where |
|---|---|---|
| the particle systems' works (0x4800, plist_demo's 0x5000): the SPL manager, emitters, particles, resources | particle.c, lib/spl | intro_particle.c, fld_particle.c, mystery.c, plist_demo.c, btlv_effvm.c, comm_btl_demo.c, pokemontrade_3d.c, include/demo/demo_particle.h (shinka_demo_effect.c, egg_demo_view.c), include/demo/ddemo_main.h, include/app/musical/sta_act_effect.h |
| the sound heap (0x95ad0): its block headers, 0x60 bytes each on x86_64 (0x20 on the DS); too small for the intro's BGM on a PC target (stop 13) | libntrsystem snd | snd_sys.c |
| WM's system buffer (0xf00): libntr's WM_SYSTEM_BUF_SIZE is 0x1e00 on a PC target | libntr wm | wih.c |
| MB's parent work (0xb100): MBiParam, MB_CommPWork and WM's buffer | libntr mb | mbp.c |

Checked and right as they are (the size comes from the library or sizeof): the TCB managers (GFL_TCBMgrCalcAllocSize,
all 21 callers), the multi-cell animation works (NNS_G2dGetMCWorkAreaSize), the linked-list VRAM managers' works
(NNS_GfdGetLnk*VramManagerWorkSize), the VRAM transfer tasks and cell transfer states (typed arrays), the 3D
animation objects (NNS_G3dAllocAnmObj; field_ground_anime.c's fixed one had WORK_AREA_SIZE from p43f), the geometry
buffer (no pointers), the curves' load buffers (VecFx32 keyframes), the FS table (FS_TryLoadTable), the game threads'
stacks (port/src/main_threads.c ignores them), the heaps (p43f). Left alone: dwc_rapcommon.c's and dwc_rap_vchat.c's
(online, stubbed). libntr's WM and MB still cut pointers to 32 bits (wm_system.c's WmInitCore: `(u32)wm9buf + ...`),
so local wireless can't run on a 64-bit target until libntr is fixed.

## Resource loaders on 64-bit

NitroSystem's loaders unpack a file in place: the 4-byte offsets in its structs become pointers. A host pointer is
8 bytes, so libntrsystem's PC paths build host copies of the structs instead (WIN_* views of the file), pointing into
the file. `python3 port/tests/res_load_test.py` (port/tests/res_load_test.c, under ASan, built from the PC build's
compile commands) loads every NitroSystem and SPL file of the ROM (its NARC members, LZ10/LZ11 undone, duplicates
once: 25k files) through the loaders and checks each result against its own parse of an untouched copy, again after
all files of a kind are loaded, then frees them through the heap hook. Measured 2026-10-28:

| format | loader | before | now |
|---|---|---|---|
| NCGR (7493) | NNS_G2dGetUnpacked(BG)CharacterData, ...CharacterPosInfo | OK | OK |
| NCLR (3312) | NNS_G2dGetUnpackedPaletteData | broken: the colours were copied to 64 ring slots of 1 KB (later palettes overwrote them; an 8 KB extended palette overran into the next slots); 3250 of 3312 wrong once all were loaded | fixed: a host struct pointing at the file's colours |
| NCLR | NNS_G2dGetUnpackedPaletteCompressInfo | ring of 64 structs | fixed: a host struct |
| NSCR (570) | NNS_G2dGetUnpackedScreenData | OK (no pointers) | OK |
| NCER (3061) | NNS_G2dGetUnpackedCellBank | cells OK; VRAM transfer data and user attributes in rings of 256 (one slot past the end, 128 cells at most) | fixed: one host block |
| NANR (2953), NMAR (1054) | NNS_G2dGetUnpacked(MC)AnimBank | broken: pExtendedData (and pStringBank) left as malloc gave them when the file has no user attributes (205 + 20 files here; GFL's clact.c reads it through NNS_G2dGetUserExAnimAttrBank); attributes in rings of 200 | fixed: NULL; one host block |
| NMCR (2809) | NNS_G2dGetUnpackedMultiCellBank | OK (boot.patch, stop 11) | OK, one block per file (boot.patch) |
| NENR | NNS_G2dGetUnpackedEntityBank | in place: broken on 64-bit | not fixed: no NENR in the ROM, no caller |
| NFTR (5, a/0/2/3) | NNSi_G2dGetUnpackedFont | crashes on the ROM's fonts | not fixed: the game's fonts go through GFL's own code, not NitroSystem's |
| NSBMD (411), NSBTX (1254), NSBCA/TP/TA/MA/VA (258/112/187/69/31) | NNS_G3dGet*, dict lookups, NNS_G3dTexSetTexKey, NNS_G3dPlttSetPlttKey, NNS_G3dBindMdlTex/Pltt | OK: G3D resources are offsets throughout; 408 of 408 models bind fully to their own file's textures | OK |
| SPA (768) | lib/spl's SPLManager_LoadResources (func_02052a50) | OK: the file structs have the DS sizes on x86_64 (each texture lands on its " TPS") | OK |

The fix, port/subprojects/packagefiles/libntrsystem-load64.patch (after boot.patch): each loader's host copies in
one malloc'd block, registered by the file they came from (WIN_G2dRegisterUnpacked, g2d_Load.c) and freed when
NNS_FndFreeToExpHeap frees the heap block holding the file, which is when the data dies on the DS. Before, the key
was the caller's pointer variable, in 2048/4096-entry tables that stopped registering when full, and a free released
one bank per heap block (clact.c keeps all its cell banks in one array). The frees run 64 frees late, so a use right
after the file's free (harmless on the DS) stays harmless; a lock, since the game's threads are host threads.
Not checked: sound (SDAT, libntr's SND has its own WIN_* bank structs) and G3D animation objects bound to models.

## Frame skip

`--fast`, one run alone / four at once (fps of the game, mean of the second half of 24 s; BWPORT_FRAMESKIP=N draws one frame in N):

| scene | N=1 | 2 | 4 | 8 | four at once, N=1 / 2 / 4 / 8 |
|---|---|---|---|---|---|
| town save (field, walking) | 233 | 273 | 310 | 313 | 134 / 158 / 172 / 181 |
| route1 save, battle | 248 | 274 | 282 | 288 | 140 / 158 / 170 / 177 |
| boot (the intro) | 704 | 748 | 779 | 760 | 286 / 311 / 327 / 331 |

Drawing is 7-26 % of a --fast frame (town 26 %, battle 14 %, intro 7 %); CPU per frame 28.6 -> 21.4 ms (town). The rest is not
drawing: ~6.5 cores of the exe's CPU run even with the draw stubbed (N=1000), of which ~3.3 are llvmpipe threads (libntr's own GL:
its 3D pass on g3FrameBuffer and the window's quads/imgui, which Renderer=vcore/gpu does not need) and the game's threads.
Candidate for the next round: skip libntr's own 3D rasterization when a frame renderer is active (nothing the game reads comes
from it, to check), the biggest remaining cost. FrameSkip=auto on llvmpipe, Renderer=gpu Scale=4, town, cap on, game fps off / auto
/ all skipped: 4 CPUs 40 / 48 / 58, 6 CPUs 42 / 52 / 59, 2 CPUs 25 / 34 / -; with 28 free CPUs the game holds 60 either way.

## libntr stops drawing its own picture

libntr-norender.patch: with Renderer=vcore or gpu libntr no longer rasterizes its own 3D (SIM_G3RasterOn in sim_main.cpp: G3SIM_FlushArray drops each
batch before the texture decode/CRC/upload and the GL draw; no shown-frame blit, no clear of its 3D framebuffer; no 2D layer clears), skips the
ImGui render pass while the debug GUI is closed (it cost ~5 ms a frame on llvmpipe) and clears the window once (it cleared twice, with depth), and
limits Mesa's llvmpipe to 4 threads (LP_NUM_THREADS, set before the GL context unless the user set it, Renderer=vcore only: the present is one
quad and waking 28 raster threads cost several times its work). Kept: the geometry engine (counts, box/position/vector tests, swap state), the
display capture (vcore's StoreCapture), the window present, Renderer=libntr, BWPORT_VCAPTURE (records libntr's 3D frame), BWPORT_G3RASTER=1 (old
behaviour), and libntr's picture from the first frame the renderers do not draw. cores (whole exe) / fps, before -> after:

| scene | --fast alone | --fast, 4 at once | 60 fps cap alone | cap, 4 at once |
|---|---|---|---|---|
| town | 5.8 / 235 -> 3.6 / 339 | 3.4 / 141 -> 3.2 / 209 | 1.15 -> 0.78 | 1.33 -> 0.81 |
| route1 battle | 6.3 / 276 -> 3.8 / 429 | 3.4 / 142 -> 3.3 / 240 | 1.15 -> 0.77 | 1.36 -> 0.82 |
| boot (intro) | 8.5 / 737 -> 3.1 / 453 | 3.7 / 284 -> 2.9 / 320 | 0.84 -> 0.59 | |

What libntr still does per frame: the CPU geometry engine, the window's one textured quad (about 0.9 ms of llvmpipe at 512x768), the swap, ImGui's
NewFrame, the V-blank and sound work. The intro in --fast is bound by that present on 4 llvmpipe threads (LP_NUM_THREADS=8 gives 657 fps).

