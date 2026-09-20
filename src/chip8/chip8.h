#ifndef EMU_CHIP8_H
#define EMU_CHIP8_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 * CHIP-8 코어.
 *
 * 이 파일과 chip8.c는 사용자가 직접 설계·구현한다.
 * 여기 있는 것은 자리만 잡아 둔 뼈대이고, 구조체 필드와 함수 본문은 비어 있다.
 *
 * 규칙: 이 코어는 SDL에 의존하지 않는다. <SDL.h>를 포함하지 말 것.
 *       화면·키 입력은 값(배열, 플래그)으로만 주고받고,
 *       실제 창을 그리는 일은 src/platform/이 한다.
 */

/* 화면 크기. 플랫폼 코드와 같은 값을 써야 하지만, 서로 포함하지 않으려고
 * 양쪽에 따로 정의해 둔다(platform/window.h의 WINDOW_GUEST_*). */
#define CHIP8_SCREEN_WIDTH  64
#define CHIP8_SCREEN_HEIGHT 32

typedef struct Chip8 {
    /*
     * TODO: 여기에 필요한 필드를 직접 설계해서 채운다.
     *
     * 힌트 — CHIP-8 한 대가 기억하고 있어야 하는 것들:
     *
     *   메모리     4KB. 0x000~0x1FF는 원래 인터프리터가 쓰던 영역이고,
     *              프로그램은 0x200부터 올라간다. 폰트 데이터는 보통
     *              0x050~0x09F에 둔다.
     *   레지스터   범용 레지스터 16개(V0~VF). VF는 캐리·충돌 플래그로도 쓰여서
     *              일반 값 저장용으로 믿으면 안 된다.
     *   주소 레지스터  I. 메모리 주소를 담는다. 12비트만 쓰지만 담는 그릇은?
     *   PC         지금 실행할 명령어의 주소. 명령어 하나가 몇 바이트인지 생각할 것.
     *   스택       서브루틴 호출(2NNN)에서 돌아올 주소를 쌓는다.
     *              몇 칸이 필요한지, 지금 몇 칸 찼는지도 알아야 한다.
     *   타이머     딜레이 타이머와 사운드 타이머. 둘 다 60Hz로 줄어든다.
     *   화면       64x32. 픽셀 하나에 무엇을 쓸지(1비트? 1바이트?)는 선택 사항이다.
     *              바이트 배열이 다루기 쉽고, 비트 배열이 메모리를 아낀다.
     *   키 상태    16개 키가 눌렸는지 여부.
     *
     * 그 밖에 구현하다 보면 필요해지는 것들(스펙에는 없지만 있으면 편한 것):
     *   - 화면이 바뀌었는지 표시하는 플래그 (매 프레임 다시 그릴지 판단용)
     *   - FX0A(키를 기다리는 명령어)처럼 실행을 멈춰야 하는 상태
     *
     * 각 필드의 크기는 고정폭 정수로 쓴다(uint8_t / uint16_t).
     * "이 값이 몇 비트까지 올라가는가"를 매번 스펙에서 확인하고 고를 것.
     *
     * 참고: docs/references.md → Cowgod's Technical Reference 2.0~2.5절,
     *       Tobias V. Langhoff 가이드의 "Memory / Registers" 부분.
     */
    int _placeholder;   /* TODO: 첫 필드를 추가하면 이 줄은 지운다.
                         * C에서는 멤버가 하나도 없는 구조체를 쓸 수 없어서 둔 자리표다. */
} Chip8;

/*
 * 아래 함수들도 시그니처만 잡아 둔 것이다. 구현하면서 필요에 맞게
 * 이름·인자·반환형을 바꿔도 된다. 정답이 아니라 출발점이다.
 */

/* 전원을 켠 직후 상태로 초기화한다. (메모리·레지스터 비우기, PC 설정, 폰트 적재) */
void chip8_init(Chip8 *c);

/* ROM 바이트를 메모리에 올린다. 성공 0, 실패 0이 아닌 값. */
int chip8_load_rom(Chip8 *c, const uint8_t *data, size_t size);

/* 명령어 하나를 fetch → decode → execute 한다. */
void chip8_step(Chip8 *c);

/* 딜레이·사운드 타이머를 1씩 줄인다. 60Hz로 한 번씩 불러야 한다. */
void chip8_tick_timers(Chip8 *c);

/*
 * 화면 버퍼를 돌려준다. 픽셀당 1바이트, 0이면 꺼짐.
 * platform 쪽이 이 포인터를 그대로 window_present()에 넘긴다.
 * (화면을 비트 단위로 저장하기로 했다면 여기서 풀어 주거나,
 *  이 함수의 형태 자체를 바꾸면 된다.)
 */
const uint8_t *chip8_screen(const Chip8 *c);

/* 키 하나의 눌림 상태를 알린다. key는 0x0~0xF. */
void chip8_set_key(Chip8 *c, uint8_t key, bool pressed);

#endif /* EMU_CHIP8_H */
