# 로드맵

## Phase 0 — 환경 준비
- [ ] MSYS2 설치, UCRT64 툴체인(gcc, cmake, ninja, SDL2) 설치, PATH 설정
- [ ] CMake 빌드 구조, SDL2 창 띄우기
- [ ] ROM 파일 읽기 + hex dump 출력
- [ ] git 저장소 초기화

## Phase 1 — CHIP-8
- [ ] `Chip8` 상태 구조체 설계 (메모리 4KB, V0~VF, I, PC, 스택, 타이머, 화면, 키)
- [ ] ROM을 0x200에 로드, 폰트 데이터를 메모리에 배치
- [ ] fetch-decode-execute 루프
- [ ] 첫 6개 명령어(00E0, 1NNN, 6XNN, 7XNN, ANNN, DXYN) → IBM 로고 ROM 표시
- [ ] 나머지 명령어 구현
- [ ] 딜레이·사운드 타이머 (60Hz)
- [ ] 키 입력 매핑
- [ ] CHIP-8 테스트 스위트 통과
- [ ] 비프음

## Phase 2 — CHIP-8 디버거 (리버싱 연결)
- [ ] 디스어셈블러 (ROM → 니모닉 출력)
- [ ] 한 명령어씩 실행(step), 레지스터 출력
- [ ] 브레이크포인트
- [ ] 홈브루 ROM 하나를 디스어셈블해서 동작 분석해 보기

## Phase 3 — Game Boy CPU (SM83)
- [ ] 레지스터·플래그(Z N H C) 구조
- [ ] 명령어 테이블 구현 (기본 + CB 접두 명령어)
- [ ] Gameboy Doctor 로그 비교
- [ ] Blargg `cpu_instrs` 통과

## Phase 4 — 메모리·타이머·인터럽트
- [ ] 메모리 맵, 카트리지 헤더 읽기
- [ ] MBC1
- [ ] 타이머 (DIV, TIMA)
- [ ] 인터럽트 (IE, IF, IME)

## Phase 5 — 그래픽 (PPU)
- [ ] 타일·배경 렌더링
- [ ] 윈도우, 스프라이트
- [ ] PPU 모드·타이밍
- [ ] dmg-acid2 통과

## Phase 6 — 입력·사운드
- [ ] 조이패드
- [ ] APU (사운드)

## Phase 7 — 확장
- [ ] Game Boy 디버거 (디스어셈블러, 메모리 뷰어, 브레이크포인트)
- [ ] WebAssembly 빌드
- [ ] ARM 계열(GBA) 탐색 → 펌웨어 리버싱으로 연결
