# Projekt C++ - Aplikacja Kalendarza

## Opis projektu
Aplikacja kalendarza tygodniowego napisana w C++ z użyciem biblioteki Qt. Umożliwia dodawanie, edytowanie i usuwanie zadań. Projekt wykorzystuje CMake do konfiguracji kompilacji.

## Wymagania systemowe
Aplikacja jest przeznaczona do uruchomienia na systemie **Windows** i wymaga następujących komponentów:
- **CMake** (wersja 3.10 lub nowsza)
- **Qt** (wersja 5.15 lub nowsza)
- **Microsoft Visual Studio (MSVC) lub MinGW** (zalecane użycie CLion)
- **Narzędzia deweloperskie Visual Studio (C++ Build Tools)** – jeśli używasz MSVC

## Instalacja wymaganych komponentów
1. **Zainstaluj Qt**  
   Pobierz i zainstaluj [Qt](https://www.qt.io/download) (zalecana wersja 5.15 lub nowsza).  
   Podczas instalacji upewnij się, że wybierasz moduły `msvc` lub `mingw` (zgodne z Twoim kompilatorem).  
   Po instalacji ustaw zmienną `CMAKE_PREFIX_PATH`:
   ```powershell
   $env:CMAKE_PREFIX_PATH = "C:\Qt\5.15.2\mingw81_64"
   ```
   lub dla MSVC:
   ```powershell
   $env:CMAKE_PREFIX_PATH = "C:\Qt\5.15.2\msvc2019_64"
   ```

2. **Zainstaluj CMake**  
   Pobierz [CMake](https://cmake.org/download/) i dodaj go do zmiennej środowiskowej `PATH` (opcjonalnie).

3. **Zainstaluj MinGW lub Visual Studio**  
   - Jeśli używasz MinGW: Zainstaluj `mingw-w64` i upewnij się, że `mingw32-make` jest dostępny w `PATH`.
   - Jeśli używasz MSVC: Pobierz `Visual Studio Community` i zainstaluj `C++ Build Tools`.

## Kompilacja projektu na Windows
1. Otwórz terminal **PowerShell** lub **Developer Command Prompt for VS**.
2. Skonfiguruj CMake:
   ```powershell
   cmake -B build -G "MinGW Makefiles"
   ```
   lub dla Visual Studio:
   ```powershell
   cmake -B build -G "Visual Studio 17 2022"
   ```
3. Przejdź do katalogu `build` i skompiluj aplikację:
   ```powershell
   cd build
   cmake --build .
   ```

## Uruchomienie aplikacji
Po zakończeniu kompilacji plik wykonywalny znajdziesz w katalogu `build`. Możesz uruchomić go poleceniem:
   ```powershell
   build\App.exe
   ```
Jeśli aplikacja nie uruchamia się z powodu brakujących bibliotek Qt, skopiuj pliki DLL do katalogu aplikacji lub użyj narzędzia `windeployqt`:
   ```powershell
   C:\Qt\5.15.2\mingw81_64\bin\windeployqt.exe build\App.exe
   ```

## Struktura projektu
```
Projekt-C++
├── CMakeLists.txt        # Plik konfiguracyjny CMake
├── main.cpp              # Główny plik aplikacji
├── database.cpp/.h       # Obsługa bazy danych
├── WeekView.cpp/.h       # Widok kalendarza
├── TaskWindow.cpp/.h     # Okno dodawania/edycji zadań
├── TaskEditWindow.cpp/.h # Okno edycji zadań
├── resources.qrc         # Plik zasobów Qt (ikony, obrazy)
├── icons/                # Folder z ikonami
├── README.md             # Dokumentacja
```
## Autor
Projekt stworzony przez Julię Grzywacz w ramach przedmiotu "Projekt C++".

