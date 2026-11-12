// Host unit test of the PC build's SDK gaps: port/src/sdk_mi.c, sdk_dgt.c and libntr as patched by
// port/subprojects/packagefiles/libntr-sdk-gaps.patch (SVC_GetCRC16, SVC_CpuSet, SVC_CpuSetFast, the fx matrix
// functions that were "Not implemented"). Pure functions only: nothing here needs libntr's simulator, so it links the
// archives with unresolved symbols ignored. port/tests/run_sdk_test.sh builds and runs it:
//
//     port/tests/run_sdk_test.sh LIBNTR_SRC LIBNTR_BUILD LIBNTRSYSTEM_SRC [ROM]
//
// ROM (a Black ROM, e.g. orig/baserom_b_us.nds) adds a check of SVC_GetCRC16 against the CRCs in its header, which the
// DS BIOS checks with the same function. Exit 0 when every check passes.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdk_black.h"
#include <nitro/dgt/dgt.h>

static int sFails, sChecks;

#define CHECK(cond, ...)                                                                                               \
    do {                                                                                                               \
        sChecks++;                                                                                                     \
        if (!(cond)) {                                                                                                 \
            sFails++;                                                                                                  \
            printf("FAIL %s:%d: ", __FILE__, __LINE__);                                                                \
            printf(__VA_ARGS__);                                                                                       \
            printf("\n");                                                                                              \
        }                                                                                                              \
    } while (0)

static u32 sRand = 12345;
static u32 Rand(void)
{
    sRand = sRand * 1103515245 + 12345;
    return sRand >> 8;
}

// The BIOS's algorithm as GBATEK gives it (bit by bit, the reflected polynomial 0xa001), halfwords
static u16 RefCrc16(u32 start, const u8 *p, u32 size)
{
    u32 crc = start & 0xffff;
    u32 i, j;
    for (i = 0; i < (size & ~1u); i++) {
        crc ^= p[i];
        for (j = 0; j < 8; j++) {
            crc = (crc >> 1) ^ ((crc & 1) ? 0xa001 : 0);
        }
    }
    return (u16)crc;
}

static void TestCrc(const char *rom)
{
    static const char check[] = "123456789";
    MATHCRC16Table table;
    u8 buf[1024];
    u32 i, n;

    // the catalogue's check values: CRC-16/MODBUS (from 0xffff) and CRC-16/ARC (from 0)
    CHECK(SVC_GetCRC16(0xffff, "12345678", 8) == RefCrc16(0xffff, (const u8 *)"12345678", 8), "8 bytes");
    {
        u8 nine[10];
        memcpy(nine, check, 9);
        nine[9] = 0;
        // 9 bytes: the BIOS takes halfwords, so it leaves the last byte out
        CHECK(SVC_GetCRC16(0xffff, nine, 9) == SVC_GetCRC16(0xffff, nine, 8), "odd size");
    }
    CHECK(SVC_GetCRC16(0, "", 0) == 0, "empty from 0");
    CHECK(SVC_GetCRC16(0xffff, "", 0) == 0xffff, "empty from 0xffff");

    // against libntr's table-driven MATH CRC-16 (the same polynomial, reflected), from the BIOS's usual 0xffff and 0
    MATHi_CRC16InitTableRev(&table, 0xa001);
    for (n = 0; n < 200; n++) {
        u32 size = (Rand() % sizeof(buf)) & ~1u;
        u16 start = (n & 1) ? 0xffff : (u16)Rand();
        MATHCRC16Context ctx = start;
        for (i = 0; i < size; i++) {
            buf[i] = (u8)Rand();
        }
        MATHi_CRC16UpdateRev(&table, &ctx, buf, size);
        CHECK(SVC_GetCRC16(start, buf, size) == (u16)ctx, "random %u bytes from %04x: %04x, MATH %04x", size, start,
              SVC_GetCRC16(start, buf, size), (u16)ctx);
    }
    {
        MATHCRC16Context ctx = 0xffff;
        MATHi_CRC16UpdateRev(&table, &ctx, check, 9);
        CHECK((u16)ctx == 0x4b37, "MATH check value %04x", (u16)ctx);
        // 8 bytes plus the ninth through the table: SVC over 8 then one more byte
        {
            u16 c8 = SVC_GetCRC16(0xffff, check, 8);
            MATHCRC16Context c = c8;
            MATHi_CRC16UpdateRev(&table, &c, check + 8, 1);
            CHECK((u16)c == 0x4b37, "CRC-16/MODBUS(123456789) %04x", (u16)c);
        }
    }

    // GetCRC16, the decomp's name, is in port/src/sdk_names.c (it needs the rest of libntr); see sdk_names_test.c

    if (rom != NULL) {
        u8 header[0x160];
        FILE *f = fopen(rom, "rb");
        CHECK(f != NULL, "open %s", rom);
        if (f != NULL) {
            CHECK(fread(header, 1, sizeof(header), f) == sizeof(header), "read the header");
            fclose(f);
            // 0x15c: the logo's CRC (0xc0-0x15b, always 0xcf56); 0x15e: the header's (0x000-0x15d)
            CHECK(SVC_GetCRC16(0xffff, header + 0xc0, 0x9c) == (header[0x15c] | header[0x15d] << 8), "logo CRC");
            CHECK(SVC_GetCRC16(0xffff, header, 0x15e) == (header[0x15e] | header[0x15f] << 8),
                  "header CRC %04x, ROM %04x", SVC_GetCRC16(0xffff, header, 0x15e), header[0x15e] | header[0x15f] << 8);
            printf("ROM header CRCs checked (%04x %04x)\n", header[0x15c] | header[0x15d] << 8,
                   header[0x15e] | header[0x15f] << 8);
        }
    }
}

static void TestCpuSet(void)
{
    u32 src[40], dst[48];
    u16 src16[40], dst16[48];
    u32 i;

    for (i = 0; i < 40; i++) {
        src[i] = Rand();
        src16[i] = (u16)Rand();
    }
    memset(dst, 0xee, sizeof(dst));
    SVC_CpuSet(src, dst, 13 | (1 << 26));
    CHECK(memcmp(dst, src, 13 * 4) == 0 && dst[13] == 0xeeeeeeee, "CpuSet copy32");
    memset(dst, 0xee, sizeof(dst));
    SVC_CpuSet(src, dst, 13 | (1 << 26) | (1 << 24));
    for (i = 0; i < 13; i++) {
        CHECK(dst[i] == src[0], "CpuSet fill32 %u", i);
    }
    CHECK(dst[13] == 0xeeeeeeee, "CpuSet fill32 end");
    memset(dst16, 0xee, sizeof(dst16));
    SVC_CpuSet(src16, dst16, 7);
    CHECK(memcmp(dst16, src16, 7 * 2) == 0 && dst16[7] == 0xeeee, "CpuSet copy16");
    memset(dst16, 0xee, sizeof(dst16));
    SVC_CpuSet(src16, dst16, 7 | (1 << 24));
    for (i = 0; i < 7; i++) {
        CHECK(dst16[i] == src16[0], "CpuSet fill16 %u", i);
    }
    CHECK(dst16[7] == 0xeeee, "CpuSet fill16 end");
    memset(dst, 0xee, sizeof(dst));
    SVC_CpuSetFast(src, dst, 16);
    CHECK(memcmp(dst, src, 16 * 4) == 0 && dst[16] == 0xeeeeeeee, "CpuSetFast copy");
    memset(dst, 0xee, sizeof(dst));
    SVC_CpuSetFast(src, dst, 9 | (1 << 24)); // rounded up to 16 words
    for (i = 0; i < 16; i++) {
        CHECK(dst[i] == src[0], "CpuSetFast fill %u", i);
    }
    CHECK(dst[16] == 0xeeeeeeee, "CpuSetFast fill end");
}

static void FillRand(fx32 *a, int n)
{
    int i;
    for (i = 0; i < n; i++) {
        a[i] = (fx32)Rand() - 0x400000;
    }
}

static void TestMtx(void)
{
    MtxFx22 m22;
    MtxFx33 m33, s33, t33;
    MtxFx43 m43, s43;
    MtxFx44 m44, s44;
    int i, j;

    memset(&m22, 0x55, sizeof(m22));
    MTX_Scale22(&m22, 3, -4);
    CHECK(m22._00 == 3 && m22._01 == 0 && m22._10 == 0 && m22._11 == -4, "Scale22");

    memset(&m33, 0x55, sizeof(m33));
    MTX_Scale33(&m33, 5, 6, 7);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            CHECK(m33.m[i][j] == (i != j ? 0 : 5 + i), "Scale33 %d %d", i, j);
        }
    }

    memset(&m44, 0x55, sizeof(m44));
    MTX_Scale44(&m44, 5, 6, 7);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            CHECK(m44.m[i][j] == (i != j ? 0 : i == 3 ? FX32_ONE : 5 + i), "Scale44 %d %d", i, j);
        }
    }

    FillRand(s33.a, 9);
    memset(&m43, 0x55, sizeof(m43));
    MTX_Copy33To43(&s33, &m43);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            CHECK(m43.m[i][j] == (i < 3 ? s33.m[i][j] : 0), "Copy33To43 %d %d", i, j);
        }
    }
    memset(&m44, 0x55, sizeof(m44));
    MTX_Copy33To44(&s33, &m44);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            CHECK(m44.m[i][j] == (i < 3 && j < 3 ? s33.m[i][j] : i == 3 && j == 3 ? FX32_ONE : 0), "Copy33To44 %d %d", i,
                  j);
        }
    }
    MTX_Transpose33(&s33, &t33);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            CHECK(t33.m[i][j] == s33.m[j][i], "Transpose33 %d %d", i, j);
        }
    }
    m33 = s33;
    MTX_Transpose33(&m33, &m33);
    CHECK(memcmp(&m33, &t33, sizeof(m33)) == 0, "Transpose33 in place");

    FillRand(s43.a, 12);
    memset(&m44, 0x55, sizeof(m44));
    MTX_Copy43To44(&s43, &m44);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            CHECK(m44.m[i][j] == (j < 3 ? s43.m[i][j] : i == 3 ? FX32_ONE : 0), "Copy43To44 %d %d", i, j);
        }
    }
    m43 = s43;
    MTX_Transpose43(&m43, &m43);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            CHECK(m43.m[i][j] == (i < 3 ? s43.m[j][i] : 0), "Transpose43 in place %d %d", i, j);
        }
    }

    FillRand(s44.a, 16);
    m44 = s44;
    MTX_Transpose44(&m44, &m44);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            CHECK(m44.m[i][j] == s44.m[j][i], "Transpose44 in place %d %d", i, j);
        }
    }
}

static void TestMem(void)
{
    u8 a[300], b[300], ref[300];
    u32 n;

    for (n = 0; n < 2000; n++) {
        u32 size = Rand() % 100, so = Rand() % 150, d = Rand() % 150, i;
        u32 value = Rand();
        for (i = 0; i < sizeof(a); i++) {
            a[i] = b[i] = ref[i] = (u8)Rand();
        }

        // memset of the low byte
        sys_memset_fast(a + d, value, size);
        for (i = 0; i < size; i++) {
            ref[d + i] = (u8)value;
        }
        CHECK(memcmp(a, ref, sizeof(a)) == 0, "sys_memset_fast %u at %u", size, d);

        // memmove: the reference copies through a buffer
        memcpy(a, b, sizeof(a));
        memcpy(ref, b, sizeof(ref));
        {
            u8 tmp[300];
            memcpy(tmp, b + so, size);
            memcpy(ref + d, tmp, size);
        }
        sys_memcpy_ex(a + so, a + d, size);
        CHECK(memcmp(a, ref, sizeof(a)) == 0, "sys_memcpy_ex %u from %u to %u", size, so, d);

        // forwards copy: disjoint, or the source above the destination (what sys_memcpy_ex leaves to it)
        if (so >= d || so + size <= d) {
            memcpy(a, b, sizeof(a));
            sys_memcpy_fast(a + so, a + d, size);
            CHECK(memcmp(a, ref, sizeof(a)) == 0, "sys_memcpy_fast %u from %u to %u", size, so, d);
        }

        // MIi_CpuClearFast: words of value, any size, nothing past the end
        memcpy(a, b, sizeof(a));
        memcpy(ref, b, sizeof(ref));
        d &= ~3u;
        for (i = 0; i < size; i++) {
            ref[d + i] = (u8)(value >> (8 * (i % 4)));
        }
        MIi_CpuClearFast(value, a + d, size);
        CHECK(memcmp(a, ref, sizeof(a)) == 0, "MIi_CpuClearFast %u at %u", size, d);
    }
    // libntr's inline over it
    {
        u32 w[10];
        memset(w, 0x11, sizeof(w));
        MI_CpuClearFast(w, 8 * 4);
        CHECK(w[0] == 0 && w[7] == 0 && w[8] == 0x11111111, "MI_CpuClearFast");
        // libntr's MI_CpuFillFast is a memset of data's low byte on PC (nitro/mi/memory.h), not words of data: right
        // only for a value of four equal bytes, which is what its callers here pass (0, NitroSystem's CharCanvas colours)
        MI_CpuFillFast(w, 0xabababab, 4 * 4);
        CHECK(w[0] == 0xabababab && w[3] == 0xabababab && w[4] == 0, "MI_CpuFillFast");
    }
}

// RFC 2202's HMAC-SHA1 test cases 1, 2 and 6 (a key longer than the block)
static void TestHmac(void)
{
    static const struct {
        int keyByte, keyLen;
        const char *key, *data, *digest;
    } cases[] = {
        {0x0b, 20, NULL, "Hi There", "b617318655057264e28bc0b6fb378c8ef146be00"},
        {0, 4, "Jefe", "what do ya want for nothing?", "effcdf6ae5eb2fa2d27416d5f184df9c259a7c79"},
        {0xaa, 80, NULL, "Test Using Larger Than Block-Size Key - Hash Key First",
         "aa4ae5e15272d00e95705637ce8a3b55ed402112"},
    };
    int c, i;

    for (c = 0; c < 3; c++) {
        u8 key[80], md[20], md2[20];
        char hex[41];
        if (cases[c].key != NULL) {
            memcpy(key, cases[c].key, cases[c].keyLen);
        } else {
            memset(key, cases[c].keyByte, cases[c].keyLen);
        }
        DGT_Hash2CalcHmac(md, (void *)cases[c].data, (int)strlen(cases[c].data), key, cases[c].keyLen);
        for (i = 0; i < 20; i++) {
            sprintf(hex + 2 * i, "%02x", md[i]);
        }
        CHECK(strcmp(hex, cases[c].digest) == 0, "HMAC-SHA1 case %d: %s", c, hex);
        SVC_CalcHMACSHA1(md2, cases[c].data, (u32)strlen(cases[c].data), key, (u32)cases[c].keyLen);
        CHECK(memcmp(md, md2, 20) == 0, "SVC_CalcHMACSHA1 case %d", c);
        // libntr's inline over it
        MATH_CalcHMACSHA1(md2, cases[c].data, (u32)strlen(cases[c].data), key, (u32)cases[c].keyLen);
        CHECK(memcmp(md, md2, 20) == 0, "MATH_CalcHMACSHA1 case %d", c);
    }
    CHECK(MATHi_SetOverlayTableMode(TRUE) == FALSE && MATHi_SetOverlayTableMode(FALSE) == TRUE, "overlay table mode");
}

int main(int argc, char **argv)
{
    TestCrc(argc > 1 ? argv[1] : NULL);
    TestCpuSet();
    TestMtx();
    TestMem();
    TestHmac();
    printf("sdk_test: %d checks, %d failed\n", sChecks, sFails);
    return sFails != 0;
}
