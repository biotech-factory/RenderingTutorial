cmake_minimum_required(VERSION 3.21)
project(RenderingTutorial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 스크립트 추가시 경로 설정 필요
add_executable(RenderingTutorial
    src/main.cpp
    src/WindowSDL.h
    src/WindowSDL.cpp
    src/RendererD3D11.h
    src/RendererD3D11.cpp
)
# SDL2 (vcpkg가 find_package를 제공)
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(RenderingTutorial PRIVATE SDL2::SDL2 SDL2::SDL2main)



# DirectX 11 (Windows SDK)
target_link_libraries(RenderingTutorial PRIVATE d3d11 dxgi d3dcompiler)

# Windows에선 보통 UNICODE가 기본이므로 명시해두면 혼선이 줄어듦
target_compile_definitions(RenderingTutorial PRIVATE UNICODE _UNICODE)

# 디버그 빌드에서 경고/디버그 편의
if (MSVC)
  target_compile_options(RenderingTutorial PRIVATE /W4 /permissive-)
endif()
