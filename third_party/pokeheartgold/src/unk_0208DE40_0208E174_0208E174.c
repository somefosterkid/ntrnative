#include "global.h"
#include "pokemon_summary_app.h"

s32 sub_0208DF9C(s32 *, void *, u8);

s32 sub_0208DFF8(void *, void *, void *);

extern s32 _02104EB0;

extern s32 _02104EC2;

extern s32 _02104ED4;

extern s32 _02104EE6;

extern s32 _02104EF8;

extern s32 _02104F0A;

extern s32 _02104F1C;

extern s32 _02104F2E;

extern s32 _02104F40;

extern s32 _02104F52;

extern s32 _02104F64;

extern s32 _02104F76;

extern s32 _02104F88;

extern s32 _02104F9A;

extern s32 _02104FAC;

extern s32 _02104FBE;

void sub_0208E174(PokemonSummary *arg0);

void sub_0208E174(PokemonSummary *arg0) {
    u32 sp24;
    void *sp20;
    void *sp1C;
    void *sp18;
    void *sp14;
    void *sp10;
    void *spC;
    void *sp8;
    void *sp4;
    void *sp0;
    u32 temp_r0;
    void *var_r7;
    void *var_r6;
    void *var_r4;
    sub_0208DF9C(&_02104EB0, arg0->conditionChartTo, arg0->mon.contestStats[0]);
    sub_0208DF9C(&_02104EC2, &arg0->conditionChartTo[0].vertices[1], arg0->mon.contestStats[1]);
    sub_0208DF9C(&_02104ED4, &arg0->conditionChartTo[0].vertices[3], arg0->mon.contestStats[2]);
    sub_0208DF9C(&_02104EE6, &arg0->conditionChartTo[0].vertices[2], 0U);
    sub_0208DF9C(&_02104EF8, &arg0->conditionChartTo[1], arg0->mon.contestStats[4]);
    sub_0208DF9C(&_02104F0A, &arg0->conditionChartTo[1].vertices[1], arg0->mon.contestStats[0]);
    sub_0208DF9C(&_02104F1C, &arg0->conditionChartTo[1].vertices[3], 0U);
    sub_0208DF9C(&_02104F2E, &arg0->conditionChartTo[1].vertices[2], arg0->mon.contestStats[3]);
    sub_0208DF9C(&_02104F40, &arg0->conditionChartTo[2], arg0->mon.contestStats[4]);
    sub_0208DF9C(&_02104F52, &arg0->conditionChartTo[2].vertices[1], 0U);
    sub_0208DF9C(&_02104F64, &arg0->conditionChartTo[2].vertices[3], arg0->mon.contestStats[2]);
    sub_0208DF9C(&_02104F76, &arg0->conditionChartTo[2].vertices[2], arg0->mon.contestStats[3]);
    sub_0208DF9C(&_02104F88, &arg0->conditionChartTo[3], 0U);
    sub_0208DF9C(&_02104F9A, &arg0->conditionChartTo[3].vertices[1], arg0->mon.contestStats[1]);
    sub_0208DF9C(&_02104FAC, &arg0->conditionChartTo[3].vertices[3], arg0->mon.contestStats[2]);
    sub_0208DF9C(&_02104FBE, &arg0->conditionChartTo[3].vertices[2], arg0->mon.contestStats[3]);
    sp24 = 0;
    sp20 = arg0->conditionChartStep;
    sp1C = arg0->conditionChartTo;
    sp18 = arg0->conditionChartFrom;
    sp14 = &arg0->conditionChartStep[0].vertices[1];
    sp10 = &arg0->conditionChartTo[0].vertices[1];
    spC = &arg0->conditionChartFrom[0].vertices[1];
    sp8 = &arg0->conditionChartStep[0].vertices[2];
    sp4 = &arg0->conditionChartTo[0].vertices[2];
    sp0 = &arg0->conditionChartFrom[0].vertices[2];
    var_r7 = &arg0->conditionChartStep[0].vertices[3];
    var_r6 = &arg0->conditionChartTo[0].vertices[3];
    var_r4 = &arg0->conditionChartFrom[0].vertices[3];
    do {
        sub_0208DFF8(sp18, sp1C, sp20);
        sub_0208DFF8(spC, sp10, sp14);
        sub_0208DFF8(sp0, sp4, sp8);
        sub_0208DFF8(var_r4, var_r6, var_r7);
        sp20 += 0x18;
        sp1C += 0x18;
        var_r7 += 0x18;
        var_r6 += 0x18;
        var_r4 += 0x18;
        sp18 += 0x18;
        sp14 += 0x18;
        sp10 += 0x18;
        spC += 0x18;
        sp8 += 0x18;
        sp4 += 0x18;
        sp0 += 0x18;
        temp_r0 = sp24 + 1;
        sp24 = temp_r0;
    } while (temp_r0 < 4U);
    *((s32 *)&arg0->conditionChartDone) = 0;
}
