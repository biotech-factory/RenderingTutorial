# 실행 절차

## 1. 프로젝트 루트로 이동
cd "C:\GIthub_workspace\RenderingTutorial\RenderingTutorial\RenderingTutorial"

## 2. A 일반 재구성
cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake `
  -DVCPKG_TARGET_TRIPLET=x64-windows

## 2. B 강제 초기화 재구성
if (Test-Path build) { Remove-Item -Recurse -Force build }

cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE:FILEPATH=C:/vcpkg/scripts/buildsystems/vcpkg.cmake `
  -DVCPKG_TARGET_TRIPLET=x64-windows


## 3. 빌드
cmake --build build --config Debug

## 4. 실행 파일 찾기
dir .\build -Recurse -Filter *.exe

## 5. 실행
./build/Debug/RenderingTutorial.exe
