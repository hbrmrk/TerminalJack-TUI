cmake_minimum_required(VERSION 4.1)

# A projekt neve
project(TerminalJack)

# C++ szabvány beállítása (Az FTXUI-hoz minimum C++17 szükséges)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Lépés: Az FTXUI letöltése a GitHubról
include(FetchContent)
FetchContent_Declare(
        ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
        GIT_TAG main # A legfrissebb verziót húzzuk le
)
FetchContent_MakeAvailable(ftxui)

# 2. Lépés: A futtatható program (executable) létrehozása a fájljaidból
add_executable(TerminalJack main.cpp Card.cpp Card.h)

# 3. Lépés: Az FTXUI modulok hozzácsatolása (linkelése) a programodhoz
target_link_libraries(TerminalJack
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
)
