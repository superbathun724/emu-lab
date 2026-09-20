#include "chip8/chip8.h"

/*
 * CHIP-8 코어 구현 — 여기부터는 사용자 담당이다.
 * 지금은 빌드만 통과하도록 빈 본문만 남겨 두었다.
 *
 * 인자에 붙은 (void)c; 는 "아직 안 쓴다"고 컴파일러에게 알리는 표시다.
 * -Wextra가 켜져 있어서 이게 없으면 "사용하지 않는 인자" 경고가 난다.
 * 실제로 쓰기 시작하면 지운다.
 */

void chip8_init(Chip8 *c)
{
    (void)c;
    /* TODO: 메모리·레지스터·스택·타이머·화면을 0으로 지운다.
     *       PC를 프로그램 시작 주소로 맞춘다 (ROM이 올라가는 곳과 같아야 한다).
     *       폰트 데이터(0~F, 각 5바이트 = 80바이트)를 메모리 앞쪽에 넣는다.
     *       폰트 바이트 값은 Cowgod 2.4절 / Langhoff 가이드의 "Font" 표에 있다. */
}

int chip8_load_rom(Chip8 *c, const uint8_t *data, size_t size)
{
    (void)c;
    (void)data;
    (void)size;
    /* TODO: ROM 크기 검사 필요.
     *       "파일을 열 수 있는가"는 platform/rom.c가 이미 확인했다.
     *       여기서 볼 것은 CHIP-8 규칙 쪽이다 — 4KB 메모리에서 프로그램 시작
     *       주소부터 끝까지 남는 공간에 이 ROM이 들어가는가?
     *       들어가지 않으면 메모리 밖을 덮어쓰게 되므로 0이 아닌 값으로 거절한다.
     * TODO: 검사를 통과하면 data를 메모리의 프로그램 시작 주소부터 복사한다. */
    return 0;
}

void chip8_step(Chip8 *c)
{
    (void)c;
    /* TODO: fetch  — PC가 가리키는 곳에서 명령어를 읽는다.
     *                CHIP-8 명령어는 2바이트이고 빅엔디안이다.
     *                (앞 바이트가 상위 8비트. 두 바이트를 어떻게 합칠지 생각할 것)
     *                읽은 뒤 PC를 다음 명령어로 옮긴다 — 실행하기 "전에" 옮기는 게
     *                편하다. 점프 명령어가 PC를 덮어써야 하기 때문.
     * TODO: decode — 상위 4비트로 큰 갈래를 나누고, 나머지 니블로 세부를 가른다.
     *                자주 쓰는 조각: X, Y, N, NN, NNN (Cowgod 3.1절 표기)
     * TODO: execute— 갈래별 동작. 처음에는 로드맵의 6개만:
     *                00E0, 1NNN, 6XNN, 7XNN, ANNN, DXYN → IBM 로고가 뜬다. */
}

void chip8_tick_timers(Chip8 *c)
{
    (void)c;
    /* TODO: 딜레이 타이머와 사운드 타이머가 0보다 크면 1씩 줄인다.
     *       0 아래로 내려가지 않게 할 것. */
}

const uint8_t *chip8_screen(const Chip8 *c)
{
    (void)c;
    /* TODO: 화면 버퍼의 시작 주소를 돌려준다.
     *       지금은 NULL이라 창이 검게만 나온다(platform 쪽이 NULL을 검정으로 처리한다). */
    return NULL;
}

void chip8_set_key(Chip8 *c, uint8_t key, bool pressed)
{
    (void)c;
    (void)key;
    (void)pressed;
    /* TODO: 키 상태 배열의 해당 칸을 갱신한다.
     *       key가 0x0~0xF 범위인지 확인할 것. */
}
