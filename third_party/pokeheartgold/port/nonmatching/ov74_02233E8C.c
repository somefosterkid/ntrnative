#include "global.h"
extern int Unk_ov97_0223F438;
extern u32 Unk_ov97_0223F448;
extern int Unk_ov97_0223F43C;
extern int Unk_ov97_0223EC00;
void ov74_02233DBC(int gbaSectorID);

#ifdef __MWERKS__
static int sState;
static int sSlot;
static int sSectorID;
static void *sSaveSlot;
static int sUnkC;
static u32 sCounter;
#else
#include "pm_agb_save.h"
/* the real record, not private copies: the DS layout of ov74_0223D33C */
#define sSlot (*(int *)&ov74_0223D33C.slot)
#define sSectorID (*(int *)&ov74_0223D33C.sectorsWritten)
#define sSaveSlot ((void *)ov74_0223D33C.buffer)
#define sUnkC (*(int *)&ov74_0223D33C.firstSector)
#define sCounter (ov74_0223D33C.counter)
#define sState (*(int *)&ov74_0223D33C.writeStatus)
#endif
extern int sPmAgbCartridgeSpec[2];

BOOL ov74_02233E8C();
BOOL ov74_02233E8C()
{
  int new_var;
  if (sSectorID == 0)
  {
    if (sSlot == (new_var = 1))
    {
      sPmAgbCartridgeSpec[1] = 0;
    }
    else
    {
      sPmAgbCartridgeSpec[1] = 1;
    }
    sCounter++;
  }
  else
  {
    return 0;
  }
  if (sPmAgbCartridgeSpec[1] < 0)
  {
    return 0;
  }
  ov74_02233DBC(sSectorID);
  return 1;
}

