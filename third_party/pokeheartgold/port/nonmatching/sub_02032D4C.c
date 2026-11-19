/* PORTOK hk42 */
BOOL sub_02032C84(void);
s32 sub_02032E48(void);
s32 sub_02032844(s32);
s32 sub_02032858();
extern s32 _021D4124;

void sub_02032D4C(void *wm_startscancallback) {
    s32 (*temp_r2)(s32, u8);
    s32 (*temp_r2_2)(void *);
    u16 temp_r1;

    if (*(u16 *)((u8 *)wm_startscancallback + 2) != 0) {
        sub_02032858(*(u16 *)((u8 *)wm_startscancallback + 2));
        sub_02032844(9);
        return;
    }
    if (*(s32 *)((u8 *)(*(void **)((u8 *)(&_021D4124) + 4)) + 0x1310) != 2) {
        *(u16 *)((u8 *)(*(void **)((u8 *)(&_021D4124) + 4)) + 0x130E) = 0U;
        if (sub_02032E48() == 0) {
            sub_02032844(9);
        }
    } else {
        temp_r1 = *(u16 *)((u8 *)wm_startscancallback + 8);
        switch (temp_r1) {
        case 5:
            DC_InvalidateRange(*(u8 **)((u8 *)(&_021D4124) + 4) + 0x1220, 0xC0U);
            temp_r2 = *(s32 (**)(s32, u8))((u8 *)(*(void **)((u8 *)(&_021D4124) + 4)) + 0x1320);
            if ((temp_r2 != NULL) && ((u32) *(u16 *)((u8 *)wm_startscancallback + 0x36) >= 8U)) {
                temp_r2(*(s32 *)((u8 *)wm_startscancallback + 0x3C), *((u8 *)wm_startscancallback + 0x48 + 4));
            }
            if (((u32) *(u16 *)((u8 *)wm_startscancallback + 0x36) >= 8U) && (*(s32 *)((u8 *)wm_startscancallback + 0x3C) == *(s32 *)((u8 *)(*(void **)((u8 *)(&_021D4124) + 4)) + 8)) && ((3 & *(u8 *)((u8 *)wm_startscancallback + 0x43)) == 1)) {
                temp_r2_2 = *(s32 (**)(void *))((u8 *)(*(void **)((u8 *)(&_021D4124) + 4)) + 0x1300);
                if (temp_r2_2 != NULL) {
                    temp_r2_2(*(u8 **)((u8 *)(&_021D4124) + 4) + 0x1220);
                }
                if (*(u16 *)((u8 *)(*(void **)((u8 *)(&_021D4124) + 4)) + 0x130E) != 0) {
                    if (sub_02032E48() == 0) {
                        sub_02032844(9);
                        return;
                    }
                } else {
                    goto block_19;
                }
            } else {
            default:
            case 4:
block_19:
                if (sub_02032C84() == 0) {
                    sub_02032844(9);
                }
            case 3:
                return;
            }
            break;
        }
    }
}
