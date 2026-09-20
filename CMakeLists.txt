cmake_minimum_required(VERSION 3.20)
project(emu_lab C)

# --- 언어 표준 -------------------------------------------------------------
# C11 고정. GNU 확장은 끈다(CMAKE_C_EXTENSIONS OFF → -std=c11).
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# 빌드 타입을 안 주면 Debug로. 디버거·트레이스를 자주 쓸 프로젝트라서.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "빌드 타입" FORCE)
endif()

# 경고는 전부 켜고 남겨두지 않는다.
add_compile_options(-Wall -Wextra)

find_package(SDL2 REQUIRED)

# --- 코어: CHIP-8 (SDL 비의존) ---------------------------------------------
# 일부러 정적 라이브러리로 떼어 둔다. 나중에 테스트·디스어셈블러·WebAssembly
# 빌드가 SDL 없이 코어만 링크할 수 있어야 하기 때문.
add_library(chip8_core STATIC
  src/chip8/chip8.c
)
target_include_directories(chip8_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# --- 실행 파일: emu.exe ----------------------------------------------------
add_executable(emu
  src/main.c
  src/platform/rom.c
  src/platform/window.c
)
target_include_directories(emu PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# SDL2main을 링크하지 않는 이유:
# MSYS2의 SDL2::SDL2main은 인터페이스에 -mwindows를 끌고 들어와 콘솔 창을 없앤다.
# 디버그 출력을 콘솔에서 봐야 하므로, 평범한 main()을 그대로 쓰고
# SDL_MAIN_HANDLED + SDL_SetMainReady() 조합을 택한다.
target_compile_definitions(emu PRIVATE SDL_MAIN_HANDLED)
target_link_libraries(emu PRIVATE chip8_core SDL2::SDL2)

# 빌드 후 SDL2.dll을 실행 파일 옆으로 복사 (Windows에서 실행하려면 필요).
add_custom_command(TARGET emu POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:SDL2::SDL2> $<TARGET_FILE_DIR:emu>
  COMMENT "SDL2.dll을 실행 파일 옆으로 복사"
)
