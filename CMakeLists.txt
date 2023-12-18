cmake_minimum_required(VERSION 3.5)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VITASDK})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
    else()
        message(FATAL_ERROR "Please defined VITASDK to point to your SDK path!")
    endif()
endif()

project(psas-hooks)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O3 -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")

link_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} src/psas_hooks.c src/detours.c src/hooks.c include/hooks.h)
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME}
    taihen_stub
    SceLibc_stub
    SceKernelModulemgr_stub
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES LINK_FLAGS "-nostdlib"
)

vita_create_self(${PROJECT_NAME}.suprx ${PROJECT_NAME}
    CONFIG ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.yml
)