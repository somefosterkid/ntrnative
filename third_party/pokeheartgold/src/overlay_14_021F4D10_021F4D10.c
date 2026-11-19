#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "item.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "pokedex_util.h"
#include "pokemon_storage_system.h"
#include "save.h"
#include "sprite_system.h"
#include "string_util.h"
#include "unk_02013534.h"
#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F4D10(UnkStruct_ov14_boxsys *sys);

void sub_020138B0(TextOBJ *textOBJ, BOOL a1);

void ov14_021F4D10(UnkStruct_ov14_boxsys *sys) {
    Window window;
    TextOBJTemplate template;
    UnkStruct_ov14_textobj *textObj;

    sys->unk_410 = FontSystem_NewInit(2, HEAP_ID_10);
    textObj = &sys->textObjs[0];
    InitWindow(&window);
    AddTextWindowTopLeftCorner(sys->unk_14, &window, 12, 2, 0, 2);
    sub_02021AC8(sub_02013688(&window, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_10), TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &textObj->charTransfer);
    template.fontSystem = sys->unk_410;
    template.window = &window;
    template.spriteList = SpriteManager_GetSpriteList(sys->unk_2F8);
    template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(sys->unk_2F8, 0xC101);
    template.sprite = sys->sprites[7]->sprite;
    template.offset = textObj->charTransfer.offset;
    template.x = 0x80;
    template.y = 0x80 - 0x9C;
    template.unk_20 = 1;
    template.unk_24 = 4;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_10;
    sys->textObjs[0].textObj = sub_020135D8(&template);
    sub_020138B0(sys->textObjs[0].textObj, 1);
    sub_020138E0(sys->textObjs[0].textObj, 1);
    RemoveWindow(&window);
    textObj = &sys->textObjs[1];
    InitWindow(&window);
    AddTextWindowTopLeftCorner(sys->unk_14, &window, 5, 2, 0, 2);
    sub_02021AC8(sub_02013688(&window, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_10), TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &textObj->charTransfer);
    template.fontSystem = sys->unk_410;
    template.window = &window;
    template.spriteList = SpriteManager_GetSpriteList(sys->unk_2F8);
    template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(sys->unk_2F8, 0xC101);
    template.sprite = sys->sprites[7]->sprite;
    template.offset = textObj->charTransfer.offset;
    template.x = 0x80;
    template.y = 0x80 - 0x9C;
    template.unk_20 = 1;
    template.unk_24 = 4;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_10;
    textObj->textObj = sub_020135D8(&template);
    sub_020138B0(textObj->textObj, 1);
    sub_020138E0(textObj->textObj, 1);
    RemoveWindow(&window);
}
