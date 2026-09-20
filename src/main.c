#include <stdio.h>
#include <string.h>

#include "chip8/chip8.h"
#include "platform/rom.h"
#include "platform/window.h"

/* CHIP-8 프로그램이 올라가는 주소. hex dump를 이 주소 기준으로 찍는다. */
#define CHIP8_PROGRAM_START 0x200

static void print_usage(const char *argv0)
{
    fprintf(stderr,
        "사용법: %s <ROM 경로> [--dump]\n"
        "\n"
        "  <ROM 경로>   실행할 CHIP-8 ROM 파일\n"
        "  --dump       ROM을 16바이트씩 hex dump로 출력하고 끝낸다(창을 띄우지 않음)\n"
        "\n"
        "예) %s roms/ibm-logo.ch8 --dump\n",
        argv0, argv0);
}

int main(int argc, char **argv)
{
    const char *rom_path = NULL;
    bool dump_only = false;

    /* 경로는 하드코딩하지 않고 명령줄 인자로만 받는다. */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--dump") == 0) {
            dump_only = true;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "오류: 모르는 옵션: %s\n\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        } else if (rom_path == NULL) {
            rom_path = argv[i];
        } else {
            fprintf(stderr, "오류: ROM 경로가 두 개 이상이다: %s\n\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    if (rom_path == NULL) {
        print_usage(argv[0]);
        return 1;
    }

    Rom rom;
    if (rom_load(rom_path, &rom) != 0) {
        return 1;   /* 실패 이유는 rom_load가 이미 찍었다 */
    }
    printf("ROM: %s (%zu 바이트)\n", rom_path, rom.size);

    if (dump_only) {
        rom_hexdump(rom.data, rom.size, CHIP8_PROGRAM_START);
        rom_free(&rom);
        return 0;
    }

    Chip8 chip8;
    chip8_init(&chip8);
    if (chip8_load_rom(&chip8, rom.data, rom.size) != 0) {
        fprintf(stderr, "오류: ROM을 CHIP-8 메모리에 올리지 못했다\n");
        rom_free(&rom);
        return 1;
    }
    /* 코어가 메모리에 복사해 갔으므로 파일 버퍼는 더 필요 없다. */
    rom_free(&rom);

    Window *win = window_create("emu-lab — CHIP-8");
    if (win == NULL) {
        return 1;
    }

    printf("창을 띄웠다. 키를 누르면 콘솔에 찍힌다. ESC 또는 창 닫기로 종료.\n");
    fflush(stdout);

    /*
     * 메인 루프. 지금은 한 프레임에 "이벤트 처리 → 그리기 → 대기"만 한다.
     * Phase 1에서 여기에 두 가지가 들어온다:
     *   - 프레임당 명령어 N개 실행 (chip8_step)
     *   - 프레임당 타이머 1회 감소 (chip8_tick_timers)
     */
    while (window_poll_events(win)) {
        window_present(win, chip8_screen(&chip8));
        window_wait_next_frame(win);
    }

    window_destroy(win);
    printf("종료.\n");
    return 0;
}
