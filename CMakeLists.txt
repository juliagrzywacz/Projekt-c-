cmake_minimum_required(VERSION 3.28)
project(App)

# Ustawienie standardu C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ścieżka do biblioteki Qt
set(CMAKE_PREFIX_PATH "C:/Qt/6.7.1/mingw_64/lib/cmake")

# Znalezienie modułu Qt6 Widgets i Sql
# find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)

# Źródła projektu
set(SOURCES
        main.cpp
        WeekView.cpp
        database.cpp
        TaskWindow.cpp
        TaskEditWindow.cpp

)

# Nagłówki projektu
set(HEADERS
        WeekView.h
        database.h
        TaskWindow.h
        TaskEditWindow.h
)

# Generowanie plików MOC (Meta-Object Compiler)
qt6_wrap_cpp(MOC_FILES ${HEADERS})

# Tworzenie pliku wykonywalnego
add_executable(App ${SOURCES} ${MOC_FILES})
#add_executable(App main.cpp database.cpp database.h)

# Powiązanie pliku wykonywalnego z biblioteką Qt Widgets i Sql
target_link_libraries(App PRIVATE Qt6::Core Qt6::Widgets Qt6::Sql)
