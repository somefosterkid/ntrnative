// Black's code at ov009 0x021819b0-0x02183f68. GameSpy's XML writer and reader (gsXml's role; name guessed from the
// sorted file order stringutil, udp engine (gt2.c), xml): the request builder with <, xmlns tokens and the stream
// reader the soap code calls.

#include "types.h"
#include "dwc/ext.h"
#include "gs/darray.h"
#include "gs/nonport.h"
#include "stdlib.h"
#include "dwc/gt2.h"
#include "printf.h"
#include "ctype.h"
#include "nitro/std.h"
#include "gs/largeint.h"
#include "gs/xml.h"

void func_ov009_02181e10(void);
void func_ov009_02181e14(void);
void func_ov009_02181dc4(u32 *param_1);
typedef struct GSIXmlString {
    const u8 *mData;
    s32 mLen;
} GSIXmlString;

typedef struct GSIXmlElement {
    GSIXmlString mName;
    GSIXmlString mValue;
    s32 mIndex;
    s32 mParentIndex;
} GSIXmlElement;

typedef struct GSIXmlAttribute {
    GSIXmlString mName;
    GSIXmlString mValue;
    s32 mIndex;
    s32 mParentIndex;
} GSIXmlAttribute;

BOOL func_ov009_02181e28(u32 *reader, const char *buffer, s32 len, s32 *pos);
BOOL func_ov009_02181e80(u32 *reader, char *buffer, s32 len, s32 *pos, s32 parentIndex);
BOOL func_ov009_02182414(u32 *reader, const char *buffer, s32 len, s32 *pos, GSIXmlString *strOut);
BOOL func_ov009_02182554(u32 *reader, char *buffer, s32 len, s32 *pos, GSIXmlString *strOut);
BOOL func_ov009_0218290c(u32 *reader, char *buffer, s32 len, s32 *pos, GSIXmlString *strOut);
BOOL func_ov009_02183d98(s8 *param_1, s32 *param_2);
s32 func_ov009_0217fbc8(const char *str, s32 kind);
s64 func_ov009_0217f124(s8 *str);

u32 func_ov009_0218325c(u32 *param_1, s8 *param_2);
u32 func_ov009_02183210(u32 *param_1, s8 param_2);
u32 func_ov009_0218335c(u32 *param_1, s8 *param_2);
u32 func_ov009_021832dc(u32 *writer, u16 *text);
u32 func_ov009_02183490(u32 *param_1);

#pragma thumb off

static char *data_ov009_021a3d0c[4] = {
    "SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\"",
    "SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\"",
    "xsi=\"http://www.w3.org/2001/XMLSchema-instance\"",
    "xsd=\"http://www.w3.org/2001/XMLSchema\"",
};
BOOL func_ov009_0218263c(char *buffer, s32 *len);
u32 *func_ov009_021819b0(char **namespaces, s32 count) {
    u32 *newStream;
    s32 initialCapacity = 0x400;
    s32 namespaceLen = 0;
    s32 i;

    newStream = gsimalloc(0x10);
    if (newStream == NULL) {
        gsDebugFormat(0xc, 2, 1, "Out of memory in gsXmlCreateStreamWriter, needed %d bytes", 0x10);
        return NULL;
    }
    for (i = 0; i < 4; i++) {
        namespaceLen += strlen("xmlns:") + 1;
        namespaceLen += strlen(data_ov009_021a3d0c[i]);
    }
    for (i = 0; i < count; i++) {
        namespaceLen += strlen("xmlns:") + 1;
        namespaceLen += strlen(namespaces[i]);
    }
    while (initialCapacity < namespaceLen) {
        initialCapacity += 0x400;
    }
    newStream[0] = (u32)gsimalloc(initialCapacity);
    if (newStream[0] == 0) {
        gsDebugFormat(0xc, 2, 1, "Out of memory in gsXmlCreateStreamWriter, needed %d bytes", initialCapacity);
        return NULL;
    }
    newStream[2] = initialCapacity;
    newStream[1] = 0;
    *(char *)newStream[0] = '\0';
    newStream[3] = FALSE;
    if (!func_ov009_0218325c(newStream, (s8 *)"<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope")) {
        gsifree((void *)newStream[0]);
        gsifree(newStream);
        return NULL;
    }
    for (i = 0; i < 4; i++) {
        if (!func_ov009_02183210(newStream, ' ') || !func_ov009_0218325c(newStream, (s8 *)"xmlns:") || !func_ov009_0218325c(newStream, (s8 *)data_ov009_021a3d0c[i])) {
            gsifree((void *)newStream[0]);
            gsifree(newStream);
            return NULL;
        }
    }
    for (i = 0; i < count; i++) {
        if (!func_ov009_02183210(newStream, ' ') || !func_ov009_0218325c(newStream, (s8 *)"xmlns:") || !func_ov009_0218325c(newStream, (s8 *)namespaces[i])) {
            gsifree((void *)newStream[0]);
            gsifree(newStream);
            return NULL;
        }
    }
    if (!func_ov009_02183210(newStream, '>') || !func_ov009_0218325c(newStream, (s8 *)"<SOAP-ENV:Body>")) {
        gsifree((void *)newStream[0]);
        gsifree(newStream);
        return NULL;
    }
    return newStream;
}

u32 *func_ov009_02181c2c(void) {
    u32 *ptr;
    void *array;

    ptr = gsimalloc(0x14);
    if (ptr == (u32 *)0x0) {
        gsDebugFormat(0xc, 2, 1, "Out of memory in gsXmlCreateStream, needed %d bytes", 0x14);
        return (u32 *)0x0;
    }
    array = ArrayNew(0x18, 0x20, (void (*)(void *element))func_ov009_02181e10);
    *ptr = (u32)array;
    if (array == (void *)0x0) {
        gsDebugFormat(0xc, 2, 1, "Out of memory in gsXmlCreateStream with mElementArray=ArrayNew()");
        gsifree(ptr);
        return (u32 *)0x0;
    }
    array = ArrayNew(0x18, 0x10, (void (*)(void *element))func_ov009_02181e14);
    ptr[1] = (u32)array;
    if (array == (void *)0x0) {
        gsDebugFormat(0xc, 2, 1, "Out of memory in gsXmlCreateStream with mElementArray=ArrayNew()");
        ArrayFree((void *)*ptr);
        gsifree(ptr);
        return (u32 *)0x0;
    }
    func_ov009_021834d0(ptr);
    return ptr;
}
#pragma thumb on

#pragma thumb off
u32 func_ov009_02181d10(u32 *param_1, char *param_2, s32 param_3) {
    s32 iStack_1c;

    iStack_1c = 0;
    func_ov009_02181dc4(param_1);
    if (iStack_1c < param_3) {
        do {
            if (!func_ov009_02181e80(param_1, param_2, param_3, &iStack_1c, 0xffffffff)) {
                return 0;
            }
        } while (iStack_1c < param_3);
    }
    return 1;
}
#pragma thumb on

#pragma thumb off
void func_ov009_02181d84(s32 *param_1) {
    gsifree((void *)*param_1);
    gsifree(param_1);
}
#pragma thumb on

#pragma thumb off
void func_ov009_02181da0(u32 *param_1) {
    ArrayFree((void *)param_1[1]);
    ArrayFree((void *)*param_1);
    gsifree(param_1);
}
#pragma thumb on

#pragma thumb off
void func_ov009_02181dc4(u32 *param_1) {
    ArrayClear((void *)param_1[1]);
    ArrayClear((void *)*param_1);
    func_ov009_021834d0(param_1);
}
#pragma thumb on

#pragma thumb off
s32 func_ov009_02181de8(void *param_1) {
    if (func_ov009_0218325c((u32 *) param_1, "</SOAP-ENV:Body></SOAP-ENV:Envelope>") == 0) {
        return 0;
    }
    *(s32 *)((u8 *)(u8 *)param_1 + 0xC) = 1;
    return 1;
}
#pragma thumb on

#pragma thumb off
void func_ov009_02181e10(void) {
}
#pragma thumb on

#pragma thumb off
void func_ov009_02181e14(void) {
}
#pragma thumb on

#pragma thumb off
u32 func_ov009_02181e18(u32 *param_1) {
    return *param_1;
}
#pragma thumb on

#pragma thumb off
u32 func_ov009_02181e20(s32 param_1) {
    return *(u32 *)(param_1 + 4);
}

BOOL func_ov009_02181e28(u32 *reader, const char *buffer, s32 len, s32 *pos) {
    while (*pos < len) {
        if (strchr(" \t\r\n", buffer[*pos]) == NULL) {
            return TRUE;
        }
        (*pos)++;
    }
    return FALSE;
}


BOOL func_ov009_02181e80(u32 *reader, char *buffer, s32 len, s32 *pos, s32 parentIndex) {
    s32 startPos;
    BOOL storeElement = TRUE;

    func_ov009_02181e28(reader, buffer, len, pos);
    if (*pos >= len) {
        return TRUE;
    }
    {
        GSIXmlElement newElem = {0};

        if (buffer[*pos] != '<') {
            return FALSE;
        }
        (*pos)++;
        if (*pos >= len) {
            return FALSE;
        }
        if (!func_ov009_02181e28(reader, buffer, len, pos)) {
            return FALSE;
        }
        if (buffer[*pos] == '!' || buffer[*pos] == '?' || buffer[*pos] == '%' || buffer[*pos] == '/') {
            storeElement = FALSE;
            (*pos)++;
            startPos = *pos - 2;
        } else {
            startPos = *pos - 1;
        }
        if (!func_ov009_02182414(reader, buffer, len, pos, &newElem.mName)) {
            return FALSE;
        }
        if (!func_ov009_02181e28(reader, buffer, len, pos)) {
            return FALSE;
        }
        if (storeElement) {
            newElem.mIndex = ArrayLength((void *)reader[0]);
            newElem.mParentIndex = parentIndex;
            ArrayAppend((void *)reader[0], &newElem);
        }
        while (*pos < len && isalnum(buffer[*pos])) {
            GSIXmlAttribute newAttr = {0};

            if (!func_ov009_02182414(reader, buffer, len, pos, &newAttr.mName)) {
                return FALSE;
            }
            if (!func_ov009_02181e28(reader, buffer, len, pos)) {
                return FALSE;
            }
            if (buffer[*pos] != '=') {
                return FALSE;
            }
            (*pos)++;
            if (!func_ov009_02182554(reader, buffer, len, pos, &newAttr.mValue)) {
                return FALSE;
            }
            if (!func_ov009_02181e28(reader, buffer, len, pos)) {
                return FALSE;
            }
            if (storeElement) {
                newAttr.mIndex = ArrayLength((void *)reader[1]);
                newAttr.mParentIndex = newElem.mIndex;
                ArrayAppend((void *)reader[1], &newAttr);
            }
        }
        if (!func_ov009_02181e28(reader, buffer, len, pos)) {
            return FALSE;
        }
        if ((!isalnum(buffer[startPos + 1]) && buffer[*pos] == buffer[startPos + 1]) || buffer[*pos] == '/') {
            (*pos)++;
            if (!func_ov009_02181e28(reader, buffer, len, pos)) {
                return FALSE;
            }
            if (buffer[*pos] != '>') {
                return FALSE;
            }
            (*pos)++;
            return TRUE;
        }
        if (buffer[*pos] != '>') {
            return FALSE;
        }
        (*pos)++;
        if (!func_ov009_02181e28(reader, buffer, len, pos)) {
            return FALSE;
        }
        if (buffer[*pos] != '<') {
            if (!func_ov009_0218290c(reader, buffer, len, pos, &newElem.mValue)) {
                return FALSE;
            }
            if (storeElement) {
                ArrayReplaceAt((void *)reader[0], &newElem, newElem.mIndex);
            }
            if (!func_ov009_02181e28(reader, buffer, len, pos)) {
                return FALSE;
            }
        }
        while (*pos < len) {
            s32 childStartPos = *pos;

            if (buffer[*pos] != '<') {
                return FALSE;
            }
            (*pos)++;
            if (!func_ov009_02181e28(reader, buffer, len, pos)) {
                return FALSE;
            }
            if (buffer[*pos] == '/') {
                (*pos)++;
                if (!func_ov009_02181e28(reader, buffer, len, pos)) {
                    return FALSE;
                }
                if (*pos + newElem.mName.mLen >= len) {
                    return FALSE;
                }
                if (strncmp((const char *)newElem.mName.mData, &buffer[*pos], newElem.mName.mLen) != 0) {
                    return FALSE;
                }
                *pos += newElem.mName.mLen;
                if (!func_ov009_02181e28(reader, buffer, len, pos)) {
                    return FALSE;
                }
                if (buffer[*pos] != '>') {
                    return FALSE;
                }
                (*pos)++;
                return TRUE;
            }
            if (newElem.mValue.mData != NULL) {
                return FALSE;
            }
            *pos = childStartPos;
            if (!func_ov009_02181e80(reader, buffer, len, pos, newElem.mIndex)) {
                return FALSE;
            }
            func_ov009_02181e28(reader, buffer, len, pos);
        }
        return FALSE;
    }
}


BOOL func_ov009_02182414(u32 *reader, const char *buffer, s32 len, s32 *pos, GSIXmlString *strOut) {
    BOOL hasNamespace = FALSE;

    if (!func_ov009_02181e28(reader, buffer, len, pos)) {
        return FALSE;
    }
    if (*pos >= len) {
        return FALSE;
    }
    if (!isalnum(buffer[*pos]) || isdigit(buffer[*pos])) {
        return FALSE;
    }
    strOut->mData = (const u8 *)&buffer[*pos];
    strOut->mLen = 1;
    (*pos)++;
    while (*pos < len && strchr(" \t\r\n", buffer[*pos]) == NULL) {
        if (buffer[*pos] == ':') {
            if (hasNamespace) {
                return FALSE;
            }
            hasNamespace = TRUE;
        } else if (buffer[*pos] == '_' || buffer[*pos] == '-') {
        } else if (!isalnum(buffer[*pos])) {
            return TRUE;
        }
        strOut->mLen++;
        (*pos)++;
    }
    return TRUE;
}

BOOL func_ov009_02182554(u32 *reader, char *buffer, s32 len, s32 *pos, GSIXmlString *strOut) {
    char delimiter;
    char *start;

    if (!func_ov009_02181e28(reader, buffer, len, pos)) {
        return FALSE;
    }
    delimiter = buffer[*pos];
    if (delimiter != '"' && delimiter != '\'') {
        return FALSE;
    }
    (*pos)++;
    start = &buffer[*pos];
    strOut->mLen = 0;
    if (*pos >= len) {
        return FALSE;
    }
    if (buffer[*pos] == delimiter) {
        strOut->mData = (u8 *)start;
        (*pos)++;
        return TRUE;
    }
    while (buffer[*pos] != delimiter) {
        if (*pos >= len) {
            return FALSE;
        }
        (*pos)++;
        strOut->mLen++;
    }
    (*pos)++;
    if (!func_ov009_0218263c(start, &strOut->mLen)) {
        return FALSE;
    }
    strOut->mData = (u8 *)start;
    return TRUE;
}


BOOL func_ov009_0218263c(char *buffer, s32 *len) {
    s32 readPos = 0;

    while (readPos < *len) {
        if (buffer[readPos] != '&') {
            readPos++;
        } else {
            s32 shiftAmount;

            if (strncmp(&buffer[readPos], "&amp;", 5) == 0) {
                buffer[readPos++] = '&';
                shiftAmount = 4;
            } else if (strncmp(&buffer[readPos], "&quot;", 6) == 0) {
                buffer[readPos++] = '"';
                shiftAmount = 5;
            } else if (strncmp(&buffer[readPos], "&apos;", 6) == 0) {
                buffer[readPos++] = '\'';
                shiftAmount = 5;
            } else if (strncmp(&buffer[readPos], "&lt;", 4) == 0) {
                buffer[readPos++] = '<';
                shiftAmount = 3;
            } else if (strncmp(&buffer[readPos], "&gt;", 4) == 0) {
                buffer[readPos++] = '>';
                shiftAmount = 3;
            } else if (strncmp(&buffer[readPos], "&#x", 3) == 0) {
                s32 start = readPos + 3;
                BOOL started = FALSE;
                u32 digitValue = 0;
                u32 mask = 0xFF000000;
                char *endPos;
                s32 i;

                endPos = strchr(&buffer[start], ';');
                if (endPos == NULL) {
                    return FALSE;
                }
                if (endPos - &buffer[start] > 8) {
                    return FALSE;
                }
                STD_TSScanf(&buffer[start], "%08x", &digitValue);
                for (i = 0; i < 4; i++) {
                    char c = (char)((digitValue & mask) >> ((3 - i) * 8));
                    if (started || c != 0) {
                        buffer[readPos++] = c;
                        started = TRUE;
                    }
                    mask >>= 8;
                }
                shiftAmount = endPos - &buffer[readPos] + 1;
            } else if (strncmp(&buffer[readPos], "&#", 2) == 0) {
                s32 start = readPos + 2;
                BOOL started = FALSE;
                u32 digitValue;
                u32 mask = 0xFF000000;
                char *endPos;
                s32 i;

                endPos = strchr(&buffer[start], ';');
                if (endPos == NULL) {
                    return FALSE;
                }
                digitValue = atol(&buffer[start]);
                for (i = 0; i < 4; i++) {
                    char c = (char)((digitValue & mask) >> ((3 - i) * 8));
                    if (started || c != 0) {
                        buffer[readPos++] = c;
                        started = TRUE;
                    }
                    mask >>= 8;
                }
                shiftAmount = endPos - &buffer[readPos] + 1;
            } else {
                return FALSE;
            }
            memmove(&buffer[readPos], &buffer[readPos + shiftAmount], *len - (readPos + shiftAmount));
            memset(&buffer[*len - shiftAmount], ' ', shiftAmount);
            *len -= shiftAmount;
        }
    }
    return TRUE;
}

BOOL func_ov009_0218290c(u32 *reader, char *buffer, s32 len, s32 *pos, GSIXmlString *strOut) {
    char *start = NULL;

    if (!func_ov009_02181e28(reader, buffer, len, pos)) {
        return FALSE;
    }
    if (buffer[*pos] != '<') {
        start = &buffer[*pos];
        strOut->mData = (u8 *)&buffer[*pos];
    }
    while (*pos < len) {
        if (buffer[*pos] == '<') {
            return func_ov009_0218263c(start, &strOut->mLen) != 0;
        }
        (*pos)++;
        strOut->mLen++;
    }
    return FALSE;
}



#pragma thumb on

#pragma thumb off
u32 func_ov009_021829ac(u32 *param_1, s8 *param_2, s8 *param_3) {
    if ((func_ov009_02183210(param_1, 0x3CU) == 0) || (func_ov009_0218325c(param_1, param_2) == 0) || (func_ov009_02183210(param_1, 0x3AU) == 0) || (func_ov009_0218325c(param_1, param_3) == 0) || (func_ov009_02183210(param_1, 0x3EU) == 0)) {
        return 0U;
    }
    return 1U;
}
#pragma thumb on

#pragma thumb off
u32 func_ov009_02182a2c(u32 *param_1, s8 *param_2, s8 *param_3) {
    if ((func_ov009_02183210(param_1, 0x3CU) == 0) || (func_ov009_02183210(param_1, 0x2FU) == 0) || (func_ov009_0218325c(param_1, param_2) == 0) || (func_ov009_02183210(param_1, 0x3AU) == 0) || (func_ov009_0218325c(param_1, param_3) == 0) || (func_ov009_02183210(param_1, 0x3EU) == 0)) {
        return 0U;
    }
    return 1U;
}
#pragma thumb on

#pragma thumb off
u32 func_ov009_02182ac0(u32 *param_1, s8 *param_2, s8 *param_3, s8 *param_4) {
    s32 len;
    s32 i;
    u8 ch;

    len = (s32) strlen(param_4);
    i = 0;
    if (len > 0) {
loop_1:
        ch = (u8) param_4[i];
        if (((u32) ch < 0x20U) && (ch != 9) && (ch != 0xA) && (ch != 0xD)) {
            return 0U;
        }
        i += 1;
        if (i >= len) {
            goto block_7;
        }
        goto loop_1;
    }
block_7:
    if ((func_ov009_021829ac(param_1, param_2, param_3) == 0) || (func_ov009_0218335c(param_1, param_4) == 0) || (func_ov009_02182a2c(param_1, param_2, param_3) == 0)) {
        return 0U;
    }
    return 1U;
}
#pragma thumb on

#pragma thumb off
u32 func_ov009_02182b68(u32 *param_1, s8 *param_2, s8 *param_3, s8 *param_4) {
    s32 len;
    s32 i;
    u8 ch;

    len = (s32) strlen(param_4);
    i = 0;
    if (len > 0) {
loop_1:
        ch = (u8) param_4[i];
        if (((u32) ch < 0x20U) && (ch != 9) && (ch != 0xA) && (ch != 0xD)) {
            return 0U;
        }
        i += 1;
        if (i >= len) {
            goto block_7;
        }
        goto loop_1;
    }
block_7:
    if ((func_ov009_021829ac(param_1, param_2, param_3) == 0) || (func_ov009_0218335c(param_1, param_4) == 0) || (func_ov009_02182a2c(param_1, param_2, param_3) == 0)) {
        return 0U;
    }
    return 1U;
}
#pragma thumb on

#pragma thumb off
u32 func_ov009_02182c10(u32 *param_1, s8 *param_2, s8 *param_3, u16 *param_4) {
    s32 result;
    s32 i;
    u16 val;

    result = func_ov009_0218198c(param_4);
    i = 0;
    if (result > 0) {
loop_2:
        val = param_4[i];
        if ((u32) val < 0x20U) {
            if ((val != 9) && (val != 0xA) && (val != 0xD)) {
                return 0U;
            }
            goto block_14;
        }
        if (((u32) val > 0xD7FFU) && ((u32) val < 0xE000U)) {
            return 0U;
        }
        if ((u32) val > 0xFFFDU) {
            return 0U;
        }
block_14:
        i += 1;
        if (i >= result) {
            goto block_15;
        }
        goto loop_2;
    }
block_15:
    if ((func_ov009_021829ac(param_1, param_2, param_3) == 0) || (func_ov009_021832dc(param_1, param_4) == 0) || (func_ov009_02182a2c(param_1, param_2, param_3) == 0)) {
        return 0U;
    }
    return 1U;
}

u32 func_ov009_02182cf0(u32 *param_1, s8 *param_2, s8 *param_3, s32 param_4) {
    char buf[32];

    sprintf(buf, "%d", param_4);
    if (func_ov009_021829ac(param_1, param_2, param_3) == 0 || func_ov009_0218325c(param_1, (s8 *)buf) == 0 || func_ov009_02182a2c(param_1, param_2, param_3) == 0) {
        return 0;
    }
    return 1;
}


int func_ov009_0217f130(char *a, s64 value);

u32 func_ov009_02182d78(u32 *param_1, s8 *param_2, s8 *param_3, s64 param_4) {
    char buf[36];

    func_ov009_0217f130(buf, param_4);
    if (func_ov009_021829ac(param_1, param_2, param_3) == 0 || func_ov009_0218325c(param_1, (s8 *)buf) == 0 || func_ov009_02182a2c(param_1, param_2, param_3) == 0) {
        return 0;
    }
    return 1;
}


u32 func_ov009_02182dfc(u32 *param_1, s8 *param_2, s8 *param_3, f32 param_4) {
    char buf[32];

    sprintf(buf, "%f", param_4);
    if ((func_ov009_021829ac(param_1, param_2, param_3) == 0) || (func_ov009_0218325c(param_1, (s8 *)buf) == 0) || (func_ov009_02182a2c(param_1, param_2, param_3) == 0)) {
        return 0U;
    }
    return 1U;
}

u32 func_ov009_02182e90(u32 *writer, s8 *prefix, s8 *name, const u8 *data, s32 size) {
    char hex[3];
    s32 i;

    i = 0;
    hex[2] = 0;
    if (!func_ov009_021829ac(writer, prefix, name)) {
        return FALSE;
    }
    while (i < size) {
        sprintf(hex, "%02x", data[i]);
        i++;
        if (!func_ov009_02183210(writer, hex[0])) {
            return FALSE;
        }
        if (!func_ov009_02183210(writer, hex[1])) {
            return FALSE;
        }
    }
    if (!func_ov009_02182a2c(writer, prefix, name)) {
        return FALSE;
    }
    return TRUE;
}


BOOL func_ov009_02182f40(u32 *param_1, s8 *param_2, s8 *param_3, const u8 *data, s32 size) {
    B64StreamData coder;
    char chunk[5];

    if (!func_ov009_021829ac(param_1, param_2, param_3)) {
        return FALSE;
    }
    B64InitEncodeStream(&coder, data, size, 0);
    while (B64EncodeStream(&coder, chunk)) {
        chunk[4] = 0;
        if (!func_ov009_0218325c(param_1, (s8 *)chunk)) {
            return FALSE;
        }
    }
    if (!func_ov009_02182a2c(param_1, param_2, param_3)) {
        return FALSE;
    }
    return TRUE;
}


u32 func_ov009_02182ff8(u32 *param_1, s8 *param_2, s8 *param_3, s32 param_4) {
    char text[24];
    struct tm *time;

    time = func_ov009_0217f2c8(&param_4);
    sprintf(text, "%d-%02d-%02dT%02d:%02d:%02dZ", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min,
            time->tm_sec);
    if (!func_ov009_021829ac(param_1, param_2, param_3) || !func_ov009_0218325c(param_1, (s8 *)text) ||
        !func_ov009_02182a2c(param_1, param_2, param_3)) {
        return 0;
    }
    return 1;
}


// GameSpy's gsXmlWriteLargeIntElement: the element of a large integer, its words in hex with no leading zeros
BOOL func_ov009_021830d0(u32 *writer, s8 *prefix, s8 *name, LargeIntBuf *lint) {
    u32 *data;
    s32 readPos;
    u32 i;
    BOOL leadingZero;
    char hex[4];

    readPos = lint->length - 1;
    data = &lint->data[0];
    leadingZero = TRUE;
    if (!func_ov009_021829ac(writer, prefix, name)) {
        return FALSE;
    }
    while (readPos >= 0 && data[readPos] == 0) {
        readPos--;
    }
    for (; readPos >= 0; readPos--) {
        if (leadingZero) {
            for (i = 0; i < 4; i++) {
                if ((u8)(lint->data[readPos] >> ((3 - i) * 8)) != 0) {
                    break;
                }
            }
            leadingZero = FALSE;
        } else {
            i = 0;
        }
        for (; i < 4; i++) {
            sprintf(hex, "%02x", (u8)(lint->data[readPos] >> ((3 - i) * 8)));
            if (!func_ov009_02183210(writer, hex[0])) {
                return FALSE;
            }
            if (!func_ov009_02183210(writer, hex[1])) {
                return FALSE;
            }
        }
    }
    if (!func_ov009_02182a2c(writer, prefix, name)) {
        return FALSE;
    }
    return TRUE;
}


#pragma thumb on

#pragma thumb off
u32 func_ov009_02183210(u32 *param_1, s8 param_2) {
    u32 result;

    if (((s32)param_1[1] >= (s32)param_1[2]) && (result = func_ov009_02183490(param_1), result == 0)) {
        return 0;
    }
    *(u8 *)(*param_1 + param_1[1]) = param_2;
    param_1[1] = param_1[1] + 1;
    return 1;
}

u32 func_ov009_0218325c(u32 *param_1, s8 *param_2) {
    s32 length;

    length = strlen((const char *)param_2);
    if (length == 0) {
        return 1;
    }
    while ((s32)param_1[2] - (s32)param_1[1] <= length) {
        if (!func_ov009_02183490(param_1)) {
            return 0;
        }
    }
    strcpy((char *)(*param_1 + param_1[1]), (const char *)param_2);
    param_1[1] += length;
    return 1;
}


extern int _UCS2CharToUTF8String(u16 c, s8 *utf8);
u32 func_ov009_021832dc(u32 *writer, u16 *text) {
    s8 utf8[4] = { 0, 0, 0, 0 };
    s32 length;
    s32 i;

    length = func_ov009_0218198c(text);
    utf8[3] = 0;
    for (i = 0; i < length; i++) {
        utf8[_UCS2CharToUTF8String(text[i], utf8)] = 0;
        if (!func_ov009_0218335c(writer, utf8)) {
            return FALSE;
        }
    }
    return TRUE;
}


u32 func_ov009_0218335c(u32 *param_1, s8 *param_2) {
    s32 len;
    s32 i;
    u32 result;

    len = strlen((const char *)param_2);
    for (i = 0; i < len; i++) {
        if (param_2[i] == '&') {
            result = func_ov009_0218325c(param_1, (s8 *)"&amp;");
        } else if (param_2[i] == '\'') {
            result = func_ov009_0218325c(param_1, (s8 *)"&apos;");
        } else if (param_2[i] == '"') {
            result = func_ov009_0218325c(param_1, (s8 *)"&quot;");
        } else if (param_2[i] == '<') {
            result = func_ov009_0218325c(param_1, (s8 *)"&lt;");
        } else if (param_2[i] == '>') {
            result = func_ov009_0218325c(param_1, (s8 *)"&gt;");
        } else if (param_2[i] == ' ') {
            result = func_ov009_0218325c(param_1, (s8 *)"&#x20;");
        } else if (param_2[i] < ' ' || (u8)param_2[i] > 0x7f) {
            char numeric[8];
            sprintf(numeric, "&#x%02x;", (u8)param_2[i]);
            numeric[6] = '\0';
            result = func_ov009_0218325c(param_1, (s8 *)numeric);
        } else {
            result = func_ov009_02183210(param_1, param_2[i]);
        }
        if (!result) {
            return 0;
        }
    }
    return 1;
}

#pragma thumb on

#pragma thumb off
u32 func_ov009_02183490(u32 *param_1) {
    void *ptr;
    s32 val;
    val = param_1[2];
    ptr = gsirealloc((void *)*param_1, val + 0x400U);
    if (ptr == (void *)0x0) {
        return 0;
    }
    param_1[2] = val + 0x400U;
    if (ptr != (void *)*param_1) {
        *param_1 = (u32)ptr;
    }
    return 1;
}
#pragma thumb on

#pragma thumb off
u32 func_ov009_021834d0(u32 *param_1) {
    param_1[2] = 0xffffffff;
    param_1[3] = 0xffffffff;
    param_1[4] = 0xffffffff;
    return 1;
}
#pragma thumb on

#pragma thumb off
u32 func_ov009_021834e8(u32 *param_1, u8 *param_2) {
    s32 arrayLength;
    u32 index;

    index = param_1[2] + 1;
    arrayLength = ArrayLength((void *)*param_1);
    if ((s32)index < arrayLength) {
        do {
            if (func_ov009_02183d98((s8 *)param_2, ArrayNth((void *)*param_1, index))) {
                param_1[2] = index;
                param_1[3] = 0xffffffff;
                param_1[4] = 0xffffffff;
                return 1;
            }
            index = index + 1;
            arrayLength = ArrayLength((void *)*param_1);
        } while ((s32)index < arrayLength);
    }
    return 0;
}
#pragma thumb on

#pragma thumb off
s32 func_ov009_02183560(u32 *param_1) {
    s32 index;
    s32 result;

    index = *(s32 *)((u8 *)param_1 + 8);
    if ((index >= ArrayLength((void *) *(u32 *)param_1)) || (index == -1)) {
        return 0;
    }
    result = *(s32 *)((u8 *)(ArrayNth((void *) *(u32 *)param_1, index)) + 0x14);
    if (result == -1) {
        return 0;
    }
    if (result >= ArrayLength((void *) *(u32 *)param_1)) {
        return 0;
    }
    *(s32 *)((u8 *)param_1 + 8) = result;
    *(s32 *)((u8 *)param_1 + 0xC) = -1;
    *(s32 *)((u8 *)param_1 + 0x10) = -1;
    return 1;
}
#pragma thumb on

#pragma thumb off
u32 func_ov009_021835d0(u32 *param_1, u8 *param_2) {
    BOOL result;
    s32 arrayLength;
    s32 *elem;
    u32 index;

    index = param_1[2] + 1;
    arrayLength = ArrayLength((void *)*param_1);
    if ((s32)index < arrayLength) {
        do {
            elem = ArrayNth((void *)*param_1, index);
            if ((elem[5] == param_1[2]) && (result = func_ov009_02183d98((s8 *)param_2, elem), result)) {
                param_1[2] = index;
                param_1[3] = 0xffffffff;
                param_1[4] = 0xffffffff;
                return 1;
            }
            if (elem[5] < (s32)param_1[2]) {
                return 0;
            }
            index = index + 1;
            arrayLength = ArrayLength((void *)*param_1);
        } while ((s32)index < arrayLength);
    }
    return 0;
}
#pragma thumb on

#pragma thumb off
u32 func_ov009_02183670(u32 *param_1, u8 *param_2, s32 *param_3, s32 *param_4) {
    BOOL result;
    s32 arrayLength;
    s32 *elem;
    u32 index;

    if (param_1[3] == 0xffffffff) {
        param_1[3] = param_1[2];
    }
    index = param_1[3] + 1;
    arrayLength = ArrayLength((void *)*param_1);
    if ((s32)index < arrayLength) {
        do {
            elem = ArrayNth((void *)*param_1, index);
            if ((elem[5] == param_1[2]) && (result = func_ov009_02183d98((s8 *)param_2, elem), result)) {
                param_1[3] = index;
                *param_3 = elem[2];
                *param_4 = elem[3];
                return 1;
            }
            if (elem[5] < (s32)param_1[2]) {
                return 0;
            }
            index = index + 1;
            arrayLength = ArrayLength((void *)*param_1);
        } while ((s32)index < arrayLength);
    }
    return 0;
}
#pragma thumb on

#pragma thumb off
u32 func_ov009_0218372c(u32 *param_1, u8 *param_2, s8 *param_3, size_t param_4) {
    s8 *psStack_14;
    size_t sStack_18;
    size_t num;

    psStack_14 = NULL;
    sStack_18 = 0;
    if (func_ov009_02183670(param_1, param_2, (s32 *)&psStack_14, (s32 *)&sStack_18) == 0) {
        *param_3 = 0;
        return 0;
    }
    num = sStack_18;
    if ((s32)param_4 < (s32)sStack_18) {
        num = param_4;
    }
    strncpy(param_3, psStack_14, num);
    num = param_4 - 1;
    if ((s32)(param_4 - 1) >= (s32)sStack_18) {
        num = sStack_18;
    }
    param_3[num] = 0;
    return 1;
}

u32 func_ov009_021837a4(u32 *reader, char *name, u8 *data, s32 length, u32 *len) {
    s32 arrayLength;
    GSIXmlElement *elem;
    s32 index;
    s32 val;
    s32 count;
    s32 pos;
    s32 valueLen;
    s32 hexLength;

    if (reader[3] == 0xffffffff) {
        reader[3] = reader[2];
    }
    index = reader[3] + 1;
    arrayLength = ArrayLength((void *)*reader);
    if (index < arrayLength) {
        do {
            elem = ArrayNth((void *)*reader, index);
            if ((elem->mParentIndex == reader[2]) && func_ov009_02183d98(name, (s32 *)elem)) {
                count = 0;
                val = 0;
                pos = 0;
                hexLength = length * 2;
                valueLen = elem->mValue.mLen;
                if (hexLength >= valueLen) {
                    hexLength = valueLen;
                }
                if (valueLen == 0 || elem->mValue.mData == NULL) {
                    *data = 0;
                    *len = 0;
                    return 1;
                }
                if (data == NULL) {
                    *len = valueLen / 2;
                    return 1;
                }
                while (hexLength > 1) {
                    STD_TSScanf((const char *)elem->mValue.mData + pos, "%02x", &val);
                    data[count] = val;
                    hexLength -= 2;
                    pos += 2;
                    count++;
                }
                if (hexLength == 1) {
                    STD_TSScanf((const char *)elem->mValue.mData + pos, "%01x", &val);
                    data[count] = val;
                    count++;
                }
                if (len != NULL) {
                    *len = count;
                }
                reader[3] = index;
                return 1;
            }
            if (elem->mParentIndex < (s32)reader[2]) {
                return 0;
            }
            index++;
            arrayLength = ArrayLength((void *)*reader);
        } while (index < arrayLength);
    }
    return 0;
}

#pragma thumb on

#pragma thumb off
u32 func_ov009_02183918(u32 *param_1, u8 *param_2, u8 *param_3, s32 *param_4) {
    BOOL result;
    s32 arrayLength;
    s32 *elem;
    u32 index;
    if (param_1[3] == 0xffffffff) {
        param_1[3] = param_1[2];
    }
    index = param_1[3] + 1;
    arrayLength = ArrayLength((void *)*param_1);
    if ((s32)index < arrayLength) {
        do {
            elem = ArrayNth((void *)*param_1, index);
            if ((elem[5] == param_1[2]) && (result = func_ov009_02183d98((s8 *)param_2, elem), result)) {
                if (param_3 != (u8 *)0x0) {
                    param_1[3] = index;
                    if (elem[2] != 0) {
                        B64Decode((char *)elem[2], (char *)param_3, elem[3], param_4, 0);
                    } else {
                        *param_4 = 0;
                    }
                } else if ((u8 *)elem[2] != (u8 *)0x0) {
                    *param_4 = func_ov009_0217fbc8((char *)elem[2], 0);
                } else {
                    *param_4 = 0;
                }
                return 1;
            }
            if (elem[5] < (s32)param_1[2]) {
                return 0;
            }
            index = index + 1;
            arrayLength = ArrayLength((void *)*param_1);
        } while ((s32)index < arrayLength);
    }
    return 0;
}
#pragma thumb on

#pragma thumb off
u32 func_ov009_02183a24(u32 *param_1, u8 *param_2, u32 *param_3) {
    BOOL result;
    s32 arrayLength;
    s32 *elem;
    u32 index;

    if (param_1[3] == 0xffffffff) {
        param_1[3] = param_1[2];
    }
    index = param_1[3] + 1;
    arrayLength = ArrayLength((void *)*param_1);
    if ((s32)index < arrayLength) {
        do {
            elem = ArrayNth((void *)*param_1, index);
            if ((elem[5] == param_1[2]) && (result = func_ov009_02183d98((s8 *)param_2, elem), result)) {
                param_1[3] = index;
                if ((s8 *)elem[2] != (s8 *)0x0) {
                    index = atol((s8 *)elem[2]);
                    *param_3 = index;
                    return 1;
                }
                return 0;
            }
            if (elem[5] < (s32)param_1[2]) {
                return 0;
            }
            index = index + 1;
            arrayLength = ArrayLength((void *)*param_1);
        } while ((s32)index < arrayLength);
    }
    return 0;
}
#pragma thumb on

#pragma thumb off
u32 func_ov009_02183ae4(u32 *param_1, u8 *param_2, s64 *param_3) {
    BOOL result;
    s32 arrayLength;
    s32 *elem;
    u32 index;

    if (param_1[3] == 0xffffffff) {
        param_1[3] = param_1[2];
    }
    index = param_1[3] + 1;
    arrayLength = ArrayLength((void *)*param_1);
    if ((s32)index < arrayLength) {
        do {
            elem = ArrayNth((void *)*param_1, index);
            if ((elem[5] == param_1[2]) && (result = func_ov009_02183d98((s8 *)param_2, elem), result)) {
                param_1[3] = index;
                if ((s8 *)elem[2] != (s8 *)0x0) {
                    *param_3 = func_ov009_0217f124((s8 *)elem[2]);
                    return 1;
                }
                return 0;
            }
            if (elem[5] < (s32)param_1[2]) {
                return 0;
            }
            index = index + 1;
            arrayLength = ArrayLength((void *)*param_1);
        } while ((s32)index < arrayLength);
    }
    return 0;
}

u32 func_ov009_02183ba4(u32 *param_1, u8 *param_2, s32 *param_3) {
    GSIXmlElement *element;
    struct tm time;
    s32 i;

    if ((s32)param_1[3] == -1) {
        param_1[3] = param_1[2];
    }
    for (i = param_1[3] + 1; i < ArrayLength((void *)*param_1); i++) {
        element = ArrayNth((void *)*param_1, i);
        if (element->mParentIndex == (s32)param_1[2] && func_ov009_02183d98((s8 *)param_2, (s32 *)element)) {
            param_1[3] = i;
            if (element->mValue.mData == NULL) {
                return FALSE;
            }
            STD_TSScanf((const char *)element->mValue.mData, "%i-%02d-%02dT%02d:%02d:%02d", &time.tm_year, &time.tm_mon, &time.tm_mday, &time.tm_hour, &time.tm_min, &time.tm_sec);
            time.tm_year -= 1900;
            time.tm_mon -= 1;
            time.tm_isdst = -1;
            *param_3 = func_ov009_0217f478(&time);
            return TRUE;
        }
        if (element->mParentIndex < (s32)param_1[2]) {
            return FALSE;
        }
    }
    return FALSE;
}


u32 func_ov009_02183cd4(u32 *reader, u8 *name, f32 *value) {
    BOOL result;
    s32 arrayLength;
    GSIXmlElement *elem;
    u32 index;

    if (reader[3] == 0xffffffff) {
        reader[3] = reader[2];
    }
    index = reader[3] + 1;
    arrayLength = ArrayLength((void *)*reader);
    if ((s32)index < arrayLength) {
        do {
            elem = ArrayNth((void *)*reader, index);
            if ((elem->mParentIndex == reader[2]) && (result = func_ov009_02183d98((s8 *)name, (s32 *)elem), result)) {
                reader[3] = index;
                if (elem->mValue.mData != NULL) {
                    *value = atof((const char *)elem->mValue.mData);
                    return 1;
                }
                return 0;
            }
            if (elem->mParentIndex < (s32)reader[2]) {
                return 0;
            }
            index = index + 1;
            arrayLength = ArrayLength((void *)*reader);
        } while ((s32)index < arrayLength);
    }
    return 0;
}

#pragma thumb on

#pragma thumb off
BOOL func_ov009_02183d98(s8 *param_1, s32 *param_2) {
    size_t len;
    s8 *str;
    s32 val;
    s32 i;
    size_t num;
    i = 0;
    if (param_1 == NULL) {
        return 1;
    }
    len = strlen(param_1);
    if (param_1[len - 1] == 0x3a) {
        return 0;
    }
    str = strchr(param_1, 0x3a);
    if (str == NULL) {
        str = param_1;
    }
    val = param_2[1];
    for (; (i < val && (*(u8 *)(*param_2 + i) != 0x3a)); i = i + 1) {
    }
    if (i == val) {
        i = 0;
    } else {
        i = i + 1;
    }
    num = val - i;
    val = *param_2;
    len = strlen(str);
    if (num <= len) {
        num = strlen(str);
    }
    return (u32)(strncmp(str, (s8 *)(val + i), num) == 0);
}

BOOL func_ov009_02183e50(u32 *param_1, char *param_2, LargeIntBuf *value) {
    u32 readSize;

    readSize = 0;
    memset(value, 0, sizeof(LargeIntBuf));
    if (!func_ov009_021837a4(param_1, param_2, (u8 *)value->data, 0x200, &readSize)) {
        return FALSE;
    }
    value->length = readSize / 4;
    if (readSize & 3) {
        value->length++;
    }
    if (!func_ov009_0217e934(value)) {
        return FALSE;
    }
    return TRUE;
}

#pragma thumb on

#pragma thumb off
s32 func_ov009_02183ee0(u32 *param_1, s8 *param_2) {
    s32 *elem;
    s32 val;
    s32 val2;
    s32 index;
    s32 i;

    index = *(s32 *)((u8 *)param_1 + 8) + 1;
    i = 0;
    if (index < ArrayLength((void *) *(u32 *)param_1)) {
loop_1:
        elem = ArrayNth((void *) *(u32 *)param_1, index);
        val2 = *(s32 *)((u8 *)param_1 + 8);
        val = *(s32 *)((u8 *)elem + 0x14);
        if (val == val2) {
            if ((param_2 == NULL) || (func_ov009_02183d98(param_2, elem) != 0)) {
                i += 1;
            }
            goto block_6;
        }
        if (val >= val2) {
block_6:
            index += 1;
            if (index < ArrayLength((void *) *(u32 *)param_1)) {
                goto loop_1;
            }
        }
    }
    return i;
}
#pragma thumb on
