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

add_executable(parser ATparser_main.cpp ATParser.h ATParser.cpp BookInfo.h BookInfo.cpp nlohmann/json.hpp)
target_link_libraries(parser PRIVATE cpr::cpr)
target_include_directories(parser PUBLIC nlohmann)

add_executable(Normalization utf8.h utf8.cpp Normalization.h Normalization.cpp NormalizationTable.cpp)
add_executable(CreateNoramalTable CreateNoramalizationTable.cpp NormalizationTable.h)
add_executable(OpenCorporaDict OpenCorporaDict.cpp pugixml/pugixml.hpp pugixml/pugixml.cpp utf8.h utf8.cpp DATrie.h DATrie.cpp Normalization.h Normalization.cpp NormalizationTable.h NormalizationTable.cpp)
target_include_directories(OpenCorporaDict PUBLIC pugixml)
