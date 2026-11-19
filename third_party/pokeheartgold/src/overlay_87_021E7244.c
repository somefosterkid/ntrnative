#include "global.h"

#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "unk_0200A090.h"
#include "overlay_87_scratch_off_card_app.h"
#include "overlay_manager.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "message_printer.h"
#include "yes_no_prompt.h"
#include "string_util.h"
#include "msgdata.h"

struct PaletteAnimator;
struct MessageFormat;
struct SysTask;
enum CharsetMode {

    CHARSET_MODE_JP,

    CHARSET_MODE_EN,
};
typedef struct PaletteAnimator PaletteAnimator;
typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);
struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};
struct MessageFormat;
void ov87_021E7244(ScratchOffCardApp *app, u32 idx, s32 num);

void ov87_021E7244(ScratchOffCardApp *app, u32 idx, s32 num) {
    BufferIntegerAsString(app->strTemplate, idx, num, 1, PRINTING_MODE_LEFT_ALIGN, CHARSET_MODE_EN);
}
