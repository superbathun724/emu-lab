CLAUDE.md, docs/ROADMAP.md, docs/references.md를 먼저 읽고 이 프로젝트의 규칙, 특히 역할 분담을 파악해줘.

지금부터 Phase 0을 시작하려고 해. 해줄 일은 다음과 같아.

1. 개발 환경 점검
   Windows에서 MSYS2 UCRT64 툴체인(gcc, cmake, ninja, SDL2)이 설치되어 있고
   Git Bash에서 PATH로 잡히는지 확인해줘(`gcc --version`, `cmake --version`, `sdl2-config --version` 등).
   없는 게 있으면 MSYS2 UCRT64 터미널에서 실행할 pacman 명령어와 PATH 설정 방법을 알려줘.
   설치는 내가 직접 할게.

2. 빌드 구조
   CMake + Ninja로 `emu.exe`를 만드는 구조를 잡아줘. `-Wall -Wextra`, C11 기준.
   CLAUDE.md의 Windows 주의사항(SDL2main, SDL2.dll 복사, 콘솔 유지)을 반영해줘.

3. 플랫폼 코드 (src/platform/)
   SDL2 창을 띄워줘. CHIP-8 화면(64x32)을 10배 확대한 크기로.
   약 60Hz로 도는 메인 루프, 키를 누르면 어떤 키인지 콘솔에 출력, ESC나 창 닫기로 종료.

4. ROM 로딩과 hex dump
   `./build/emu.exe <ROM 경로>`로 실행하면 ROM 파일을 바이너리 모드로 읽어 바이트 배열에 담고,
   `--dump` 옵션을 주면 주소와 함께 16바이트씩 hex dump를 출력하게 해줘.

5. 코어 자리만 만들기 (src/chip8/)
   chip8.h와 chip8.c 파일만 만들어줘. `Chip8` 구조체의 필드는 내가 직접 설계할 거니까
   구조체 안에는 어떤 정보가 필요한지 힌트 주석만 적고 필드는 비워둬.
   함수도 시그니처와 `// TODO:` 주석만 남겨줘.

6. git init 후 첫 커밋 메시지를 제안해줘.

작업을 시작하기 전에 전체 계획과 만들 파일 목록을 먼저 보여주고 내 확인을 받아줘.
모두 끝나면 다음을 알려줘.
- 빌드·실행 방법
- 내가 다음에 직접 해야 할 일(Chip8 구조체 설계)을 위해 스펙 문서의 어느 부분을 읽으면 되는지
- docs/ROADMAP.md에서 체크할 항목
