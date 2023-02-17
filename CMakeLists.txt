cmake_minimum_required(VERSION 3.21)
project(search_for_AT)

set(CMAKE_CXX_STANDARD 20)

if(WIN32) # Install dlls in the same directory as the executable on Windows
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

set(CMAKE_RC_COMPILER windres.exe)

include(FetchContent)
FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG e53437b79965e5de9e9bef5a3e7ec609587268d6
)
FetchContent_MakeAvailable(cpr)

add_executable(parser parser.cpp ATParser.h ATParser.cpp BookInfo.h BookInfo.cpp)
target_link_libraries(parser PRIVATE cpr::cpr)

add_executable(Normalization Normalization.cpp utf8.cpp utf8.h Normalization.h)
add_executable(CreateNoramalTable CreateNoramalizationTable.cpp)
add_executable(test OpenCorporaDict.cpp DATrie.cpp BookInfo.cpp BookInfo.h)