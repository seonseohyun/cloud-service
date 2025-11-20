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
add_executable(cloud_server
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/AppSever.cpp
    ${SRC_DIR}/Router.cpp
    ${SRC_DIR}/AuthController.cpp
    ${SRC_DIR}/DBManager.cpp
)

# 헤더 찾을 경로 등록
target_include_directories(cloud_server PRIVATE
    ${INC_DIR}
)

# === MariaDB Connector/C 설정 ===

# MariaDB 설치 루트
set(MARIADB_ROOT /opt/homebrew/Cellar/mariadb-connector-c/3.4.7)

# 헤더 경로
target_include_directories(cloud_server
    PRIVATE
        ${MARIADB_ROOT}/include
)

# 라이브러리 파일 “직접” 지정
target_link_libraries(cloud_server
    PRIVATE
        ${MARIADB_ROOT}/lib/libmariadb.3.dylib
)
