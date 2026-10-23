/*
 * miasma_sprite.h — Dr. Ashgrave sprite and MIASMA UI palette
 * MIASMA firmware for the M-VAVE FM-1. GPL-3.0-only.
 *
 * Sprite: 16 x 26 pixels, one palette index per pixel, row-major.
 * Index 0 is transparent. Colours are RGB565 (check the panel's byte
 * order in Skiff's LCD driver; swap bytes if the panel wants BGR or
 * big-endian words).
 */
#ifndef MIASMA_SPRITE_H
#define MIASMA_SPRITE_H

#include <stdint.h>

/* ---- UI palette (RGB565) ------------------------------------------ */
#define MIASMA_C_VOID      0x0021u  /* #07060B background            */
#define MIASMA_C_PANEL     0x18C4u  /* #1C1826 selected-row fill      */
#define MIASMA_C_RULE      0x39A8u  /* #3A3546 empty steps, frames    */
#define MIASMA_C_COAT      0x2926u  /* #2A2436 hat and coat           */
#define MIASMA_C_SHADOW    0x4A0Bu  /* #4A4258 coat folds             */
#define MIASMA_C_DIM       0x8C32u  /* #8A8496 secondary text, cane   */
#define MIASMA_C_BONE      0xEF3Bu  /* #ECE6D8 text, frames, mask     */
#define MIASMA_C_EMBER     0xE2C7u  /* #E0583A accent, hat band       */
#define MIASMA_C_SPECTRAL  0x6E98u  /* #6FD3C7 cursor, lens, engines  */
#define MIASMA_C_BRASS     0xCD09u  /* #C9A24A cane knob              */

/* ---- Sprite palette: index -> colour ------------------------------ */
enum {
    ASH_T = 0, /* transparent   */
    ASH_K,     /* outline, bone */
    ASH_H,     /* hat / coat    */
    ASH_B,     /* hat band      */
    ASH_E,     /* lens glow     */
    ASH_W,     /* beak mask     */
    ASH_N,     /* cane knob     */
    ASH_S,     /* cane shaft    */
    ASH_D,     /* coat fold     */
    ASH_PAL_COUNT
};

static const uint16_t ash_palette[ASH_PAL_COUNT] = {
    0x0000u,            /* ASH_T, unused */
    MIASMA_C_BONE,      /* ASH_K */
    MIASMA_C_COAT,      /* ASH_H */
    MIASMA_C_EMBER,     /* ASH_B: swap for the user accent colour */
    MIASMA_C_SPECTRAL,  /* ASH_E */
    MIASMA_C_BONE,      /* ASH_W */
    MIASMA_C_BRASS,     /* ASH_N */
    MIASMA_C_DIM,       /* ASH_S */
    MIASMA_C_SHADOW,    /* ASH_D */
};

#define ASH_W_PX 16
#define ASH_H_PX 26
/* Head-only portrait: rows 0..12, cols 1..14 */
#define ASH_PORTRAIT_X0 1
#define ASH_PORTRAIT_W  14
#define ASH_PORTRAIT_H  13

/*
 * Source map (keep in sync with ash_pixels below):
 *  ....KKKKKKK.....   hat top
 *  ....KHHHHHK.....
 *  ....KHHHHHK.....
 *  ....KHHHHHK.....
 *  ....KHHHHHK.....
 *  ....KBBBBBK.....   hat band
 *  ..KKKKKKKKKKK...   brim
 *  ...KCCCCCCK.....
 *  ...KCEECCCWW....   lens + beak
 *  ...KCEECCWWWW...
 *  ...KCCCCCWWWWW..
 *  ...KCCCCCCWWW...
 *  ...KKCCCCCKW....
 *  ..KCCCCCCCCK..N.   cane knob
 *  .KCCCCCCCCCCKKNN   hand on cane
 *  .KCCCCCCCCCCKSN.
 *  .KCCDCCCCCCCK.S.
 *  .KCCDCCCCCCCK.S.
 *  .KCCCCCCCCCCK.S.
 *  KCCCCDCCCCCCCKS.
 *  KCCCCDCCCCCCCKS.
 *  KCCCCCCCCCCCCKS.
 *  KCCCCCCCCCCCCKS.
 *  KKKKKKKKKKKKKKS.   coat hem
 *  ...KC...KC....S.   legs
 *  ..KKK..KKK....S.   boots
 * (C in the map = ASH_H)
 */
#define T ASH_T
#define K ASH_K
#define H ASH_H
#define B ASH_B
#define E ASH_E
#define W ASH_W
#define N ASH_N
#define S ASH_S
#define D ASH_D
static const uint8_t ash_pixels[ASH_H_PX][ASH_W_PX] = {
    {T,T,T,T,K,K,K,K,K,K,K,T,T,T,T,T},
    {T,T,T,T,K,H,H,H,H,H,K,T,T,T,T,T},
    {T,T,T,T,K,H,H,H,H,H,K,T,T,T,T,T},
    {T,T,T,T,K,H,H,H,H,H,K,T,T,T,T,T},
    {T,T,T,T,K,H,H,H,H,H,K,T,T,T,T,T},
    {T,T,T,T,K,B,B,B,B,B,K,T,T,T,T,T},
    {T,T,K,K,K,K,K,K,K,K,K,K,K,T,T,T},
    {T,T,T,K,H,H,H,H,H,H,K,T,T,T,T,T},
    {T,T,T,K,H,E,E,H,H,H,W,W,T,T,T,T},
    {T,T,T,K,H,E,E,H,H,W,W,W,W,T,T,T},
    {T,T,T,K,H,H,H,H,H,W,W,W,W,W,T,T},
    {T,T,T,K,H,H,H,H,H,H,W,W,W,T,T,T},
    {T,T,T,K,K,H,H,H,H,H,K,W,T,T,T,T},
    {T,T,K,H,H,H,H,H,H,H,H,K,T,T,N,T},
    {T,K,H,H,H,H,H,H,H,H,H,H,K,K,N,N},
    {T,K,H,H,H,H,H,H,H,H,H,H,K,S,N,T},
    {T,K,H,H,D,H,H,H,H,H,H,H,K,T,S,T},
    {T,K,H,H,D,H,H,H,H,H,H,H,K,T,S,T},
    {T,K,H,H,H,H,H,H,H,H,H,H,K,T,S,T},
    {K,H,H,H,H,D,H,H,H,H,H,H,H,K,S,T},
    {K,H,H,H,H,D,H,H,H,H,H,H,H,K,S,T},
    {K,H,H,H,H,H,H,H,H,H,H,H,H,K,S,T},
    {K,H,H,H,H,H,H,H,H,H,H,H,H,K,S,T},
    {K,K,K,K,K,K,K,K,K,K,K,K,K,K,S,T},
    {T,T,T,K,H,T,T,T,K,H,T,T,T,T,S,T},
    {T,T,K,K,K,T,T,K,K,K,T,T,T,T,S,T},
};
#undef T
#undef K
#undef H
#undef B
#undef E
#undef W
#undef N
#undef S
#undef D

#endif /* MIASMA_SPRITE_H */
