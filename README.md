# emu-lab

CHIP-8 인터프리터에서 시작해 Game Boy(DMG) 에뮬레이터까지 직접 만들어 보는 학습 프로젝트.

- 목표: CPU·메모리·기계어 동작을 구현하며 이해하고, 리버스 엔지니어링·펌웨어 분석으로 연결한다.
- 스택: C11, SDL2, CMake (Windows, MSYS2 UCRT64 툴체인)
- 진행 상황: [docs/ROADMAP.md](docs/ROADMAP.md)
- 개발 일지: [docs/DEVLOG.md](docs/DEVLOG.md)

## 빌드
```
cmake -B build -G Ninja
cmake --build build
./build/emu.exe roms/<파일명>.ch8
```

## ROM
이 저장소에는 ROM이 포함되어 있지 않다. 테스트 ROM은 `docs/references.md`의 링크에서 받아 `roms/`에 넣는다.
