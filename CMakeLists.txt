cmake_minimum_required(VERSION 3.5)
project(PassCrackViz LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(${OPENSSL_INCLUDE_DIR})

add_executable(PassCrackViz
    src/main.cpp
    src/UIManager.h
    src/UIManager.cpp
    src/HashUtils.h
    src/HashUtils.cpp
    src/DictionaryCracker.h
    src/DictionaryCracker.cpp
    src/BruteForceCracker.h
    src/BruteForceCracker.cpp
)

target_link_libraries(PassCrackViz
    Qt5::Widgets
    Qt5::Core
    ${OPENSSL_LIBRARIES}
)

