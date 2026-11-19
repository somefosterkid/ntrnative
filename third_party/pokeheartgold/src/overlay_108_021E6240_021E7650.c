#include "global.h"

#include "obj_char_transfer.h"
#include "unk_02013534.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E7650(SafariAreaCustomizer *arg0);

void ov108_021E7650(SafariAreaCustomizer *arg0) {
    TextOBJ_Destroy(arg0->unk_3A4);
    sub_02021B5C(&arg0->unk_3A8);
    arg0->unk_3A4 = NULL;
    sub_02013938(arg0->unk_3A0);
}
