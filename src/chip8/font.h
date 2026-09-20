#ifndef EMU_CHIP8_FONT_H
#define EMU_CHIP8_FONT_H

#include <stdint.h>

/*
 * CHIP-8 기본 폰트 (16진 숫자 0~F).
 *
 * 글자 하나가 5바이트, 16글자라 전부 80바이트다.
 * 각 바이트가 한 줄이고, 스프라이트 폭은 4픽셀이라 상위 4비트만 쓴다.
 * (그래서 모든 바이트의 하위 4비트가 0이다. 눈으로 확인할 수 있는 검사다.)
 *
 * 출처: docs/references.md → Tobias V. Langhoff 가이드의 "Font" 절.
 *       Cowgod's Technical Reference 2.4절도 같은 스프라이트를 설명한다.
 *
 * 이 헤더는 chip8.h를 포함하지 않는다. 폰트 표는 그 자체로 독립적이라
 * 나중에 디스어셈블러나 테스트가 코어 없이 폰트만 가져다 쓸 수 있다.
 *
 * 이 배열을 메모리 어느 주소에 넣을지, 넣는 코드를 어떻게 쓸지는
 * chip8_init()에서 직접 정한다. 관례적인 위치는 스펙 문서에 적혀 있다.
 */

#define CHIP8_FONT_CHAR_COUNT 16                 /* 0~F */
#define CHIP8_FONT_CHAR_BYTES 5                  /* 글자 하나당 5줄 */
#define CHIP8_FONT_SIZE (CHIP8_FONT_CHAR_COUNT * CHIP8_FONT_CHAR_BYTES)  /* 80 */

static const uint8_t chip8_font[CHIP8_FONT_SIZE] = {
    /*
     * '0'만 예시로 풀어서 그려 둔다. 바이트의 상위 비트가 왼쪽 픽셀이다.
     * (DXYN에서 스프라이트를 그릴 때 0x80부터 오른쪽으로 밀며 검사하는 이유)
     *
     *   0xF0 = 1111 0000 = ####
     *   0x90 = 1001 0000 = #..#
     *   0x90 = 1001 0000 = #..#
     *   0x90 = 1001 0000 = #..#
     *   0xF0 = 1111 0000 = ####
     *
     * 나머지 글자도 같은 방식으로 풀어서 확인해 볼 것.
     */
    0xF0, 0x90, 0x90, 0x90, 0xF0,   /* 0 */
    0x20, 0x60, 0x20, 0x20, 0x70,   /* 1 */
    0xF0, 0x10, 0xF0, 0x80, 0xF0,   /* 2 */
    0xF0, 0x10, 0xF0, 0x10, 0xF0,   /* 3 */
    0x90, 0x90, 0xF0, 0x10, 0x10,   /* 4 */
    0xF0, 0x80, 0xF0, 0x10, 0xF0,   /* 5 */
    0xF0, 0x80, 0xF0, 0x90, 0xF0,   /* 6 */
    0xF0, 0x10, 0x20, 0x40, 0x40,   /* 7 */
    0xF0, 0x90, 0xF0, 0x90, 0xF0,   /* 8 */
    0xF0, 0x90, 0xF0, 0x10, 0xF0,   /* 9 */
    0xF0, 0x90, 0xF0, 0x90, 0x90,   /* A */
    0xE0, 0x90, 0xE0, 0x90, 0xE0,   /* B */
    0xF0, 0x80, 0x80, 0x80, 0xF0,   /* C */
    0xE0, 0x90, 0x90, 0x90, 0xE0,   /* D */
    0xF0, 0x80, 0xF0, 0x80, 0xF0,   /* E */
    0xF0, 0x80, 0xF0, 0x80, 0x80,   /* F */
};

#endif /* EMU_CHIP8_FONT_H */
