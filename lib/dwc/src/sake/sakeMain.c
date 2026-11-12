// Black's sakeMain.c (GameSpy's file, as pokebw2 names it), split from the glued ov175_021dd1ec.c.

#include "dwc/webservices_internal.h"

// URLs the game sets for the file server, in place of the default ones
// pokebw2's gSakeUploadUrlOverride
static char data_ov175_021e4c08[128];
// pokebw2's gSakeDownloadUrlOverride
static char data_ov175_021e4b88[128];

u32 func_ov175_021dd1ec(Ov175Data **param_1) {
    void *dest;
    if (data_ov009_021acef8 != 1) {
        return 1;
    }
    if (gsCoreIsShutdown() != 0) {
        return 2;
    }
    dest = gsimalloc(0x4c);
    if (dest == (void *)0x0) {
        return 3;
    }
    memset(dest, 0, 0x4c);
    *param_1 = dest;
    return 0;
}

void func_ov175_021dd250(Ov175Data *param_1) {
    gsifree(param_1);
}

void func_ov175_021dd25c(Ov175Data *param_1, s8 *param_2, u32 param_3, s8 *param_4) {
    strcpy(param_1->unk_4, param_2);
    strcpy(param_1->unk_18, param_4);
    param_1->unk_14 = param_3;
    param_1->unk_0 = 1;
}

void func_ov175_021dd290(Ov175Data *param_1, u32 param_2, const u8 *param_3) {
    param_1->unk_28 = param_2;
    param_1->unk_2c = *(Ov175Key *)param_3;
    param_1->unk_24 = 1;
}

u32 func_ov175_021dd2cc(Ov175Data *param_1) {
    return param_1->unk_48;
}

Ov175Op *func_ov175_021dd2d4(Ov175Data *param_1, void *param_2, Ov175ReqCb param_3, void *param_4, s32 param_5, u32 (*param_6)(Ov175Op *)) {
    Ov175Op *ptr;
    u32 result;
    ptr = func_ov175_021dd56c(param_1, param_5, param_2, param_3, param_4);
    if (ptr == NULL) {
        return NULL;
    }
    result = param_6(ptr);
    param_1->unk_48 = result;
    if (result != 0) {
        func_ov175_021dd604(ptr);
        return NULL;
    }
    return ptr;
}

s32 func_ov175_021dd328(Ov175Data *param_1, Ov175Op *param_2) {
    void *theTask;
    Ov175Desc *desc;

    if (param_1 != NULL && param_2 != NULL) {
        desc = param_2->unk_20;
        if (desc != NULL) {
            theTask = desc->unk_24;
            if (theTask != NULL) {
                gsiCoreCancelTask(theTask);
                return 0;
            }
        }
    }
    return 3;
}

Ov175Op *func_ov175_021dd358(Ov175Data *param_1, void *param_2, Ov175ReqCb param_3, void *param_4) {
    return func_ov175_021dd2d4(param_1, param_2, param_3, param_4, 0, func_ov175_021de024);
}

Ov175Op *func_ov175_021dd380(Ov175Data *param_1, void *param_2, Ov175ReqCb param_3, void *param_4) {
    return func_ov175_021dd2d4(param_1, param_2, param_3, param_4, 1, func_ov175_021de0d4);
}

Ov175Op *func_ov175_021dd3a8(Ov175Data *param_1, void *param_2, Ov175ReqCb param_3, void *param_4) {
    return func_ov175_021dd2d4(param_1, param_2, param_3, param_4, 4, func_ov175_021debe0);
}

Ov175Op *func_ov175_021dd3d0(Ov175Data *param_1, void *param_2, Ov175ReqCb param_3, void *param_4) {
    return func_ov175_021dd2d4(param_1, param_2, param_3, param_4, 5, func_ov175_021dece8);
}

u32 func_ov175_021dd3f8(Ov175Data *param_1, s8 *param_2) {
    if (param_1 != NULL && param_2 != (s8 *)0x0) {
        strcpy(data_ov175_021e4b88, param_2);
        return 1;
    }
    return 0;
}

u32 func_ov175_021dd420(Ov175Data *param_1, u32 param_2, s8 *param_3) {
    s32 result;
    if (param_1 == NULL || param_3 == NULL || param_1->unk_0 == 0 || param_1->unk_24 == 0) {
        return 0;
    }
    if (data_ov175_021e4b88[0] != '\0') {
        result = OS_SNPrintf(param_3, 0x80, "%s?fileid=%d&gameid=%d&pid=%d", data_ov175_021e4b88, param_2, param_1->unk_14, param_1->unk_28);
    } else {
        result = OS_SNPrintf(param_3, 0x80, "http://%s.sake.gamespy.com/SakeFileServer/download.aspx?fileid=%d&gameid=%d&pid=%d", param_1->unk_4, param_2, param_1->unk_14, param_1->unk_28);
    }
    if (result < 0) {
        return 0;
    }
    return 1;
}

u32 func_ov175_021dd4b0(Ov175Data *param_1, s8 *param_2) {
    if (param_1 != NULL && param_2 != (s8 *)0x0) {
        strcpy(data_ov175_021e4c08, param_2);
        return 1;
    }
    return 0;
}

u32 func_ov175_021dd4d8(Ov175Data *param_1, s8 *param_2) {
    s32 result;
    if (param_1 == NULL || param_2 == NULL || param_1->unk_0 == 0 || param_1->unk_24 == 0) {
        return 0;
    }
    if (data_ov175_021e4c08[0] != '\0') {
        result = OS_SNPrintf(param_2, 0x80, "%s?gameid=%d&pid=%d", data_ov175_021e4c08, param_1->unk_14, param_1->unk_28);
    } else {
        result = OS_SNPrintf(param_2, 0x80, "http://%s.sake.gamespy.com/SakeFileServer/upload.aspx?gameid=%d&pid=%d", param_1->unk_4, param_1->unk_14, param_1->unk_28);
    }
    if (result < 0) {
        return 0;
    }
    return 1;
}

void func_ov175_021dd560(Ov175Data *param_1, Ov175Op *param_2) {
    func_ov175_021dd328(param_1, param_2);
}
