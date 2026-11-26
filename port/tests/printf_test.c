// Host test for libntr's OS_SNPrintf integer conversions (the DS's semantics: int is 32 bits, %hd/%hhd narrow, %lld is 64).
#include <nitro.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

static int fails;
#define CHECK(expect, ...) do { char b[96]; OS_SNPrintf(b, sizeof b, __VA_ARGS__); \
    if (strcmp(b, expect)) { printf("FAIL %s: got '%s' want '%s'\n", #__VA_ARGS__, b, expect); fails++; } } while (0)

int main(void) {
    int m5 = -5, five = 5, imin = INT_MIN;
    unsigned ffff = 0xFFFFFFFFu;
    CHECK("-5", "%d", m5);
    CHECK("5", "%d", five);
    CHECK("-2147483648", "%d", imin);
    CHECK("0", "%d", 0);
    CHECK("4294967291", "%u", (unsigned)m5);
    CHECK("4294967295", "%u", ffff);
    CHECK("2147483648", "%u", (unsigned)imin);
    CHECK("fffffffb", "%x", (unsigned)m5);
    CHECK("ffffffff", "%x", ffff);
    CHECK("FFFFFFFF", "%X", ffff);
    CHECK("80000000", "%x", (unsigned)imin);
    CHECK("-5", "%hd", m5);
    CHECK("-1", "%hd", 0xFFFF);
    CHECK("-32768", "%hd", 0x8000);
    CHECK("65535", "%hu", 0xFFFFFFFFu);
    CHECK("ffff", "%hx", ffff);
    CHECK("-1", "%hhd", 0xFF);
    CHECK("-128", "%hhd", 0x80);
    CHECK("ff", "%hhx", ffff);
    CHECK("255", "%hhu", 0x1FF);
    CHECK("-5", "%lld", (long long)m5);
    CHECK("-2147483648", "%lld", (long long)imin);
    CHECK("-9223372036854775808", "%lld", LLONG_MIN);
    CHECK("9223372036854775807", "%lld", LLONG_MAX);
    CHECK("18446744073709551615", "%llu", ULLONG_MAX);
    CHECK("ffffffffffffffff", "%llx", ULLONG_MAX);
    CHECK("4294967295", "%llu", (unsigned long long)ffff);
    CHECK("  -5", "%4d", m5);
    CHECK("-005", "%04d", m5);
    CHECK("-5 5 4294967295", "%d %d %u", m5, five, ffff);
    CHECK("-5", "%ld", (long)m5);
    CHECK("4294967295", "%lu", (unsigned long)ffff);
    printf(fails ? "printf_test: %d FAILED\n" : "printf_test: all passed\n", fails);
    return fails != 0;
}
