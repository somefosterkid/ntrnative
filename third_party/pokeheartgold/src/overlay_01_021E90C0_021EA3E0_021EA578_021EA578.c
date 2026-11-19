#include "global.h"
#include "ascii_util.h"

char *ov01_021EA578(char *fileIter, GXRgb *lightColor, VecFx16 *lightVector);

char *ov01_021EA578(char *fileIter, GXRgb *lightColor, VecFx16 *lightVector)
{
    char lineBuffer[256 ];
    char partBuffer[256 ];

    fileIter = Ascii_GetDelim(fileIter, lineBuffer, '\r');

    char *iter = lineBuffer;
    iter = Ascii_GetDelim(iter, partBuffer, ',');
    u32 lightValid = Ascii_StrToL(partBuffer);

    if (lightValid == 1 ) {
        int i;
        u16 lightColorParts[3 ];
        s32 lightVectorParts[3 ];

        for (i = 0; i < 3 ; i++) {
            iter = Ascii_GetDelim(iter, partBuffer, ',');
            lightColorParts[i] = Ascii_StrToL(partBuffer);
        }

        *lightColor = ((GXRgb)(((lightColorParts[0]) << (0)) | (( lightColorParts[1]) << (5)) | (( lightColorParts[2]) << (10)))) ;

        for (i = 0; i < 3 ; i++) {
            iter = Ascii_GetDelim(iter, partBuffer, ',');
            lightVectorParts[i] = Ascii_StrToL(partBuffer);
        }

        lightVector->x = lightVectorParts[0];
        lightVector->y = lightVectorParts[1];
        lightVector->z = lightVectorParts[2];

        if (lightVector->x > ((fx16) 0x0000000000001000) ) {
            lightVector->x = ((fx16) 0x0000000000001000) ;
        }

        if (lightVector->x < -((fx16) 0x0000000000001000) ) {
            lightVector->x = -((fx16) 0x0000000000001000) ;
        }

        if (lightVector->y > ((fx16) 0x0000000000001000) ) {
            lightVector->y = ((fx16) 0x0000000000001000) ;
        }

        if (lightVector->y < -((fx16) 0x0000000000001000) ) {
            lightVector->y = -((fx16) 0x0000000000001000) ;
        }

        if (lightVector->z > ((fx16) 0x0000000000001000) ) {
            lightVector->z = ((fx16) 0x0000000000001000) ;
        }

        if (lightVector->z < -((fx16) 0x0000000000001000) ) {
            lightVector->z = -((fx16) 0x0000000000001000) ;
        }
    } else {
        *lightColor = 0xFFFF ;
    }

    return fileIter;
}
