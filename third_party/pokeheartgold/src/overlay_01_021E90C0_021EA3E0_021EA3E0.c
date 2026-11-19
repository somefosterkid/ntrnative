#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"

struct AreaLightTemplate;

typedef struct AreaLightTemplate {
    u32 endTime;
    u8 validLightsMask;
    GXRgb lightColors[4];
    VecFx16 lightVectors[4];
    GXRgb diffuseReflectColor;
    GXRgb ambientReflectColor;
    GXRgb specularReflectColor;
    GXRgb emissionColor;
} AreaLightTemplate;

char *Ascii_GetDelim(char *src, char *dst, s32 terminator);

int Ascii_StrToL(char *str);

void *Sys_AllocAndReadFile(enum NarcId narcID, const char *narcMemberIdx);

void *Heap_Alloc(u32 heapID, u32 size);

void Heap_FreeExplicit(u32 heapID, void *ptr);

char *ov01_021EA578(char *fileIter, GXRgb *lightColor, VecFx16 *lightVector);

char *ov01_021EA668(char *fileIter, GXRgb *color);

u32 ov01_021EA3E0(void *archiveID, AreaLightTemplate **templates);

u32 ov01_021EA3E0(void *archiveID, AreaLightTemplate **templates) {
    int i, j;
    char lineBuffer[256];
    char endTimeBuffer[256];

    void *fileBuffer = Sys_AllocAndReadFile((enum NarcId)4, archiveID);
    void *fileIter = fileBuffer;
    int templateCount = 0;

    do {
        fileIter = Ascii_GetDelim(fileIter, lineBuffer, '\r');

        if (!(lineBuffer[0] == 'E' && lineBuffer[1] == 'O' && lineBuffer[2] == 'F')) {
            fileIter = Ascii_GetDelim(fileIter, lineBuffer, '\r');
            fileIter = Ascii_GetDelim(fileIter, lineBuffer, '\r');
            fileIter = Ascii_GetDelim(fileIter, lineBuffer, '\r');
            fileIter = Ascii_GetDelim(fileIter, lineBuffer, '\r');
            fileIter = Ascii_GetDelim(fileIter, lineBuffer, '\r');
            fileIter = Ascii_GetDelim(fileIter, lineBuffer, '\r');
            fileIter = Ascii_GetDelim(fileIter, lineBuffer, '\r');
            fileIter = Ascii_GetDelim(fileIter, lineBuffer, '\r');
            fileIter = Ascii_GetDelim(fileIter, lineBuffer, '\r');
            templateCount++;
        }
    } while (!(lineBuffer[0] == 'E' && lineBuffer[1] == 'O' && lineBuffer[2] == 'F'));

    *templates = Heap_Alloc(HEAP_ID_FIELD1, sizeof(AreaLightTemplate) * templateCount);
    MI_CpuClear8(*templates, sizeof(AreaLightTemplate) * templateCount);
    fileIter = fileBuffer;

    for (i = 0; i < templateCount; i++) {
        AreaLightTemplate *template = &(*templates)[i];
        fileIter = Ascii_GetDelim(fileIter, lineBuffer, '\r');
        char *iter = lineBuffer;
        iter = Ascii_GetDelim(iter, endTimeBuffer, ',');
        template->endTime = Ascii_StrToL(endTimeBuffer);

        for (j = 0; j < 4; j++) {
            fileIter = ov01_021EA578(fileIter, &template->lightColors[j], &template->lightVectors[j]);

            if (template->lightColors[j] != 0xFFFF) {
                template->validLightsMask |= 1 << j;
            } else {
                template->lightColors[j] = ((GXRgb)(((0) << (0)) | ((0) << (5)) | ((0) << (10))));
            }
        }

        fileIter = ov01_021EA668(fileIter, &template->diffuseReflectColor);
        fileIter = ov01_021EA668(fileIter, &template->ambientReflectColor);
        fileIter = ov01_021EA668(fileIter, &template->specularReflectColor);
        fileIter = ov01_021EA668(fileIter, &template->emissionColor);
        fileIter = Ascii_GetDelim(fileIter, lineBuffer, '\r');
    }

    Heap_FreeExplicit(HEAP_ID_FIELD1, fileBuffer);

    return templateCount;
}
