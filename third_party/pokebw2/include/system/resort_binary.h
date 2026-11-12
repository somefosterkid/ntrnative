#ifndef POKEBW2_SYSTEM_RESORT_BINARY_H
#define POKEBW2_SYSTEM_RESORT_BINARY_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The Join Avenue's tables (resort_binary.c): files of archive ARCID_RESORT_BINARY holding rows of u16 columns, and
// the pair of them that describes the shops and what they sell.
// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0): join_ave_raffle_shop. The rest are ours

// The prizes of a raffle row: pairs of {weight, prize} from column 2
#define RESORT_RAFFLE_PRIZE_COUNT 10
// The goods a shop sells: the rows of the goods table that its columns 9 to 24 give
#define RESORT_SHOP_GOODS_MAX 16

struct ResortBinary {
    u32 columns;
    // In bytes
    u32 size;
    u16 *data;
};

struct ResortShopData {
    // File 1, 25 columns a row: column 0 is the shop's kind, then its map patches and the goods it sells
    ResortBinary *shops;
    // File 2, 10 columns a row
    ResortBinary *goods;
};

ResortBinary *ResortBinary_Load(u32 fileId, u32 columns, HeapID heapId);
void ResortBinary_Free(ResortBinary *table);
u16 ResortBinary_Get(ResortBinary *table, u32 row, u32 column);
u32 ResortBinary_GetRowCount(ResortBinary *table);
// Column 3 of a row that ResortBinary_FindRow returns
u16 ResortBinary_GetFoundValue(const u16 *row);
// The row whose columns 0 to 2 are key0 to key2, or NULL
const u16 *ResortBinary_FindRow(ResortBinary *table, u32 key0, u32 key1, u16 key2);
// The first row whose columns 0 and 1 hold the value between them, or 0
u32 ResortBinary_FindRange(ResortBinary *table, u32 value);
// Column 1 of the last row: the end of the last range
u16 ResortBinary_GetLastRangeEnd(ResortBinary *table);

ResortShopData *ResortShopData_Load(HeapID heapId);
void ResortShopData_Free(ResortShopData *data);
// The first shop from row 1 whose columns 0 to 2 are key0, key1 and key2, or 0
u32 ResortShopData_FindShop(ResortShopData *data, u16 key0, u16 key2, u16 key1);
// The shop a person runs
const u16 *ResortShopData_GetPersonShop(ResortShopData *data, JoinAvenuePerson *person);
const u16 *ResortShopData_GetShop(ResortShopData *data, u32 id);
u16 ResortShopData_GetShopParam(const u16 *shop, u32 column);
// The goods of a shop, or NULL past its last
const u16 *ResortShopData_GetGoods(ResortShopData *data, const u16 *shop, u16 index);
u32 ResortShopData_GetGoodsCount(ResortShopData *data, const u16 *shop);
u16 ResortShopData_GetGoodsParam(const u16 *goods, u32 column);
// The prize a raffle draw of value wins: the first whose weights summed pass it, or RESORT_RAFFLE_PRIZE_COUNT
u16 join_ave_raffle_shop(ResortBinary *table, u32 row, u32 value);

#endif // POKEBW2_SYSTEM_RESORT_BINARY_H
