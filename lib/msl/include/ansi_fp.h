// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/ansi_fp.h
#ifndef _C_ANSI_FP_H
#define _C_ANSI_FP_H

#ifdef __cplusplus
extern "C" {
#endif

#define FLOATDECIMAL ((char) (0))
#define FIXEDDECIMAL ((char) (1))

typedef struct decimal {
    unsigned char sgn;
    char unused;
    short exp;
    struct {
        unsigned char length;
        unsigned char text[32];
        unsigned char unused;
    } sig;
} decimal;

typedef struct decform {
    char style;
    char unused;
    short digits;
} decform;

void __num2dec(const decform *f, double x, decimal *d);
double __dec2num(const decimal *d);

#ifdef __cplusplus
}
#endif

#endif
