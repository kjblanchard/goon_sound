cmake_minimum_required(VERSION 3.2)

#Build universal if we are on our m1 mac, also requires using xcode as the build engine when doing this.  -G Xcode.
# Must be called before project.
if (UNIX AND APPLE)
    set(CMAKE_OSX_ARCHITECTURES arm64;x86_64)
endif(UNIX AND APPLE)

project(goon_sound
    VERSION 0.1.0
    LANGUAGES C)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
#Add in the windows cmake path to prefix, so that we can find things.
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /c/cmake)
set(CMAKE_OSX_DEPLOYMENT_TARGET 12.3)

#Add all of our source files
file(GLOB FILES src/**/*.c)
# Build a win32 bundle when building on win, and macos when building on ios,  Remove macosbundle as its easier to copy the files and debug #Remove win32 as couldn't build with sdl2
add_library(goon_sound
    STATIC
    ${FILES})
set_property(TARGET goon_sound PROPERTY C_STANDARD 99)

add_subdirectory(external/openal-soft)
find_package(PkgConfig)

target_compile_definitions(goon_sound PRIVATE AL_LIBTYPE_STATIC )

#Potentially needed for windows / unix.
#find_package(VORBIS
#    NAMES vorbis Vorbis
#    PATH_SUFFIXES .a
#    COMPONENTS REQUIRED VORBISFILE VORBIS VORBISENC
#    REQUIRED)

find_library(LIB_VORBIS
    REQUIRED
    PATH_SUFFIXES .a
    NAMES libvorbis vorbis vorbis.a vorbis.dll libvorbis.a
    HINTS /usr/local/lib /c/cmake/lib)

find_library(LIB_VORBISFILE
    REQUIRED
    NAMES  libvorbisfile vorbisfile vorbisfile.a vorbisfile.dll
    HINTS /usr/local/lib)

find_package(
    OGG
    NAMES ogg Ogg
    PATH_SUFFIXES .a
    REQUIRED)

#For Debug, print aout all variables.
get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
   message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

#Order matters
target_link_libraries(goon_sound
    PUBLIC ${LIB_VORBISFILE} ${LIB_VORBIS} ${OGG_LIBRARIES} OpenAL
    INTERFACE )

target_include_directories(goon_sound
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/"
    )

target_compile_options(goon_sound PUBLIC -std=c99 -Wall  -pedantic
-DDEBUG_BUILD_ENABLED)

file(GLOB MAIN main.c)
add_executable(test MAIN)
target_link_libraries(test
    PUBLIC
    PRIVATE  goon_sound
)

target_include_directories(test
    PUBLIC
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )