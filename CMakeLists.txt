cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME opengl-portfolio)
set(CMAKE_CXX_STANDARD 17)

set(WINDOW_NAME "OpenGL portfolio")
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 540) 

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} 
    src/main.cc 
    src/common.cc
    src/shader.cc
)

include(Dependency.cmake)

target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME}  PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
    WINDOW_NAME="${WINDOW_NAME}"
    WINDOW_WIDTH=${WINDOW_WIDTH}
    WINDOW_HEIGHT=${WINDOW_HEIGHT}
)

add_dependencies(${PROJECT_NAME} ${DEP_LIST})
