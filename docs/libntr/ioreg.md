# I/O registers on the host: one page per block

On the DS the I/O registers are one piece of memory (0x04000000 up), and the SDK and the games use that: a fill over
`REG_BG0CNT_ADDR .. REG_DISP_MMEM_FIFO_ADDR` in GX_Init, `G2x_SetBGyAffine_` writing four words from `&reg_G2_BG2PA`,
DMA and timer registers indexed by channel from the first one, the edge / fog / toon tables copied from their first
word. libntr used to give every register name its own global (`s_reg_*`, 349 of them in sim_vars.c), so each of these
ran over the end of one variable into whatever the linker had put behind it. It worked while the neighbours happened
to be laid out in declaration order; ASan reported it (global-buffer-overflow in MIi_DmaFill32 from GX_Init, FillRegs,
G2x_SetBGyAffine_, MI_StopDma), and another compiler, LTO or one more global could break it with no sign.

## Where the registers are

`port/subprojects/libntr/include/simulator/ioreg_map.h` is the only place a register gets its storage:

| page | host array | what |
|---|---|---|
| ARM9 | `s_ioreg9[0x2000]` | 0x04000000-0x04001FFF: engine A, DMA, timers, IRQ, VRAMCNT, divider, 3D; engine B at 0x1000 |
| ARM9, far | `s_ioreg9x[0x20]` | 0x04100000: IPC FIFO receive, card data |
| ARM7 | `s_ioreg7[0x600]` | SPI (0x1c0), POWCNT2 (0x304), sound (0x400-0x51d) |

Every `s_reg_*` name is a macro onto its page at the hardware offset (`NTR_IOREG9(REGType16v, 0x008)`), so
`REG_x_ADDR`, `reg_X` and every direct `s_reg_X` user compile unchanged. The three arrays are defined in
libraries/sim/src/sim_vars.c (inside the machine's section, so the in-process reset puts them back like before). The
ARM7 has its own page because its registers share offsets with the ARM9's (sound at 0x400 against the geometry FIFO).

`python3 tools/ioreg_map_check.py` checks the table: every name at the `REG_x_OFFSET` of each `REG_x_ADDR` that
points to it (both header sets, nitro/hw/X86 and twl/hw/X86), aligned, inside its page, overlapping another name
only as a whole part of it, and no `s_reg_*` left as a separate global. A new register is one line in the map.

## What changed in behaviour

- The wide and the half names of one register are the same bytes now, as on the DS: BGxOFS and BGxHOFS / BGxVOFS,
  VRAMCNT and VRAMCNT_A..D, WVRAMCNT and VRAMCNT_E..WRAM, the divider's 64-bit words and their _L / _H, the edge /
  fog / toon table words and their _L / _H, SOUND0CNT and its parts. Before, a 16-bit scroll write never reached
  the 32-bit variable the renderer reads (the capture's `main_16bit_scroll_written` note marked it; the note is gone).
- GX_Init's two fills clear what they clear on the DS: 0x008-0x067 (the 2D registers, DISP3DCNT, DISPCAPCNT) and
  engine B's 0x1000-0x106F. Before, the second one started at the lone DB_DISPCNT variable and ran 0x70 bytes over
  whatever followed it: by sim_vars.c's declaration order DB_MASTER_BRIGHT, the DMA and the card registers, not
  engine B's BG registers (read from the source, not measured: the order was the linker's to choose).

## The rule for arithmetic on a register address

`REG_x_ADDR` is an integer on the DS and a typed pointer here (`&s_reg_x`), so `REG_x_ADDR + n` moves n elements,
not n bytes. Cast to `u8 *` before adding a DS byte offset. Fixed with the page: OS_SetTimerCount / OS_SetTimerControl
(nitro/os/common/timer.h: `id * 4` on a 16-bit pointer is 8 bytes a timer, so id 1 addressed timer 2), OS_GetTickLo (os_tick.c), two DMA sites in mi_dma.c
(`REG_DMA0SAD_ADDR + dmaNo * 12`, `REG_DMA0CNT_ADDR + n * 12`), SND_SetupCapture (snd_capture.c). Already right:
the `(u64)` / `(vu32 *)` casts in mi_dma.c, dma.h and dma_red.h, the `u8 *` WININ / WINOUT / MOSAIC addresses,
sound's `REGType8v *` ones.

## What is still not a page

- The TWL-only registers of twl/hw/X86 (ioreg_CAM, ioreg_DSP, ioreg_SCFG, ioreg_EXI, the NDMA, MBK and MC_A / MC_B
  blocks of ioreg_MI, twl's ioreg_PAD): their `REG_x_ADDR` is still `HW_REG_BASE + offset`, a raw DS address that
  faults on the host if anything reaches it (the games run as an original DS, nothing does). nitro's EXI SIO
  registers are the same.
- The branches of the SDK sources that are not compiled for the host (`#else` of SDK_PORT: `(u32)REG_x_ADDR + ...`)
  were not audited. One is known: gx/g3imm.h's G3_Direct0..3 `#else` branches (`REG_MTX_MODE_ADDR + (op - 0x10) * 4`
  on a u32 pointer: 16 bytes an op). SDK_PORT is defined for every libntr and game unit, so it is dead code here (crv6).
- Making `REG_x_ADDR` an integer (`uintptr_t`) as on the DS would remove the arithmetic rule above; every user that
  passes it as a pointer without a cast would need one.
