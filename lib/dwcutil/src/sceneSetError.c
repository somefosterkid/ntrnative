// libdwcutil's sceneSetError.c (Black's ov016 0x021c7ce4-0x021c82fc): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneSetError(void);
void drawMessage(void);
void procInit_sceneSetError(void);
void procInit1_sceneSetError(void);
void procInit2_sceneSetError(void);
void procScene_sceneSetError(void);
void input_sceneSetError(void);
void draw_sceneSetError(void);
void button_sceneSetError(void);
void endScene_sceneSetError(void);
void endScene1_sceneSetError(void);
void endScene2_sceneSetError(void);
extern s32 data_ov016_021de5e8;
extern s8 data_ov016_021db568;
extern u8 data_ov016_021da5f0[];
typedef struct tagCOORD {
    u16 x;
    u16 y;
} COORD;
extern const COORD data_ov016_021da600[];
extern char data_ov016_021db57c[];

void DWCi_SceneSetError(void) {
    initGraphics_sceneSetError();
    drawMessage();
    DWCi_ChangeScene((u32)procInit_sceneSetError);
}

void initGraphics_sceneSetError(void) {
    DWCi_CMNlLoadResource(((u8 *)&data_ov016_021db568), (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void drawMessage(void) {
    u16 *msg;
    NNSG2dCharCanvas *canv;
    s32 id = 0;
    s32 prm;
    u16 buf[8];
    char numStr[8];
    u16 w, h;

    DWCi_GetParam2_(NULL, (u32 *)&prm);
    if (data_ov016_021de5e8 < -0x4E83) {
        if (data_ov016_021de5e8 >= -0x4E84) {
            id = 2;
        } else if (data_ov016_021de5e8 >= -0x4E85) {
            id = 0x13;
        } else if (data_ov016_021de5e8 >= -0x4E8B) {
            id = 2;
        } else if (data_ov016_021de5e8 >= -0x4E8C) {
            id = 0x12;
        } else if (data_ov016_021de5e8 >= -0x4E8D) {
            id = 2;
        } else if (data_ov016_021de5e8 >= -0x4E8E) {
            id = 3;
        } else if (data_ov016_021de5e8 >= -0x5207) {
            id = 2;
        } else if (data_ov016_021de5e8 < -0x59D7) {
            if (data_ov016_021de5e8 >= -0x5DBF) {
                id = 0x13;
            } else if (data_ov016_021de5e8 < -0xC34F) {
                if (data_ov016_021de5e8 >= -0xC352) {
                    id = 7;
                } else if (data_ov016_021de5e8 >= -0xC353) {
                    id = 5;
                } else if (data_ov016_021de5e8 < -0xC3B2) {
                    if (data_ov016_021de5e8 >= -0xC3B3) {
                        if (prm == 2) {
                            id = 5;
                        } else {
                            id = 7;
                        }
                    } else if (data_ov016_021de5e8 < -0xC79A) {
                        if (data_ov016_021de5e8 >= -0xC79B) {
                            if (prm == 2) {
                                id = 5;
                            } else {
                                id = 8;
                            }
                        } else if (data_ov016_021de5e8 >= -0xC79E) {
                            id = 9;
                        } else if (data_ov016_021de5e8 >= -0xC79F) {
                            id = 5;
                        } else if (data_ov016_021de5e8 < -0xC7FF) {
                            if (data_ov016_021de5e8 >= -0xC863) {
                                id = 0x14;
                            } else if (data_ov016_021de5e8 >= -0xC866) {
                                id = 0x15;
                            } else if (data_ov016_021de5e8 >= -0xC867) {
                                id = 4;
                            } else if (data_ov016_021de5e8 < -0xCB1F) {
                                if (data_ov016_021de5e8 >= -0xCB22) {
                                    id = 0xA;
                                } else if (data_ov016_021de5e8 >= -0xCB23) {
                                    id = 0x16;
                                } else if (data_ov016_021de5e8 < -0xCB83) {
                                    if (data_ov016_021de5e8 >= -0xCB87) {
                                        id = 0xB;
                                    } else if (data_ov016_021de5e8 < -0xCBE7) {
                                        if (data_ov016_021de5e8 >= -0xCBEB) {
                                            id = 0xB;
                                        } else if (data_ov016_021de5e8 < -0xCC4B) {
                                            if (data_ov016_021de5e8 >= -0xCCAF) {
                                                id = 2;
                                            } else if (data_ov016_021de5e8 < -0xCF07) {
                                                if (data_ov016_021de5e8 >= -0xD033) {
                                                    id = 2;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    canv = DWCi_FNTlFormBgEx(0, data_ov016_021da5f0[DWCi_Language()]);
    msg = (u16 *)DWCi_MSGlGet(data_ov016_021de598, id);
    OS_SNPrintf(numStr, 8, data_ov016_021db57c, -data_ov016_021de5e8);
    func_ov016_021d722c(buf, numStr, 8);
    h = data_ov016_021da600[DWCi_Language()].y;
    w = data_ov016_021da600[DWCi_Language()].x;
    DWCi_FNTlDrawStringEx(canv, w, h, 2, 0xA, buf, 0);
    DWCi_FNTlDrawRect(canv, 0xD, 0x3C, 0xE6, 0x5E, 2, DWCi_CMNlFontFlag(), msg);
    DWCi_FNTlRenewBg((s32)canv);
}

void procInit_sceneSetError(void) {
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneSetError);
}

void procInit1_sceneSetError(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_CMNlButton(4);
    DWCi_ChangeScene((u32)procInit2_sceneSetError);
}

void procInit2_sceneSetError(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneSetError);
}

void procScene_sceneSetError(void) {
    input_sceneSetError();
    draw_sceneSetError();
    button_sceneSetError();
}

void input_sceneSetError(void) {
    if (!DWCi_IPTlCheckTrigger(1)) {
        return;
    }
    func_ov016_021bcb28(0);
}

void draw_sceneSetError(void) {
}

void button_sceneSetError(void) {
    if (DWCi_BTNlGet() != 0) {
        return;
    }
    DWCi_SNDlPlay(6);
    DWCi_ChangeScene((u32)endScene_sceneSetError);
}

void endScene_sceneSetError(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneSetError);
}

void endScene1_sceneSetError(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    DWCi_BTNlEnd();
    func_ov016_021d55a4(3, 1, 1, 8);
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene2_sceneSetError);
}

void endScene2_sceneSetError(void) {
    u8 result;
    u32 uStack_10;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    if (!DWCi_BTNlIsEnd()) {
        return;
    }
    DWCi_FNTlDeleteBgLcd(0);
    DWCi_COMMENTlDelete();
    DWCi_GXlInvisible(1, 1);
    DWCi_GXlInvisible(0, 0x15);
    DWCi_GetParam2_((u32 *)0x0, &uStack_10);
    if (uStack_10 != 0) {
        DWCi_SetParam_(2, 1);
        DWCi_ChangeScene((u32)DWCi_SceneSetSelectAp);
        return;
    }
    DWCi_SetParam_(2, 0);
    DWCi_SetParam2_(0, 0);
    DWCi_SetListReturn(0);
    DWCi_ChangeScene((u32)DWCi_SceneSetList);
}

void DWCi_SetErrorCode(u32 code) {
    data_ov016_021de5e8 = code;
}
