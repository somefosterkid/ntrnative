#include "heap.h"
#include "math_util.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "pokewalker.h"
#include "save_arrays.h"
#include "trainer_memo.h"
#include "constants/heap.h"
#include "heap.h"
#include "math_util.h"
#include "player_data.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "pokemon_types_def.h"
#include "pokewalker.h"
#include "save.h"
#include "save_arrays.h"
#include "trainer_memo.h"
s16 ov112_021EE8BC(BoxPokemon *, Pokemon *, s32);
s32 ov112_021EE970(BoxPokemon *, s32);
s32 ov112_021EE9A4(Pokemon *, PlayerProfile *, void *, u32, s32);

void ov112_021EE7A8(void *arg0)
{
  s32 sp10;
  s32 spC;
  u16 sp8[2];
  void *new_var;
  PCStorage *sp4;
  BoxPokemon *temp_r6;
  Pokemon *temp_r0;
  s32 var_r7;
  sp4 = SaveArray_PCStorage_Get(*((SaveData **) (((u8 *) arg0) + 0x20)));
  temp_r0 = AllocMonZeroed(HEAP_ID_154);
  temp_r6 = Mon_GetBoxMon(temp_r0);
  new_var = arg0 + 0x9D44;
  sub_02032688(*((POKEWALKER **) (((u8 *) arg0) + 0x1E440)), &sp8[0], &sp8[1]);
  sp10 = (s32) sp8[1];
  spC = 0;
  if ((((u32) ((*((u8 *) (((u8 *) arg0) + 0x10E7))) << 0x1D)) >> 0x1F) == 0)
  {
    var_r7 = Pokewalker_TryGetBoxMon(*((POKEWALKER **) (((u8 *) arg0) + 0x1E440)), temp_r6);
    if (var_r7 != 0)
    {
      *((s16 *) (((u8 *) arg0) + 0x1F374)) = ov112_021EE8BC(temp_r6, temp_r0, *((s32 *) (((u8 *) arg0) + 0x1EC7C)));
    }
    ov112_021EE970(temp_r6, *((s32 *) (((u8 *) arg0) + 0x1EC7C)));
    *((s32 *) (((u8 *) arg0) + 0x1EC7C)) = 0;
    Pokewalker_ClearBoxMon(*((POKEWALKER **) (((u8 *) arg0) + 0x1E440)));
  }
  else
  {
    ov112_021EE9A4(temp_r0, *((PlayerProfile **) (((u8 *) arg0) + 0x1E438)), new_var, MTRandom() % 24U, 0);
    BoxMonSetTrainerMemo(Mon_GetBoxMon(temp_r0), *((PlayerProfile **) (((u8 *) arg0) + 0x1E438)), 0, 0xE9, HEAP_ID_154);
    var_r7 = 1;
  }
  if (var_r7 != 0)
  {
    PCStorage_FindFirstEmptySlot(sp4, &sp10, &spC);
    PCStorage_PlaceMonInBoxByIndexPair(sp4, (u32) sp10, (u32) spC, temp_r6);
    *((BoxPokemon **) (((u8 *) arg0) + 0x1E430)) = PCStorage_GetMonByIndexPair(sp4, (u32) sp10, (u32) spC);
    Pokedex_SetMonSeenFlag(*((Pokedex **) (((u8 *) arg0) + 0x1E444)), temp_r0);
    Pokedex_SetMonCaughtFlag(*((Pokedex **) (((u8 *) arg0) + 0x1E444)), temp_r0);
  }
  Heap_Free(temp_r0);
}


