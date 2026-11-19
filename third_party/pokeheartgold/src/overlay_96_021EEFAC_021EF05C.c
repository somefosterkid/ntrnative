#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "heap.h"
#include "system.h"
#include "unk_0203A3B0.h"

#include "overlay_96_app_021eefac.h"

void ov96_021EE808(Ov96TextBox *box);

void ov96_021EEF98(Ov96App_021EEFAC *work);

void ov96_021EF05C(PokeathlonCourseData *arg0);

void ov96_021EF05C(PokeathlonCourseData *data) {
    Ov96App_021EEFAC *v0;

    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    sub_0203A914();
    ov96_021EE808(v0->textBox);
    ov96_021EEF98(v0);
    PokeathlonCourse_FreePtr4HeapAlloc(data);
    Heap_Destroy(HEAP_ID_159);
}
