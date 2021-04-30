cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

project(ShroonPrism)

add_subdirectory("external/glm/")

if (NOT EMSCRIPTEN)
    add_subdirectory("external/glfw/")
endif()

set(SPIRV_CROSS_CLI OFF CACHE BOOL "Build SPIRV-Cross without CLI.")
set(SPIRV_CROSS_ENABLE_TESTS OFF CACHE BOOL "Build SPIRV-Cross without tests.")

set(SPIRV_CROSS_ENABLE_HLSL OFF CACHE BOOL "Build SPIRV-Cross without HLSL output.")
set(SPIRV_CROSS_ENABLE_MSL OFF CACHE BOOL "Build SPIRV-Cross without MSL output.")
set(SPIRV_CROSS_ENABLE_CPP OFF CACHE BOOL "Build SPIRV-Cross without C++ output.")
set(SPIRV_CROSS_ENABLE_REFLECT OFF CACHE BOOL "Build SPIRV-Cross without JSON reflection.")
set(SPIRV_CROSS_ENABLE_C_API OFF CACHE BOOL "Build SPIRV-Cross without C API output.")

add_subdirectory("external/SPIRV-Cross")

file(GLOB_RECURSE HEADER_FILES "src/*.hpp" "src/hl/api/*.hpp" "src/hl/glfw/*.hpp" "src/hl/gl33/*.hpp" "src/hl/gl45/*.hpp" "src/hl/es30/*.hpp")
file(GLOB_RECURSE SOURCE_FILES "src/hl/api/*.cpp" "src/hl/glfw/*.cpp" "src/hl/gl33/*.cpp" "src/hl/gl45/*.cpp" "src/hl/es30/*.cpp")

file(GLOB SPIRV_CROSS_HEADER_FILES "external/SPIRV-Cross/*.hpp")
file(COPY ${SPIRV_CROSS_HEADER_FILES} DESTINATION "${CMAKE_BINARY_DIR}/external/SPIRV-Cross/include/spirv_cross/")

add_library(ShroonPrism ${HEADER_FILES} ${SOURCE_FILES} "${CMAKE_CURRENT_LIST_DIR}/external/glad/src/glad.c")

target_include_directories(ShroonPrism PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/src/"
    "${CMAKE_CURRENT_LIST_DIR}/external/glad/include/"
    "${CMAKE_BINARY_DIR}/external/SPIRV-Cross/include/"
    "${CMAKE_CURRENT_LIST_DIR}/external/glm/"
)

if (NOT EMSCRIPTEN)
    target_include_directories(ShroonPrism PUBLIC "${CMAKE_CURRENT_LIST_DIR}/external/glfw/include/")
endif()

install(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/src/"
    DESTINATION "${CMAKE_CURRENT_LIST_DIR}/include/Shroon/Prism/"
    FILES_MATCHING PATTERN "*.hpp"
)

file(COPY "${CMAKE_CURRENT_LIST_DIR}/src/" DESTINATION "${CMAKE_CURRENT_LIST_DIR}/include/Shroon/Prism/" FILES_MATCHING PATTERN "*.hpp")

target_link_libraries(ShroonPrism spirv-cross-core spirv-cross-glsl GL GLESv2 glfw)

add_executable(PrismTest "${CMAKE_CURRENT_LIST_DIR}/example/main.cpp")
target_include_directories(PrismTest PUBLIC ShroonPrism "${CMAKE_CURRENT_LIST_DIR}/include/" "${CMAKE_CURRENT_LIST_DIR}/include/Shroon/Prism")
target_link_libraries(PrismTest ShroonPrism)
set_target_properties(PrismTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../example")

if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s MIN_WEBGL_VERSION=1 -s MAX_WEBGL_VERSION=2 -s FULL_ES2=1 -s FULL_ES3=1 -s WASM_ASYNC_COMPILATION=0 -s EXIT_RUNTIME=1 --bind -lidbfs.js")
endif()
