cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

project(ShroonPrism)

add_subdirectory("external/glm/")
add_subdirectory("external/glfw/")

set(SPIRV_CROSS_CLI OFF CACHE BOOL "Build SPIRV-Cross without CLI.")
set(SPIRV_CROSS_ENABLE_TESTS OFF CACHE BOOL "Build SPIRV-Cross without tests.")

set(SPIRV_CROSS_ENABLE_HLSL OFF CACHE BOOL "Build SPIRV-Cross without HLSL output.")
set(SPIRV_CROSS_ENABLE_MSL OFF CACHE BOOL "Build SPIRV-Cross without MSL output.")
set(SPIRV_CROSS_ENABLE_CPP OFF CACHE BOOL "Build SPIRV-Cross without C++ output.")
set(SPIRV_CROSS_ENABLE_REFLECT OFF CACHE BOOL "Build SPIRV-Cross without JSON reflection.")
set(SPIRV_CROSS_ENABLE_C_API OFF CACHE BOOL "Build SPIRV-Cross without C API output.")

#add_subdirectory("external/SPIRV-Cross/")
#"src/gl33/*.hpp" "src/gl45/*.hpp"
#"src/gl33/*.cpp" "src/gl45/*.cpp"
file(GLOB_RECURSE HEADER_FILES "src/*.hpp" "src/api/*.hpp"  "src/vk12/*.hpp")
file(GLOB_RECURSE SOURCE_FILES "src/api/*.cpp"  "src/vk12/*.cpp")

add_library(ShroonPrism ${HEADER_FILES} ${SOURCE_FILES} "${CMAKE_CURRENT_LIST_DIR}/external/glad/src/glad.c")

target_include_directories(ShroonPrism PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/src/"
    "${CMAKE_CURRENT_LIST_DIR}/external/glad/include/"
    #"${CMAKE_CURRENT_LIST_DIR}/external/SPIRV-Cross/include/"
    "${CMAKE_CURRENT_LIST_DIR}/external/glm/"
    "${CMAKE_CURRENT_LIST_DIR}/external/glfw/include/"
)

install(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/src/"
    DESTINATION "${CMAKE_CURRENT_LIST_DIR}/include/Shroon/Prism/"
    FILES_MATCHING PATTERN "*.hpp"
)

file(COPY "${CMAKE_CURRENT_LIST_DIR}/src/" DESTINATION "${CMAKE_CURRENT_LIST_DIR}/include/Shroon/Prism/" FILES_MATCHING PATTERN "*.hpp")

target_link_libraries(ShroonPrism spirv-cross-core spirv-cross-glsl GL glfw)

add_executable(PrismTest "${CMAKE_CURRENT_LIST_DIR}/example/main.cpp")
target_include_directories(PrismTest PUBLIC ShroonPrism "${CMAKE_CURRENT_LIST_DIR}/include/" "${CMAKE_CURRENT_LIST_DIR}/include/Shroon/Prism")
target_link_libraries(PrismTest ShroonPrism)
