cmake_minimum_required(VERSION 3.0.0)
project(libuv-tutorial VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include(CTest)
enable_testing()

link_directories(/usr/local/lib)
include_directories(/usr/local/include)
link_libraries(uv pthread dl rt hiredis)

add_subdirectory(basic)
add_subdirectory(async)
add_subdirectory(prepare)
add_subdirectory(redis)
# add_subdirectory(filesystem)

add_executable(hello main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

