cmake_minimum_required(VERSION 3.10)
project(cloud_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 소스 / 헤더 / 라이브러리 폴더 경로
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

# cpp 파일 전체 모으기
file(GLOB_RECURSE SOURCES
    ${SRC_DIR}/*.cpp
)

# 실행 파일
add_executable(cloud_server ${SOURCES})

# 헤더 찾을 경로 등록
target_include_directories(cloud_server PRIVATE
    ${INC_DIR}
    ${LIB_DIR}
)
