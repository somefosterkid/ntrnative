#include "global.h"

#include "overlay_109_photo_album_app.h"

extern s32 ov109_021E7890;

void ov109_021E5DEC(PhotoAlbumApp *app, u32 idx) {
    u32 i;
    u8 numEntries;
    u8 temp_r3;

    app->numEntries = (u8)(app->numEntries - 1);
    numEntries = app->numEntries;
    if (idx < (u32)numEntries) {
        i = idx;
        if (idx < (u32)numEntries) {
            do {
                MI_CpuCopy8(&app->entries[i + 1], &app->entries[i], sizeof(PhotoAlbumEntry));
                i = (u32)((u8)(i + 1));
                numEntries = app->numEntries;
            } while (i < (u32)numEntries);
        }
    }
    MI_CpuFill8(&app->entries[numEntries], 0U, sizeof(PhotoAlbumEntry));
    app->numPages = (u8)((s32)app->numEntries / 12);
    if (((s32)app->numEntries % 12) != 0) {
        app->numPages = (u8)(app->numPages + 1);
    }
    if (app->numEntries == 0) {
        app->numPages = 1U;
    }
    numEntries = app->numEntries;
    if (idx >= (u32)numEntries) {
        if (numEntries == 0) {
            app->cursorX = 3;
            app->cursorY = 3;
            app->page = 0;
            return;
        }
        if (idx != 0) {
            idx = (u32)((u8)(idx - 1));
        }
        app->page = (s8)((s32)idx / 12);
        temp_r3 = *((u8 *)(((u8 *)&ov109_021E7890) + (((s32)idx) % 12)));
        app->cursorX = (s8)(temp_r3 % 4);
        app->cursorY = (s8)((u32)temp_r3 >> 2);
    }
}
