// Black's code at ov114 0x021c235c-0x021c3a38. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
// (the same functions in the same order); the name is that file's. GameSpy's ghttpPost.c: the posts of a request (the form
// or the XML message that it sends), as pokebw2 has them.

#include <string.h>
#include <stdio.h>
#include "types.h"
#include "dwc/dwc.h"
#include "dwc/ext.h"
#include "dwc/nhttp.h"
#include "dwc/base.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "nitro/std.h"
#include "nitrowifi/cps.h"
#include "nitrowifi/soc.h"
#include "field/unity_tower.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/str_tool.h"
#include "gs/nonport.h"
#include "gs/darray.h"
#include "nitro/os.h"
#include "gfl/blact.h"
#include "dwc/ghttp_internal.h"

void rewind(FILE *file);
size_t fread(void *ptr, size_t memb_size, size_t num_memb, FILE *file);

// The other ghttp files of Black still take the connections and the buffers as s32 *
#define ghiAppendDataToBuffer(buffer, data, size) func_ov114_021bfcec((s32 *)(buffer), (s8 *)(data), (size))
#define ghiEncryptDataToBuffer(buffer, data, size) func_ov114_021bfdec((s32 *)(buffer), (s8 *)(data), (size))
#define ghiAppendCharToBuffer(buffer, c) func_ov114_021c0004((s32 *)(buffer), (c))
#define ghiResetBuffer(buffer) func_ov114_021c00fc((s32 *)(buffer))
#define ghiSendBufferedData(connection) func_ov114_021c011c((s32 *)(connection))
#define ghiDoSend(connection, data, size) func_ov114_021c0684((GHIConnection *)(connection), (const char *)(data), (size))
#define ghiTrySendThenBuffer(connection, data, size) func_ov114_021c07b8((s32 *)(connection), (u8 *)(data), (size))
#define ghiRequestToConnection(request) ((GHIConnection *)func_ov114_021c0da4(request))

// The soap writer of the XML message, which ov009's xml.c has
extern const char *func_ov009_02181e18(SoapWriter *writer);
extern int func_ov009_02181e20(SoapWriter *writer);
extern void func_ov009_02181d84(SoapWriter *writer);

// The header of a DIME record, with its numbers in network order
typedef struct GHIDimeHeader {
    u8 flags;
    u8 type;
    u16 optionsLength;
    u16 idLength;
    u16 typeLength;
    u32 dataLength;
} GHIDimeHeader;

#define GHI_DIME_VERSION 0x08
#define GHI_DIME_FIRST 0x04
#define GHI_DIME_LAST 0x02
#define GHI_DIME_TYPE_MEDIA 0x10
#define GHI_DIME_TYPE_URI 0x20

#define GHI_SWAP16(x) ((((x) >> 8) & 0xff) | (((x) << 8) & 0xff00))
// The length of a text as the 16 bits of a DIME header, in network order
#define GHI_DIME_LENGTH16(x) GHI_SWAP16((u16)(s16)(x))
#define GHI_SWAP32(x)                                                                                                  \
    (((((u32)(x)) << 24) & 0xff000000) |                                                                               \
     (((((u32)(x)) << 8) & 0x00ff0000) | (((((u32)(x)) >> 24) & 0xff) | ((((u32)(x)) >> 8) & 0x0000ff00))))

// How many bytes a text of this length is padded with to the next multiple of 4 (4 for none)
#define GHI_DIME_PAD(length) (4 - ((length) % 4))

#define GHI_BOUNDARY "Qr4G823s23d---<<><><<<>--7d118e0536"
#define GHI_BOUNDARY_LINE "--" GHI_BOUNDARY
#define GHI_MESSAGE_ID "cid:id0"
#define GHI_MESSAGE_TYPE "http://schemas.xmlsoap.org/soap/envelope/"

// The characters that urlencoding leaves as they are
// pokebw2's sUnreservedChars
static char data_ov114_021ced88[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_@-.*";

// A short copy, which pokebw2 writes as a loop
static inline void ghiCopy(void *dest, const void *src, u32 size) {
    u8 *d = dest;
    const u8 *s = src;

    while (size) {
        *d++ = *s++;
        size--;
    }
}

#pragma thumb off
// ghiFreePostItem: frees what an item holds
void func_ov114_021c235c(void *element) {
    GHIPostData *data = element;

    if (data->type != GHIXml) {
        gsifree(data->name);
    }
    if (data->type == GHIString) {
        gsifree(data->data.string.string);
    } else if (data->type == GHIFileDisk) {
        gsifree(data->data.fileDisk.filename);
        gsifree(data->data.fileDisk.reportFilename);
        gsifree(data->data.fileDisk.contentType);
    } else if (data->type == GHIFileMemory) {
        gsifree(data->data.fileMemory.reportFilename);
        gsifree(data->data.fileMemory.contentType);
    } else if (data->type == GHIXml) {
        func_ov009_02181d84(data->data.xml.writer);
    }
}

// ghiNewPost
GHIPost *func_ov114_021c23e4(void) {
    GHIPost *post = gsimalloc(sizeof(GHIPost));

    if (post == NULL) {
        return NULL;
    }
    memset(post, 0, sizeof(GHIPost));
    post->autoFree = TRUE;
    post->data = ArrayNew(sizeof(GHIPostData), 0, func_ov114_021c235c);
    if (post->data != NULL) {
        return post;
    }
    gsifree(post);
    return NULL;
}

// ghiPostSetAutoFree
void func_ov114_021c245c(GHIPost *post, BOOL autoFree) {
    post->autoFree = autoFree;
}

// ghiFreePost
void func_ov114_021c2464(GHIPost *post) {
    if (post->data != NULL) {
        ArrayFree(post->data);
    }
    post->data = NULL;
    gsifree(post);
}

// Frees a post of a request, and what the request keeps of it
void func_ov114_021c2490(int request, GHIPost *post) {
    GHIConnection *connection = ghiRequestToConnection(request);

    if (connection != NULL) {
        func_ov114_021c2bb0(connection);
    }
    func_ov114_021c2464(post);
}

// ghttpPostAddFileFromMemory: adds a file that is in memory; the post copies the texts but not the bytes
BOOL func_ov114_021c24b4(GHIPost *post, const char *name, const void *buffer, int len, const char *reportFilename, const char *contentType) {
    char *nameCopy = func_ov009_0217f1e0(name);
    char *fileNameCopy = func_ov009_0217f1e0(reportFilename);
    char *contentTypeCopy = func_ov009_0217f1e0(contentType);

    if (nameCopy == NULL || fileNameCopy == NULL || contentTypeCopy == NULL) {
        gsifree(nameCopy);
        gsifree(fileNameCopy);
        gsifree(contentTypeCopy);
        return FALSE;
    } else {
        GHIPostData data = { 0 };

        data.type = GHIFileMemory;
        data.name = nameCopy;
        data.data.fileMemory.buffer = buffer;
        data.data.fileMemory.len = len;
        data.data.fileMemory.reportFilename = fileNameCopy;
        data.data.fileMemory.contentType = contentTypeCopy;
        ArrayAppend(post->data, &data);
        post->hasFiles = TRUE;
        if (post->hasXml == TRUE) {
            post->mixed = TRUE;
        }
        return TRUE;
    }
}

// ghiPostAddXml: adds an XML message, which the post takes over
BOOL func_ov114_021c2584(GHIPost *post, SoapWriter *writer) {
    GHIPostData data;

    data.type = GHIXml;
    data.data.xml.writer = writer;
    ArrayAppend(post->data, &data);
    post->hasXml = TRUE;
    if (post->hasFiles == TRUE) {
        post->mixed = TRUE;
    }
    return TRUE;
}

// ghiPostGetContentType: the content type that the request says its post is
const char *func_ov114_021c25c8(GHIConnection *connection) {
    GHIPost *post = connection->post;

    if (post == NULL) {
        return "";
    }
    if (post->mixed) {
        return "application/dime";
    }
    if (post->hasFiles) {
        return "multipart/form-data; boundary=" GHI_BOUNDARY;
    }
    if (post->hasXml) {
        return "text/xml";
    }
    return "application/x-www-form-urlencoded";
}

// ghiPostGetUrlEncodedLength: the length of a post without files: the names, the values (as they will be urlencoded) and
// the = and & between them
int func_ov114_021c2620(GHIConnection *connection) {
    GHIPostData *data;
    GHIPost *post = connection->post;
    int i;
    int num;
    int total = 0;

    num = ArrayLength(post->data);
    if (!num) {
        return 0;
    }
    for (i = 0; i < num; i++) {
        data = ArrayNth(post->data, i);
        if (data->type == GHIString) {
            total += strlen(data->name);
            total += data->data.string.len;
            total += data->data.string.extendedChars * 2;
            total++;
        } else if (data->type == GHIXml) {
            total += func_ov009_02181e20(data->data.xml.writer);
        }
    }
    total += num - 1;
    return total;
}

// ghiPostGetMultipartLength: the length of a post with files: what the multipart format or the DIME one add to each
// item, and the item
int func_ov114_021c26b8(GHIConnection *connection) {
    GHIPost *post = connection->post;
    GHIPostData *data;
    int i;
    int length = 0;
    int count;
    int stringExtra;
    int fileExtra;
    int lastExtra;
    int xmlExtra;
    int padding;

    if (post->mixed) {
        stringExtra = 12;
        fileExtra = 12;
        xmlExtra = 12;
        lastExtra = 0;
    } else {
        int boundaryLength = strlen(GHI_BOUNDARY_LINE);

        stringExtra = boundaryLength + 0x2f;
        fileExtra = boundaryLength + 0x4c;
        xmlExtra = 0;
        lastExtra = boundaryLength + 4;
    }

    count = ArrayLength(post->data);
    for (i = 0; i < count; i++) {
        data = ArrayNth(post->data, i);

        if (data->type == GHIString) {
            length += stringExtra;
            length += strlen(data->name);
            length += data->data.string.len;
        } else if (data->type == GHIFileDisk) {
            GHIPostState *state;
            BOOL mixed;
            int fileSize;

            length += fileExtra;
            length += strlen(data->name);
            length += strlen(data->data.fileDisk.contentType);
            state = ArrayNth(connection->postingState.states, i);
            fileSize = state->fileSize;
            length += fileSize;
            mixed = post->mixed;
            if (!mixed) {
                length += strlen(data->data.fileDisk.reportFilename);
            }
            if (mixed) {
                padding = GHI_DIME_PAD((int)strlen(data->name));
                if (padding != 4) {
                    length += padding;
                }
                padding = GHI_DIME_PAD((int)strlen(data->data.fileDisk.contentType));
                if (padding != 4) {
                    length += padding;
                }
                padding = GHI_DIME_PAD(fileSize);
                if (padding != 4) {
                    length += padding;
                }
            }
        } else if (data->type == GHIFileMemory) {
            BOOL mixed;
            char *name;
            char *contentType;
            int dataLength;

            length += fileExtra;
            name = data->name;
            length += strlen(name);
            contentType = data->data.fileMemory.contentType;
            length += strlen(contentType);
            dataLength = data->data.fileMemory.len;
            length += dataLength;
            mixed = post->mixed;
            if (!mixed) {
                length += strlen(data->data.fileMemory.reportFilename);
            }
            if (mixed) {
                padding = GHI_DIME_PAD((int)strlen(name));
                if (padding != 4) {
                    length += padding;
                }
                padding = GHI_DIME_PAD((int)strlen(contentType));
                if (padding != 4) {
                    length += padding;
                }
                padding = GHI_DIME_PAD(dataLength);
                if (padding != 4) {
                    length += padding;
                }
            }
        } else if (data->type == GHIXml) {
            length += xmlExtra;
            length += func_ov009_02181e20(data->data.xml.writer);
            padding = GHI_DIME_PAD(func_ov009_02181e20(data->data.xml.writer));
            if (padding != 4) {
                length += padding;
            }
            length += strlen(GHI_MESSAGE_ID);
            padding = GHI_DIME_PAD((int)strlen(GHI_MESSAGE_ID));
            if (padding != 4) {
                length += padding;
            }
            length += strlen(GHI_MESSAGE_TYPE);
            padding = GHI_DIME_PAD((int)strlen(GHI_MESSAGE_TYPE));
            if (padding != 4) {
                length += padding;
            }
        } else {
            return 0;
        }
    }
    return length + lastExtra;
}

// ghiPostGetLength
int func_ov114_021c297c(GHIConnection *connection) {
    if (connection->post == NULL) {
        return 0;
    }
    if (connection->post->hasFiles) {
        return func_ov114_021c26b8(connection);
    }
    return func_ov114_021c2620(connection);
}

// ghiPostStateInit: starts an item's state: a file on disk is measured
BOOL func_ov114_021c29ac(GHIPostState *state) {
    int type = state->data->type;

    state->pos = -1;
    if (type == GHIString) {
    } else if (type == GHIFileDisk) {
        if (state->file == NULL) {
            return FALSE;
        }
        if (fseek(state->file, 0, 2) != 0) {
            return FALSE;
        }
        state->fileSize = ftell(state->file);
        if (state->fileSize == -1) {
            return FALSE;
        }
        rewind(state->file);
    } else if (type == GHIFileMemory || type == GHIXml) {
    } else {
        return FALSE;
    }
    return TRUE;
}

// ghiPostStateCleanup
void func_ov114_021c2a3c(GHIPostState *state) {
    int type = state->data->type;

    if (type == GHIString) {
    } else if (type == GHIFileDisk) {
        if (state->file != NULL) {
            fclose(state->file);
        }
        state->file = NULL;
    }
}

// ghiPostInitState: makes the states of the items of the post of a connection and works out how long the post is
BOOL func_ov114_021c2a78(GHIConnection *connection) {
    GHIPost *post = connection->post;
    int count;
    int i;

    if (post == NULL) {
        return FALSE;
    }
    connection->postingState.index = 0;
    connection->postingState.bytesPosted = 0;
    connection->postingState.totalBytes = 0;
    connection->postingState.finished = 0;
    connection->postingState.callback = post->callback;
    connection->postingState.param = post->param;
    count = ArrayLength(post->data);
    connection->postingState.states = ArrayNew(sizeof(GHIPostState), count, NULL);
    if (connection->postingState.states == NULL) {
        return FALSE;
    }
    for (i = 0; i < count; i++) {
        GHIPostData *data = ArrayNth(connection->post->data, i);
        GHIPostState state = { 0 };

        state.data = data;
        if (!func_ov114_021c29ac(&state)) {
            for (i--; i >= 0; i--) {
                func_ov114_021c2a3c(ArrayNth(connection->postingState.states, i));
            }
            ArrayFree(connection->postingState.states);
            connection->postingState.states = NULL;
            return FALSE;
        }
        ArrayAppend(connection->postingState.states, &state);
    }
    connection->postingState.totalBytes = func_ov114_021c297c(connection);
    if (connection->post->hasXml == TRUE) {
        connection->postingState.isXml = TRUE;
    } else {
        connection->postingState.isXml = FALSE;
    }
    return TRUE;
}

// ghiPostCleanupState
void func_ov114_021c2bb0(GHIConnection *connection) {
    int i;
    int len;

    if (connection->postingState.states != NULL) {
        len = ArrayLength(connection->postingState.states);
        for (i = 0; i < len; i++) {
            func_ov114_021c2a3c(ArrayNth(connection->postingState.states, i));
        }
        ArrayFree(connection->postingState.states);
        connection->postingState.states = NULL;
    }
    if (connection->post != NULL && connection->post->autoFree != 0) {
        func_ov114_021c2464(connection->post);
        connection->post = NULL;
    }
}

// ghiPostStringStateDoPosting: sends a string item's value, urlencoded if the post is. Returns 0 for an error, 1 once it is
// all sent
BOOL func_ov114_021c2c28(GHIPostState *state, GHIConnection *connection) {
    GHIPostData *data = state->data;
    GHIPost *post;
    int i;
    char c;

    if (data->data.string.len == 0) {
        return TRUE;
    }
    post = connection->post;
    if (!post->hasFiles && !post->hasXml && data->data.string.invalidChars != 0) {
        char *value = data->data.string.string;
        char escape[4] = "%00";
        GHIBuffer *buffer;
        const char *hexDigits;

        if (connection->encryptor.engine == 0) {
            buffer = &connection->sendBuffer;
        } else {
            buffer = &connection->encodeBuffer;
        }
        i = 0;
        c = value[0];
        while (c != '\0') {
            if (strchr(data_ov114_021ced88, c) != NULL) {
                ghiAppendCharToBuffer(buffer, c);
            } else if (c == ' ') {
                ghiAppendCharToBuffer(buffer, '+');
            } else {
                hexDigits = "0123456789ABCDEF";
                escape[1] = hexDigits[c / 16];
                escape[2] = hexDigits[c % 16];
                ghiAppendDataToBuffer(buffer, escape, 3);
            }
            i++;
            c = value[i];
        }
    } else {
        switch (ghiTrySendThenBuffer(connection, data->data.string.string, data->data.string.len)) {
        case 0:
            return FALSE;
        default:
            return TRUE;
        }
    }
    if (connection->encryptor.engine == 0) {
        if (!ghiSendBufferedData(connection)) {
            return FALSE;
        }
        if (connection->sendBuffer.pos == connection->sendBuffer.length) {
            ghiResetBuffer(&connection->sendBuffer);
        }
        return TRUE;
    } else {
        return TRUE;
    }
}

// ghiPostXmlStateDoPosting: sends an XML message, with the padding that a DIME record has
BOOL func_ov114_021c2db0(GHIPostState *state, GHIConnection *connection) {
    SoapWriter *message = state->data->data.xml.writer;
    char padding[3] = { 0, 0, 0 };
    int padLength = 0;

    if (connection->post->mixed) {
        padLength = 4 - func_ov009_02181e20(message) % 4;
        if (padLength == 4) {
            padLength = 0;
        }
    }
    if (connection->encryptor.engine != 0 && connection->encryptor.mode == 1) {
        if (!ghiAppendDataToBuffer(&connection->encodeBuffer, func_ov009_02181e18(message), func_ov009_02181e20(message)) ||
            !ghiAppendDataToBuffer(&connection->encodeBuffer, padding, padLength) ||
            !ghiEncryptDataToBuffer(&connection->sendBuffer, connection->encodeBuffer.data, connection->encodeBuffer.length)) {
            return FALSE;
        }
        ghiResetBuffer(&connection->encodeBuffer);
        if (!ghiSendBufferedData(connection)) {
            return FALSE;
        }
        if (connection->sendBuffer.pos == connection->sendBuffer.length) {
            ghiResetBuffer(&connection->sendBuffer);
        }
        return TRUE;
    }
    if (!ghiTrySendThenBuffer(connection, func_ov009_02181e18(message), func_ov009_02181e20(message))) {
        return FALSE;
    }
    switch (ghiTrySendThenBuffer(connection, padding, padLength)) {
    case 0:
        return FALSE;
    default:
        return TRUE;
    }
}

// ghiPostFileDiskStateDoPosting: sends a file of the disk, a piece at a time. Returns 0 for an error, 1 once it is all
// sent and 2 to go on later
int func_ov114_021c2f10(GHIPostState *state, GHIConnection *connection) {
    char buffer[0x1000];
    int count;
    int result;

    do {
        count = fread(buffer, 1, sizeof(buffer), state->file);
        if (count <= 0) {
            connection->completed = TRUE;
            connection->result = 14;
            return 0;
        }
        state->pos += count;
        if (state->pos > state->fileSize) {
            connection->completed = TRUE;
            connection->result = 14;
            return 0;
        }
        result = ghiTrySendThenBuffer(connection, buffer, count);
        if (!result) {
            return 0;
        }
        if (state->pos == state->fileSize) {
            if (connection->post->mixed) {
                char padding[3] = { 0, 0, 0 };
                int padLength;

                padLength = 4 - state->fileSize % 4;
                if (padLength != 4 && padLength > 0) {
                    if (!ghiTrySendThenBuffer(connection, padding, padLength)) {
                        return 0;
                    }
                }
            }
            return 1;
        }
    } while (result == 1);
    return 2;
}

// ghiPostFileMemoryStateDoPosting: sends a file that is in memory: the same
int func_ov114_021c304c(GHIPostState *state, GHIConnection *connection) {
    GHIPostData *data = state->data;
    int chunk;
    int length = data->data.fileMemory.len;
    int result;

    if (length == 0) {
        return 1;
    }
    if (connection->encryptor.engine == 0) {
        do {
            result = ghiDoSend(connection, data->data.fileMemory.buffer + state->pos, length - state->pos);
            if (result == -1) {
                return 0;
            }
            state->pos += result;
            data = state->data;
            length = data->data.fileMemory.len;
            if (length == state->pos) {
                if (connection->post->mixed) {
                    char padding[3] = { 0, 0, 0 };
                    int padLength;

                    padLength = 4 - data->data.fileMemory.len % 4;
                    if (padLength != 4 && padLength > 0) {
                        if (!ghiTrySendThenBuffer(connection, padding, padLength)) {
                            return 0;
                        }
                    }
                }
                return 1;
            }
        } while (result != 0);
        return 2;
    }
    do {
        chunk = length - state->pos;

        if (chunk >= 0x3f01) {
            chunk = 0x3f01;
        }
        result = ghiTrySendThenBuffer(connection, data->data.fileMemory.buffer + state->pos, chunk);
        if (!result) {
            return 0;
        }
        state->pos += chunk;
        data = state->data;
        length = data->data.fileMemory.len;
        if (length == state->pos) {
            if (connection->post->mixed) {
                char padding[3] = { 0, 0, 0 };
                int padLength;

                padLength = 4 - data->data.fileMemory.len % 4;
                if (padLength != 4 && padLength > 0) {
                    if (!ghiTrySendThenBuffer(connection, padding, padLength)) {
                        return 0;
                    }
                }
            }
            return 1;
        }
    } while (result == 1);
    return 2;
}

// ghiPostStateDoPosting: sends an item: its head (once) and then what it holds. Returns 0 for an error, 1 once it is all
// sent and 2 to go on later
int func_ov114_021c3218(GHIPostState *state, GHIConnection *connection, BOOL first, BOOL last) {
    char buffer[0x800];
    GHIDimeHeader headers[2];
    int length = 0;
    int result;

    if (state->pos == -1) {
        state->pos = 0;
        if (!connection->post->hasFiles && !connection->post->hasXml) {
            if (first) {
                OS_SPrintf(buffer, "%s=", state->data->name);
            } else {
                OS_SPrintf(buffer, "&%s=", state->data->name);
            }
        } else {
            GHIPostData *data = state->data;

            if (data->type == GHIString) {
                OS_SPrintf(buffer, "%sContent-Disposition: form-data; name=\"%s\"\r\n\r\n",
                           first ? GHI_BOUNDARY_LINE "\r\n" : "\r\n" GHI_BOUNDARY_LINE "\r\n", data->name);
            } else if (data->type == GHIXml) {
                if (connection->post->mixed) {
                    int pos = 0;
                    int count;
                    int padding;

                    headers[1].flags = GHI_DIME_VERSION;
                    if (first) {
                        headers[1].flags |= GHI_DIME_FIRST;
                    }
                    if (last) {
                        headers[1].flags |= GHI_DIME_LAST;
                    }
                    headers[1].type = GHI_DIME_TYPE_URI;
                    headers[1].optionsLength = 0;
                    headers[1].idLength = GHI_DIME_LENGTH16(strlen(GHI_MESSAGE_ID));
                    headers[1].typeLength = GHI_DIME_LENGTH16(strlen(GHI_MESSAGE_TYPE));
                    headers[1].dataLength = GHI_SWAP32(func_ov009_02181e20(state->data->data.xml.writer));
                    memcpy(&buffer[pos], &headers[1], sizeof(GHIDimeHeader));
                    pos += sizeof(GHIDimeHeader);
                    count = func_ov009_021803bc(&buffer[pos], (s8 *)GHI_MESSAGE_ID, sizeof(buffer) - pos);
                    pos += count;
                    padding = 4 - (count & 3);
                    if (padding != 4) {
                        while (padding-- > 0) {
                            buffer[pos++] = 0;
                        }
                    }
                    count = func_ov009_021803bc(&buffer[pos], (s8 *)GHI_MESSAGE_TYPE, sizeof(buffer) - pos);
                    pos += count;
                    padding = 4 - (count & 3);
                    if (padding != 4) {
                        while (padding-- > 0) {
                            buffer[pos++] = 0;
                        }
                    }
                    length = pos;
                } else {
                    buffer[0] = 0;
                }
            } else if (data->type == GHIFileDisk || data->type == GHIFileMemory) {
                char *contentType;
                int dataLength;
                char *fileName;

                if (data->type == GHIFileDisk) {
                    dataLength = state->fileSize;
                    fileName = data->data.fileDisk.reportFilename;
                    contentType = data->data.fileDisk.contentType;
                } else {
                    dataLength = data->data.fileMemory.len;
                    fileName = data->data.fileMemory.reportFilename;
                    contentType = data->data.fileMemory.contentType;
                }
                if (connection->post->mixed) {
                    int pos = 0;
                    int count;
                    int padding;
                    char *name;

                    headers[0].flags = GHI_DIME_VERSION;
                    if (first) {
                        headers[0].flags |= GHI_DIME_FIRST;
                    }
                    if (last) {
                        headers[0].flags |= GHI_DIME_LAST;
                    }
                    headers[0].type = GHI_DIME_TYPE_MEDIA;
                    headers[0].optionsLength = 0;
                    name = state->data->name;
                    headers[0].idLength = GHI_DIME_LENGTH16(strlen(name));
                    headers[0].typeLength = GHI_DIME_LENGTH16(strlen(contentType));
                    headers[0].dataLength = GHI_SWAP32(dataLength);
                    memcpy(&buffer[pos], &headers[0], sizeof(GHIDimeHeader));
                    pos += sizeof(GHIDimeHeader);
                    count = func_ov009_021803bc(&buffer[pos], (s8 *)state->data->name, sizeof(buffer) - pos);
                    pos += count;
                    padding = 4 - (count & 3);
                    if (padding != 4) {
                        while (padding-- > 0) {
                            buffer[pos++] = 0;
                        }
                    }
                    count = func_ov009_021803bc(&buffer[pos], (s8 *)contentType, sizeof(buffer) - pos);
                    pos += count;
                    padding = 4 - (count & 3);
                    if (padding != 4) {
                        while (padding-- > 0) {
                            buffer[pos++] = 0;
                        }
                    }
                    length = pos;
                } else {
                    OS_SPrintf(buffer,
                               "%sContent-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\nContent-Type: "
                               "%s\r\n\r\n",
                               first ? GHI_BOUNDARY_LINE "\r\n" : "\r\n" GHI_BOUNDARY_LINE "\r\n", data->name, fileName,
                               contentType);
                }
            }
        }

        if (connection->encryptor.engine != 0 && connection->encryptor.mode == 1) {
            if (length == 0) {
                length = strlen(buffer);
            }
            if (!ghiEncryptDataToBuffer(&connection->sendBuffer, buffer, length)) {
                return 0;
            }
            if (!ghiSendBufferedData(connection)) {
                return 0;
            }
            if (connection->sendBuffer.pos < connection->sendBuffer.length) {
                return 2;
            }
            ghiResetBuffer(&connection->sendBuffer);
        } else {
            if (length == 0) {
                length = strlen(buffer);
            }
            result = ghiTrySendThenBuffer(connection, buffer, length);
            if (!result) {
                return 0;
            }
            if (result == 2) {
                return 2;
            }
            ghiResetBuffer(&connection->sendBuffer);
        }
    }

    if (state->data->type == GHIString) {
        return func_ov114_021c2c28(state, connection);
    }
    if (state->data->type == GHIXml) {
        return func_ov114_021c2db0(state, connection);
    }
    if (state->data->type == GHIFileDisk) {
        return func_ov114_021c2f10(state, connection);
    }
    return func_ov114_021c304c(state, connection);
}

// ghiPostDoPosting: sends the post of a connection, an item at a time. Returns 0 for an error, 1 once it is all sent, 2 to
// go on later and 3 once the head of an XML message is sent
int func_ov114_021c3834(GHIConnection *connection) {
    GHIPostingState *posting = &connection->postingState;
    GHIPostState *state;
    int count = ArrayLength(posting->states);

    if (connection->sendBuffer.pos < connection->sendBuffer.length) {
        if (!ghiSendBufferedData(connection)) {
            return 0;
        }
        if (connection->sendBuffer.pos < connection->sendBuffer.length) {
            return 2;
        }
        ghiResetBuffer(&connection->sendBuffer);
        if (connection->postingState.isXml) {
            return 3;
        }
        if (connection->postingState.index == count) {
            return 1;
        }
    }

    if (connection->postingState.isXml) {
        if (connection->post->hasFiles || connection->post->hasXml) {
            const char *lineEnd = "\r\n";
            int result = ghiTrySendThenBuffer(connection, lineEnd, strlen(lineEnd));

            if (!result) {
                return 0;
            }
            if (result == 2) {
                return 2;
            }
            if (connection->postingState.isXml == 1) {
                return 3;
            }
        } else {
            connection->postingState.isXml = FALSE;
        }
    }

    while (posting->index < count) {
        state = ArrayNth(posting->states, posting->index);
        int index = posting->index;
        BOOL last = index == ArrayLength(posting->states) - 1;
        int result = func_ov114_021c3218(state, connection, index == 0, last);

        if (!result) {
            return 0;
        }
        if (result == 2) {
            return 2;
        }
        posting->index++;
    }

    if (connection->encryptor.engine != 0) {
        if (connection->encodeBuffer.length > 0) {
            if (!ghiEncryptDataToBuffer(&connection->sendBuffer, connection->encodeBuffer.data, connection->encodeBuffer.length)) {
                return 0;
            }
            ghiResetBuffer(&connection->encodeBuffer);
        }
    }
    if (connection->post->hasFiles && !connection->post->mixed) {
        const char *lastBoundary = "\r\n" GHI_BOUNDARY_LINE "--\r\n";

        if (!ghiTrySendThenBuffer(connection, lastBoundary, strlen(lastBoundary))) {
            return 0;
        }
    }
    if (connection->sendBuffer.pos < connection->sendBuffer.length) {
        return 2;
    }
    return 1;
}
