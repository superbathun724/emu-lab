#ifndef EMU_PLATFORM_ROM_H
#define EMU_PLATFORM_ROM_H

#include <stddef.h>
#include <stdint.h>

/*
 * ROM 파일 읽기 + hex dump.
 *
 * 여기는 "파일"만 다룬다. 파일을 열 수 있는지, 비어 있지 않은지 같은
 * 파일 수준의 검사만 한다. ROM이 CHIP-8 메모리에 들어갈 크기인지 같은
 * 에뮬레이터 규칙 검사는 코어(src/chip8/)의 책임이다.
 */

/* 읽어 들인 ROM 하나. data는 malloc으로 잡히므로 rom_free()로 풀어야 한다. */
typedef struct {
    uint8_t *data;
    size_t   size;
} Rom;

/*
 * path의 파일을 바이너리 모드로 통째로 읽는다.
 * 성공하면 0, 실패하면 0이 아닌 값을 돌려주고 stderr에 이유를 찍는다.
 * 실패했을 때 out은 건드리지 않는다.
 */
int rom_load(const char *path, Rom *out);

/* rom_load로 잡은 메모리를 푼다. 같은 Rom에 두 번 불러도 안전하다. */
void rom_free(Rom *rom);

/*
 * xxd 형식으로 stdout에 덤프한다.
 * 한 줄에 16바이트씩, "주소: 16진수 ... |ASCII|" 꼴.
 * base_addr는 첫 바이트에 붙일 주소. CHIP-8 ROM은 0x200에 올라가므로
 * 그 주소로 보고 싶으면 0x200을 넘기면 된다.
 */
void rom_hexdump(const uint8_t *data, size_t size, uint16_t base_addr);

#endif /* EMU_PLATFORM_ROM_H */
