cmake_minimum_required(VERSION 3.12)

project(simviz)

#Base flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall")

set(INCPATHS "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/Resources")

set(DYLIBS "")
#find_package(PkgConfig REQUIRED)

#pkg_search_module(GLFW REQUIRED glfw3)
find_package(glfw3 REQUIRED)
list(APPEND INCPATHS ${GLFW_INCLUDE_DIRS})
list(APPEND DYLIBS glfw)
find_package(OpenCL REQUIRED)
if(OpenCL_FOUND)
    list(APPEND DYLIBS OpenCL::OpenCL)
endif()
find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
    list(APPEND DYLIBS OpenGL::GL)
    if(APPLE)
        find_package(GLEW REQUIRED)
        list(APPEND DYLIBS GLEW::GLEW)
    endif()
endif()

FILE(GLOB SOURCES Resources/*.cpp *.cpp)
add_executable(pnj ${SOURCES})
target_include_directories(pnj PUBLIC ${INCPATHS})

target_link_libraries(pnj PRIVATE ${DYLIBS})

