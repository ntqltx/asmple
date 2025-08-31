cmake_minimum_required(VERSION 3.16)
project(asmple
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_ROOT lang)
set(FRONTEND_SRC
    ${SRC_ROOT}/frontend/lexer.cpp
    ${SRC_ROOT}/frontend/parser.cpp
)
set(BACKEND_SRC
    ${SRC_ROOT}/backend/interpreter.cpp
)

set(SOURCES
    ${FRONTEND_SRC}
    ${BACKEND_SRC}
    lang/main.cpp
)
include_directories(
    ${SRC_ROOT}/frontend
    ${SRC_ROOT}/backend
    ${SRC_ROOT}/common
)
add_executable(asmple ${SOURCES})

if (MSVC)
    target_compile_options(asmple PRIVATE /W4)
else()
    target_compile_options(asmple PRIVATE -Wall -Wextra -pedantic)
endif()

# // ENABLE TESTING HERE
# enable_testing()
# add_subdirectory(tests)