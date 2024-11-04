cmake_minimum_required(VERSION 3.28)
project(App)

# Ustawienie standardu C++
set(CMAKE_CXX_STANDARD 17)

# Ścieżka do biblioteki Qt
set(CMAKE_PREFIX_PATH "C:/Qt/6.7.1/mingw_64/lib/cmake")

# Znalezienie modułu Qt6 Widgets
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Źródła projektu
set(SOURCES
        main.cpp
        WeekView.cpp
)

# Nagłówki projektu
set(HEADERS
        WeekView.h
)

# Generowanie plików MOC (Meta-Object Compiler)
qt6_wrap_cpp(MOC_FILES ${HEADERS})

# Tworzenie pliku wykonywalnego
add_executable(App ${SOURCES} ${MOC_FILES})

# Powiązanie pliku wykonywalnego z biblioteką Qt Widgets
target_link_libraries(App Qt6::Widgets)
