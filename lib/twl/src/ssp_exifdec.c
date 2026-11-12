// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/twl/src/ssp_exifdec.c, for Black's copy of the
// library in overlay 172, which is ARM code.
#include "types.h"
#include "nitro/mi.h"
#include "gfl/std.h"
#include "ssp_private.h"

// The EXIF reader of TwlSDK's JPEG decoder (ssp_jpegdec.c): it walks the TIFF structure of a JPEG's APP1 segment for
// its date and software, the DSi's maker note, which holds the signature and two blocks of the application's, and the
// thumbnail. The game links only what the decoder calls; the getters were dead-stripped. It is libssp.TWL.HYB.a's
// exifdec.o, and its functions have the SDK's names, which Black 1's decompilation (aletheia) gives the same functions;
// the names of its data are ours.

// The tags of the TIFF structure that the reader looks for
#define EXIF_TAG_SOFTWARE 0x131
#define EXIF_TAG_DATE_TIME 0x132
#define EXIF_TAG_JPEG_INTERCHANGE_FORMAT 0x201
#define EXIF_TAG_EXIF_IFD 0x8769
#define EXIF_TAG_MAKER_NOTE 0x927c
#define EXIF_TAG_INTEROPERABILITY_IFD 0xa005

// The tags of the maker note: its first entry, which holds the signature, then the application's two blocks
#define MAKER_NOTE_TAG_SIGNATURE 0x1000
#define MAKER_NOTE_TAG_DATA_1 0x1001
#define MAKER_NOTE_TAG_DATA_2 0x1002

// The offsets in the TIFF structure are from its header, 12 bytes into the APP1 segment
#define TIFF_HEADER_OFFSET 0xc

// An IFD that is not there, or has been read
#define IFD_NONE 0xffff

#define EXIF_SOFTWARE_LENGTH_MAX 30

enum {
    EXIF_RESULT_THUMBNAIL,
    EXIF_RESULT_NEXT_SEGMENT,
    EXIF_RESULT_ERROR,
};

static u16 sSoftwareLength;
static u16 sMakerNoteData1Size;
static u16 sMakerNoteData2Size;
static u32 sMakerNoteData1;
static u32 sMakerNoteData2;
BOOL data_ov172_021cada4;
static char sDateTime[20];
static char sSoftware[32];

// Reads a 16-bit value of the TIFF structure, in its byte order
static u16 GetWord(u8 *data, u32 pos, BOOL bigEndian) {
    u16 ret;
    if (bigEndian == FALSE) {
        int low = *(data + pos);
        ret = low;
        ret |= (*(data + pos + 1) << 8) & 0xff00;
        return ret;
    }
    ret = (*(data + pos) << 8) & 0xff00;
    ret |= *(data + pos + 1);
    return ret;
}

// Reads a 32-bit value of the TIFF structure, in its byte order
static u32 GetLong(u8 *data, u32 pos, BOOL bigEndian) {
    u32 value;
    if (bigEndian == FALSE) {
        value = *(data + pos);
        value |= (*(data + pos + 1) << 8) & 0xff00;
        value |= (*(data + pos + 2) << 16) & 0xff0000;
        value |= (*(data + pos + 3) << 24) & 0xff000000;
        return value;
    }
    value = (*(data + pos) << 24) & 0xff000000;
    value |= (*(data + pos + 1) << 16) & 0xff0000;
    value |= (*(data + pos + 2) << 8) & 0xff00;
    value |= *(data + pos + 3);
    return value;
}

// Reads the APP1 segment at pos: the 0th IFD, the Exif IFD, then the maker note, the Interoperability IFD and the 1st
// IFD in the order they are stored
static int GetAPP1(u8 *data, u32 size, u32 pos, int *posOut, u32 option) {
    u32 valueCount;
    u32 makerNote;
    BOOL bigEndian;
    u32 value;
    s32 len;
    u16 count;
    u32 ifd1;
    u32 exifIfd;
    u32 interopIfd;
    u32 thumbnail;
    s32 tag;
    s32 a;
    s32 b;
    u8 *p;

    makerNote = interopIfd = exifIfd = IFD_NONE;
    if (pos + 0x12 >= size) {
        return EXIF_RESULT_ERROR;
    }
    bigEndian = TRUE;
    len = GetWord(data, pos, TRUE);
    p = data + pos;
    if (p[2] != 'E') {
        return EXIF_RESULT_ERROR;
    }
    if (p[3] != 'x') {
        return EXIF_RESULT_ERROR;
    }
    if (p[4] != 'i') {
        return EXIF_RESULT_ERROR;
    }
    if (p[5] != 'f') {
        return EXIF_RESULT_ERROR;
    }
    if (p[6] != 0) {
        return EXIF_RESULT_ERROR;
    }
    if (p[7] != 0) {
        return EXIF_RESULT_ERROR;
    }
    tag = GetWord(data, pos + 8, bigEndian);
    if (tag == 0x4949) {
        bigEndian = FALSE;
    } else if (tag != 0x4d4d) {
        return EXIF_RESULT_ERROR;
    }
    count = GetWord(data, pos + 0x10, bigEndian);
    pos += 0x12;
    if (count * 12 + pos + 4 >= size) {
        return EXIF_RESULT_ERROR;
    }
    for (; count != 0; count = (u16)(count - 1)) {
        tag = GetWord(data, pos, bigEndian);
        GetWord(data, pos + 2, bigEndian);
        valueCount = GetLong(data, pos + 4, bigEndian);
        value = GetLong(data, pos + 8, bigEndian);
        pos += 12;
        if (tag == EXIF_TAG_EXIF_IFD) {
            exifIfd = value;
        }
        if (tag == EXIF_TAG_DATE_TIME) {
            if (value + 0x20 >= size) {
                return EXIF_RESULT_ERROR;
            }
            sys_memcpy_fast(&data[value + TIFF_HEADER_OFFSET], sDateTime, sizeof(sDateTime));
        }
        if (tag == EXIF_TAG_SOFTWARE) {
            if (valueCount > EXIF_SOFTWARE_LENGTH_MAX) {
                valueCount = EXIF_SOFTWARE_LENGTH_MAX;
            }
            value += TIFF_HEADER_OFFSET;
            if (value + valueCount >= size) {
                return EXIF_RESULT_ERROR;
            }
            sys_memcpy_fast(data + value, sSoftware, valueCount);
            sSoftwareLength = valueCount;
        }
    }
    ifd1 = GetLong(data, pos, bigEndian);
    if (ifd1 == 0) {
        ifd1 = IFD_NONE;
    }
    thumbnail = 0xffffffff;
    pos = exifIfd + TIFF_HEADER_OFFSET;
    if (pos + 2 >= size) {
        return EXIF_RESULT_ERROR;
    }
    count = GetWord(data, pos, bigEndian);
    pos += 2;
    if (count * 12 + pos >= size) {
        return EXIF_RESULT_ERROR;
    }
    for (; count != 0; count = (u16)(count - 1)) {
        tag = GetWord(data, pos, bigEndian);
        GetWord(data, pos + 2, bigEndian);
        valueCount = GetLong(data, pos + 4, bigEndian);
        value = GetLong(data, pos + 8, bigEndian);
        pos += 12;
        if (tag == EXIF_TAG_MAKER_NOTE && valueCount > 4) {
            makerNote = value;
        }
        if (tag == EXIF_TAG_INTEROPERABILITY_IFD) {
            interopIfd = value;
        }
    }
    while (makerNote != IFD_NONE || interopIfd != IFD_NONE || ifd1 != IFD_NONE) {
        if (makerNote < interopIfd && makerNote < ifd1) {
            if (makerNote + 0x1a >= size) {
                return EXIF_RESULT_ERROR;
            }
            valueCount = GetWord(data, makerNote + 0xc, bigEndian);
            a = GetWord(data, makerNote + 0xe, bigEndian);
            b = GetWord(data, makerNote + 0x10, bigEndian);
            value = GetLong(data, makerNote + 0x12, bigEndian);
            GetLong(data, makerNote + 0x16, bigEndian);
            pos = makerNote + 0x1a;
            if (a == MAKER_NOTE_TAG_SIGNATURE && b == 7 && value == 0x1c) {
                count = (u16)(valueCount - 1);
                if (count * 12 + pos >= size) {
                    return EXIF_RESULT_ERROR;
                }
                for (; count != 0; count = (u16)(count - 1)) {
                    tag = GetWord(data, pos, bigEndian);
                    GetWord(data, pos + 2, bigEndian);
                    valueCount = GetLong(data, pos + 4, bigEndian);
                    value = GetLong(data, pos + 8, bigEndian);
                    pos += 12;
                    if (tag == MAKER_NOTE_TAG_DATA_1) {
                        if (valueCount > 4) {
                            sMakerNoteData1 = (u16)value;
                        } else {
                            sMakerNoteData1 = (u16)(pos - valueCount);
                        }
                        sMakerNoteData1Size = valueCount;
                        if (sMakerNoteData1 + sMakerNoteData1Size >= size) {
                            sMakerNoteData1 = 0;
                            sMakerNoteData1Size = 0;
                            return EXIF_RESULT_ERROR;
                        }
                    } else if (tag == MAKER_NOTE_TAG_DATA_2) {
                        if (valueCount > 4) {
                            sMakerNoteData2 = (u16)value;
                        } else {
                            sMakerNoteData2 = (u16)(pos - valueCount);
                        }
                        sMakerNoteData2Size = valueCount;
                        if (sMakerNoteData2 + sMakerNoteData2Size >= size) {
                            sMakerNoteData2 = 0;
                            sMakerNoteData2Size = 0;
                            return EXIF_RESULT_ERROR;
                        }
                    }
                }
            }
            makerNote = IFD_NONE;
        } else if (interopIfd < makerNote && interopIfd < ifd1) {
            if (interopIfd + 0xe >= size) {
                return EXIF_RESULT_ERROR;
            }
            count = GetWord(data, interopIfd + TIFF_HEADER_OFFSET, bigEndian);
            pos = interopIfd + 0xe;
            if (count * 12 + pos >= size) {
                return EXIF_RESULT_ERROR;
            }
            for (; count != 0; count = (u16)(count - 1)) {
                GetWord(data, pos, bigEndian);
                GetWord(data, pos + 2, bigEndian);
                GetLong(data, pos + 4, bigEndian);
                GetLong(data, pos + 8, bigEndian);
                pos += 12;
            }
            interopIfd = IFD_NONE;
        } else if (ifd1 < makerNote && ifd1 < interopIfd) {
            if (ifd1 + 0xe >= size) {
                return EXIF_RESULT_ERROR;
            }
            count = GetWord(data, ifd1 + TIFF_HEADER_OFFSET, bigEndian);
            pos = ifd1 + 0xe;
            if (count * 12 + pos >= size) {
                return EXIF_RESULT_ERROR;
            }
            for (; count != 0; count = (u16)(count - 1)) {
                tag = GetWord(data, pos, bigEndian);
                GetWord(data, pos + 2, bigEndian);
                GetLong(data, pos + 4, bigEndian);
                value = GetLong(data, pos + 8, bigEndian);
                pos += 12;
                if (tag == EXIF_TAG_JPEG_INTERCHANGE_FORMAT) {
                    thumbnail = value;
                    break;
                }
            }
            ifd1 = IFD_NONE;
        } else {
            return EXIF_RESULT_ERROR;
        }
    }
    if (option & 1) {
        if (thumbnail == 0xffffffff) {
            return EXIF_RESULT_ERROR;
        }
        *posOut = thumbnail + TIFF_HEADER_OFFSET;
        return EXIF_RESULT_THUMBNAIL;
    }
    *posOut = len + 4;
    return EXIF_RESULT_NEXT_SEGMENT;
}

s32 SSP_ExifDecode(u8 *src, u32 src_size, int *cur, u32 option) {
    int result;
    u32 start = *cur;

    sMakerNoteData1 = 0;
    sMakerNoteData1Size = 0;
    sMakerNoteData2 = 0;
    sMakerNoteData2Size = 0;
    sSoftwareLength = 0;
    result = GetAPP1(src, src_size, start, cur, option);
    if (sMakerNoteData1 != 0) {
        sMakerNoteData1 += (u32)src + TIFF_HEADER_OFFSET;
    }
    if (sMakerNoteData2 != 0) {
        sMakerNoteData2 += (u32)src + TIFF_HEADER_OFFSET;
    }
    return result;
}
