#include "global.h"
#include "constants/heap.h"
#include "pokemon_types_def.h"
#include "screen_fade.h"

struct Pokemon;
struct PokepicTemplate;
void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);
void *CutIn_GetPokemonSpriteSource(Pokemon *mon, PokepicTemplate *spriteTemplate, enum HeapID heapID);
void *ov02_0224A800(PokepicTemplate *spriteTemplate, enum HeapID heapID);

void ov02_022493FC(void);
void ov02_02249420(void);

void ov02_022493FC(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, (((31) << 10) | ((31) << 5) | (31)), 6, 1, HEAP_ID_FIELD1);
}

void ov02_02249420(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, (((31) << 10) | ((31) << 5) | (31)), 6, 1, HEAP_ID_FIELD1);
}
