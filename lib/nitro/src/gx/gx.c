// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/gx/src/gx.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/gx/gx.h>
#include <nitro/mi/dma.h>
#include "gxstate.h"
#include <nitro/spi/ARM9/pm.h>

    #include <nitro/os/ARM9/vramExclusive.h>

#ifdef SDK_TEG_WA_VBLANK
    #include <nitro/os.h>
    static void GXi_TEGWorkAround(void *);
#endif

u32 GXi_DmaId = GX_DEFAULT_DMAID;

    vu16 GXi_VRamLockId = 0;



#ifdef SDK_TEG_WA_VBLANK
#endif



// From libntr (MIT, https://github.com/cybervisi0n/libntr), libraries/gx/src/gx.c: ported as it is, with credit (tools/ref_port.py)
void GX_Init (void)
{
	const u16 bg_mtx_elem_one = 1 << 8;

	reg_GX_POWCNT |= (1 << REG_GX_POWCNT_DSEL_SHIFT);
	GX_SetPower(GX_POWER_ALL);
	GXi_PowerLCD(TRUE);
	GX_InitGXState();

	{
		s32 lockResult;

		while (GXi_VRamLockId == 0) {
			lockResult = OS_GetLockID();
			if (lockResult == OS_LOCK_ID_ERROR) {
				OS_Terminate();
			}
			GXi_VRamLockId = (u16)lockResult;
		}
	}

	reg_GX_DISPSTAT = 0;
	reg_GX_DISPCNT = 0;
	if (GXi_DmaId != GX_DMA_NOT_USE) {
		if (GXi_DmaId > MI_DMA_MAX_NUM) {
			MI_NDmaFill(GXi_DmaId - 4, (void *)REG_BG0CNT_ADDR, 0,
			            REG_DISP_MMEM_FIFO_ADDR - REG_BG0CNT_ADDR);
			reg_GX_MASTER_BRIGHT = 0;
			MI_NDmaFill(GXi_DmaId - 4, (void *)REG_DB_DISPCNT_ADDR, 0,
			            REG_DB_MASTER_BRIGHT_ADDR - REG_DB_DISPCNT_ADDR + 4);
		} else {
			MIi_DmaFill32(GXi_DmaId, (void *)REG_BG0CNT_ADDR, 0,
			              REG_DISP_MMEM_FIFO_ADDR - REG_BG0CNT_ADDR, TRUE);
			reg_GX_MASTER_BRIGHT = 0;
			MIi_DmaFill32(GXi_DmaId, (void *)REG_DB_DISPCNT_ADDR, 0,
			              REG_DB_MASTER_BRIGHT_ADDR - REG_DB_DISPCNT_ADDR + 4, TRUE);
		}
	} else {
		MI_CpuFill32((void *)REG_BG0CNT_ADDR, 0, REG_DISP_MMEM_FIFO_ADDR - REG_BG0CNT_ADDR);
		reg_GX_MASTER_BRIGHT = 0;

		MI_CpuFill32((void *)REG_DB_DISPCNT_ADDR, 0,
		             REG_DB_MASTER_BRIGHT_ADDR - REG_DB_DISPCNT_ADDR + 4);
	}

	reg_G2_BG2PA = bg_mtx_elem_one;
	reg_G2_BG2PD = bg_mtx_elem_one;
	reg_G2_BG3PA = bg_mtx_elem_one;
	reg_G2_BG3PD = bg_mtx_elem_one;
	reg_G2S_DB_BG2PA = bg_mtx_elem_one;
	reg_G2S_DB_BG2PD = bg_mtx_elem_one;
	reg_G2S_DB_BG3PA = bg_mtx_elem_one;
	reg_G2S_DB_BG3PD = bg_mtx_elem_one;
}

void GX_SetVCountEqVal (s32 val)
{
	SDK_MINMAX_ASSERT(val, 0, 262);
	reg_GX_DISPSTAT = (u16)((reg_GX_DISPSTAT & (REG_GX_DISPSTAT_VBLK_MASK |
	                                            REG_GX_DISPSTAT_HBLK_MASK |
	                                            REG_GX_DISPSTAT_LYC_MASK |
	                                            REG_GX_DISPSTAT_VBI_MASK |
	                                            REG_GX_DISPSTAT_HBI_MASK |
	                                            REG_GX_DISPSTAT_VQI_MASK)) |
	                        ((val & 0xff) << 8) | ((val & 0x100) >> 1));
}

s32 GX_HBlankIntr (BOOL enable)
{
	s32 rval = (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBI_MASK);

	if (enable) {
		reg_GX_DISPSTAT |= REG_GX_DISPSTAT_HBI_MASK;
	} else {
		reg_GX_DISPSTAT &= ~REG_GX_DISPSTAT_HBI_MASK;
	}

	return rval;
}

s32 GX_VBlankIntr (BOOL enable)
{
	s32 rval = (reg_GX_DISPSTAT & REG_GX_DISPSTAT_VBI_MASK);

	if (enable) {
		reg_GX_DISPSTAT |= REG_GX_DISPSTAT_VBI_MASK;
	} else {
		reg_GX_DISPSTAT &= ~REG_GX_DISPSTAT_VBI_MASK;
	}

	return rval;
}

static u16 sDispMode = 0;
static u16 sIsDispOn = TRUE;

BOOL GX_IsDispOn (void)
{
	return sIsDispOn;
}

void GX_DispOff (void)
{
	u32 tmp = reg_GX_DISPCNT;

	sIsDispOn = FALSE;
	sDispMode = (u16)((tmp & REG_GX_DISPCNT_MODE_MASK) >> REG_GX_DISPCNT_MODE_SHIFT);

	reg_GX_DISPCNT = tmp & ~REG_GX_DISPCNT_MODE_MASK;
	PMi_SetDispOffCount();
}

void GX_DispOn (void)
{
	sIsDispOn = TRUE;

	if (sDispMode != (u16)GX_DISPMODE_OFF) {
		reg_GX_DISPCNT =
			((reg_GX_DISPCNT & ~REG_GX_DISPCNT_MODE_MASK) |
			 (sDispMode << REG_GX_DISPCNT_MODE_SHIFT));
	} else {
		reg_GX_DISPCNT = ((reg_GX_DISPCNT | (GX_DISPMODE_GRAPHICS << REG_GX_DISPCNT_MODE_SHIFT)));
	}
}

void GX_SetGraphicsMode (GXDispMode dispMode, GXBGMode bgMode, GXBG0As bg0_2d3d)
{
	u32 cnt = reg_GX_DISPCNT;

	GX_DISPMODE_ASSERT(dispMode);
	SDK_WARNING(dispMode != GX_DISPMODE_OFF,
	            "A parameter GX_DISPMODE_OFF is obsolete. Use GX_DispOff() instead.");

	sDispMode = (u16)dispMode;
	if (!sIsDispOn) {
		dispMode = GX_DISPMODE_OFF;
	}

	GX_BGMODE_ASSERT(bgMode);
	GX_BG0_AS_ASSERT(bg0_2d3d);
	cnt &= ~(REG_GX_DISPCNT_BGMODE_MASK |
	         REG_GX_DISPCNT_BG02D3D_MASK | REG_GX_DISPCNT_MODE_MASK | REG_GX_DISPCNT_VRAM_MASK);

	reg_GX_DISPCNT = (u32)(cnt |
	                       (dispMode << REG_GX_DISPCNT_MODE_SHIFT) |
	                       (bgMode << REG_GX_DISPCNT_BGMODE_SHIFT) | (bg0_2d3d <<
	                                                                  REG_GX_DISPCNT_BG02D3D_SHIFT));

	if (sDispMode == GX_DISPMODE_OFF) {
		sIsDispOn = FALSE;
	}
}

void GXS_SetGraphicsMode (GXBGMode bgMode)
{
	reg_GXS_DB_DISPCNT = ((reg_GXS_DB_DISPCNT & ~REG_GXS_DB_DISPCNT_BGMODE_MASK) |
	                      (bgMode << REG_GXS_DB_DISPCNT_BGMODE_SHIFT));
}

void GXx_SetMasterBrightness_ (vu16 *reg, int brightness)
{
	SDK_MINMAX_ASSERT(brightness, -16, 16);

	if (brightness == 0) {
		*reg = 0;
	} else if (brightness > 0)   {
		*reg = (u16)((1 << REG_GX_MASTER_BRIGHT_E_MOD_SHIFT) | brightness);
	} else {
		*reg = (u16)((2 << REG_GX_MASTER_BRIGHT_E_MOD_SHIFT) | (-brightness));
	}
}

int GXx_GetMasterBrightness_ (vu16 *reg)
{
	u16 mode = (u16)(*reg & REG_GX_MASTER_BRIGHT_E_MOD_MASK);

	if (mode == 0) {
		return 0;
	} else if (mode == (1 << REG_GX_MASTER_BRIGHT_E_MOD_SHIFT))   {
		return *reg & REG_GX_MASTER_BRIGHT_E_VALUE_MASK;
	} else if (mode == (2 << REG_GX_MASTER_BRIGHT_E_MOD_SHIFT))   {
		return -(*reg & REG_GX_MASTER_BRIGHT_E_VALUE_MASK);
	} else {
		OS_TWarning("Illegal MasterBright mode!\n");
		return 0;
	}
}
