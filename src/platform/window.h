#ifndef EMU_PLATFORM_WINDOW_H
#define EMU_PLATFORM_WINDOW_H

#include <stdbool.h>
#include <stdint.h>

/*
 * SDL2 창·입력·메인 루프.
 * 여기서만 SDL에 의존한다. 코어(src/chip8/)는 이 헤더를 포함하지 않는다.
 */

/* CHIP-8 화면 크기와 확대 배율 */
#define WINDOW_GUEST_WIDTH  64
#define WINDOW_GUEST_HEIGHT 32
#define WINDOW_SCALE        10

typedef struct Window Window;

/*
 * 창을 만들고 64x32 텍스처를 준비한다.
 * 성공하면 Window*, 실패하면 NULL(이유는 stderr에).
 */
Window *window_create(const char *title);

/* window_create로 잡은 자원을 모두 푼다. NULL을 넘겨도 안전하다. */
void window_destroy(Window *win);

/*
 * 쌓인 SDL 이벤트를 전부 처리한다.
 * 창 닫기나 ESC를 받으면 false를 돌려준다(= 종료해야 한다).
 * 지금은 눌린 키 이름을 콘솔에 찍기만 한다. CHIP-8 키패드 매핑은 Phase 1에서.
 */
bool window_poll_events(Window *win);

/*
 * 64x32 픽셀 버퍼를 화면에 확대해서 그린다.
 * pixels는 픽셀 하나당 1바이트, 0이면 꺼짐, 0이 아니면 켜짐.
 * pixels가 NULL이면 화면을 전부 끈 상태(검정)로 그린다.
 */
void window_present(Window *win, const uint8_t *pixels);

/*
 * 약 60Hz가 되도록 이번 프레임의 남은 시간만큼 잠든다.
 * 프레임 시작 시각은 window_present/window_poll_events와 무관하게
 * 이 함수가 내부에서 관리한다.
 */
void window_wait_next_frame(Window *win);

#endif /* EMU_PLATFORM_WINDOW_H */
