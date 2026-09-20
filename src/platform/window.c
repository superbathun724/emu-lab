#include "platform/window.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

/* 60Hz 한 프레임의 길이(ms). 16.666...ms를 정수로 다루면 오차가 쌓이므로
 * 누적 시각(next_frame_ms)을 실수로 들고 간다. */
#define FRAME_MS (1000.0 / 60.0)

/* 켜진 픽셀 / 꺼진 픽셀 색 (ARGB8888) */
#define COLOR_ON  0xFFFFFFFFu
#define COLOR_OFF 0xFF000000u

struct Window {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    SDL_Texture  *texture;      /* 64x32 원본 해상도. 확대는 렌더러가 한다. */
    double        next_frame_ms; /* 다음 프레임을 시작해야 할 시각 */
};

Window *window_create(const char *title)
{
    /*
     * SDL_MAIN_HANDLED로 빌드하므로 SDL이 main을 가로채지 않는다.
     * 대신 SDL_Init 전에 SDL_SetMainReady()로 "준비됐다"고 알려야 한다.
     */
    SDL_SetMainReady();

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "오류: SDL_Init 실패: %s\n", SDL_GetError());
        return NULL;
    }

    Window *win = calloc(1, sizeof(*win));
    if (win == NULL) {
        fprintf(stderr, "오류: Window 메모리를 잡을 수 없다\n");
        SDL_Quit();
        return NULL;
    }

    win->window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_GUEST_WIDTH * WINDOW_SCALE,
        WINDOW_GUEST_HEIGHT * WINDOW_SCALE,
        SDL_WINDOW_SHOWN);
    if (win->window == NULL) {
        fprintf(stderr, "오류: 창을 만들 수 없다: %s\n", SDL_GetError());
        window_destroy(win);
        return NULL;
    }

    win->renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_ACCELERATED);
    if (win->renderer == NULL) {
        fprintf(stderr, "오류: 렌더러를 만들 수 없다: %s\n", SDL_GetError());
        window_destroy(win);
        return NULL;
    }

    /*
     * 64x32 텍스처를 만들어 놓고 렌더러가 640x320으로 늘린다.
     * 픽셀을 640x320 버퍼에 직접 그리지 않는 이유: CHIP-8 쪽은 64x32만
     * 알면 되고, 확대 배율을 바꿔도 코어는 건드릴 필요가 없기 때문.
     * 스케일링은 nearest(기본값)라 픽셀이 뭉개지지 않고 각지게 커진다.
     */
    win->texture = SDL_CreateTexture(win->renderer,
                                     SDL_PIXELFORMAT_ARGB8888,
                                     SDL_TEXTUREACCESS_STREAMING,
                                     WINDOW_GUEST_WIDTH, WINDOW_GUEST_HEIGHT);
    if (win->texture == NULL) {
        fprintf(stderr, "오류: 텍스처를 만들 수 없다: %s\n", SDL_GetError());
        window_destroy(win);
        return NULL;
    }

    win->next_frame_ms = (double)SDL_GetTicks64();
    return win;
}

void window_destroy(Window *win)
{
    if (win == NULL) {
        return;
    }
    if (win->texture != NULL) {
        SDL_DestroyTexture(win->texture);
    }
    if (win->renderer != NULL) {
        SDL_DestroyRenderer(win->renderer);
    }
    if (win->window != NULL) {
        SDL_DestroyWindow(win->window);
    }
    free(win);
    SDL_Quit();
}

bool window_poll_events(Window *win)
{
    (void)win;   /* 아직 창 상태를 볼 일이 없다. Phase 1에서 키 상태가 들어온다. */

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
        case SDL_QUIT:
            printf("[종료] 창 닫기\n");
            return false;

        case SDL_KEYDOWN:
            /* repeat는 키를 누르고 있을 때 OS가 반복해서 보내는 이벤트.
             * 로그가 도배되지 않도록 처음 눌림만 찍는다. */
            if (ev.key.repeat) {
                break;
            }
            if (ev.key.keysym.sym == SDLK_ESCAPE) {
                printf("[종료] ESC\n");
                return false;
            }
            printf("[키 눌림] %s (SDL_Keycode=0x%X, scancode=%d)\n",
                   SDL_GetKeyName(ev.key.keysym.sym),
                   (unsigned)ev.key.keysym.sym,
                   (int)ev.key.keysym.scancode);
            fflush(stdout);
            break;

        case SDL_KEYUP:
            printf("[키 뗌]   %s\n", SDL_GetKeyName(ev.key.keysym.sym));
            fflush(stdout);
            break;

        default:
            break;
        }
    }
    return true;
}

void window_present(Window *win, const uint8_t *pixels)
{
    uint32_t *dst = NULL;
    int pitch = 0;   /* 한 줄의 바이트 수. 텍스처 내부 정렬 때문에 64*4와 다를 수 있다. */

    if (SDL_LockTexture(win->texture, NULL, (void **)&dst, &pitch) != 0) {
        fprintf(stderr, "오류: 텍스처를 잠글 수 없다: %s\n", SDL_GetError());
        return;
    }

    for (int y = 0; y < WINDOW_GUEST_HEIGHT; y++) {
        /* pitch는 바이트 단위라 uint32_t 개수로 바꿔서 줄 시작을 잡는다. */
        uint32_t *row = dst + (size_t)y * ((size_t)pitch / sizeof(uint32_t));
        for (int x = 0; x < WINDOW_GUEST_WIDTH; x++) {
            bool on = (pixels != NULL) &&
                      (pixels[(size_t)y * WINDOW_GUEST_WIDTH + (size_t)x] != 0);
            row[x] = on ? COLOR_ON : COLOR_OFF;
        }
    }

    SDL_UnlockTexture(win->texture);

    SDL_RenderClear(win->renderer);
    SDL_RenderCopy(win->renderer, win->texture, NULL, NULL);
    SDL_RenderPresent(win->renderer);
}

void window_wait_next_frame(Window *win)
{
    win->next_frame_ms += FRAME_MS;

    double now = (double)SDL_GetTicks64();
    if (now < win->next_frame_ms) {
        SDL_Delay((Uint32)(win->next_frame_ms - now));
    } else {
        /*
         * 이미 늦었다. 밀린 프레임을 따라잡겠다고 연속으로 돌리면
         * 더 나빠지므로 기준 시각을 현재로 되돌린다(프레임을 버린다).
         */
        win->next_frame_ms = now;
    }
}
