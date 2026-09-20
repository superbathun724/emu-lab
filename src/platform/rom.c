#include "platform/rom.h"

#include <stdio.h>
#include <stdlib.h>

/* 한 줄에 찍을 바이트 수 (xxd 기본값과 같다) */
#define BYTES_PER_LINE 16

int rom_load(const char *path, Rom *out)
{
    /*
     * 반드시 "rb". Windows에서 텍스트 모드로 열면 0x0D 0x0A를 0x0A로 바꿔
     * ROM 바이트가 조용히 망가진다. 이건 실제 실행이 어긋나야 눈치채는
     * 종류의 버그라서 여기서 못 박아 둔다.
     */
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        fprintf(stderr, "오류: ROM 파일을 열 수 없다: %s\n", path);
        return 1;
    }

    /* 파일 끝으로 가서 크기를 잰다. */
    if (fseek(fp, 0L, SEEK_END) != 0) {
        fprintf(stderr, "오류: 파일 크기를 잴 수 없다: %s\n", path);
        fclose(fp);
        return 1;
    }

    long size = ftell(fp);
    if (size < 0) {
        fprintf(stderr, "오류: 파일 크기를 잴 수 없다: %s\n", path);
        fclose(fp);
        return 1;
    }
    if (size == 0) {
        fprintf(stderr, "오류: ROM 파일이 비어 있다: %s\n", path);
        fclose(fp);
        return 1;
    }
    rewind(fp);

    uint8_t *buf = malloc((size_t)size);
    if (buf == NULL) {
        fprintf(stderr, "오류: 메모리를 잡을 수 없다 (%ld 바이트)\n", size);
        fclose(fp);
        return 1;
    }

    size_t read = fread(buf, 1, (size_t)size, fp);
    fclose(fp);

    if (read != (size_t)size) {
        fprintf(stderr, "오류: %ld 바이트 중 %zu 바이트만 읽혔다: %s\n",
                size, read, path);
        free(buf);
        return 1;
    }

    out->data = buf;
    out->size = (size_t)size;
    return 0;
}

void rom_free(Rom *rom)
{
    if (rom == NULL) {
        return;
    }
    free(rom->data);
    rom->data = NULL;   /* 두 번 불러도 free(NULL)이라 안전하다 */
    rom->size = 0;
}

void rom_hexdump(const uint8_t *data, size_t size, uint16_t base_addr)
{
    for (size_t offset = 0; offset < size; offset += BYTES_PER_LINE) {
        /* 이 줄에 실제로 찍을 바이트 수 (마지막 줄은 16개보다 적을 수 있다) */
        size_t line_len = size - offset;
        if (line_len > BYTES_PER_LINE) {
            line_len = BYTES_PER_LINE;
        }

        /* 주소 */
        printf("%04X: ", (unsigned)(base_addr + offset));

        /* 16진수 열: 8바이트마다 한 칸 더 띄워 읽기 쉽게 한다 */
        for (size_t i = 0; i < BYTES_PER_LINE; i++) {
            if (i == BYTES_PER_LINE / 2) {
                putchar(' ');
            }
            if (i < line_len) {
                printf("%02X ", data[offset + i]);
            } else {
                printf("   ");   /* 마지막 줄 빈 자리: ASCII 열을 맞추려고 공백 */
            }
        }

        /* ASCII 열: 출력 가능한 문자만 그대로, 나머지는 '.' */
        printf(" |");
        for (size_t i = 0; i < line_len; i++) {
            uint8_t c = data[offset + i];
            /*
             * isprint()를 안 쓰고 직접 비교하는 이유: isprint는 로캘에 따라
             * 0x80 이상을 출력 가능하다고 볼 수 있어서 결과가 환경마다 달라진다.
             * ASCII 인쇄 가능 범위는 0x20(공백) ~ 0x7E(~)로 못 박는다.
             */
            putchar((c >= 0x20 && c <= 0x7E) ? (int)c : '.');
        }
        printf("|\n");
    }
}
