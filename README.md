# Projekt C++ - Aplikacja Kalendarza
## Opis projektu
Aplikacja kalendarza tygodniowego napisana w C++ z użyciem biblioteki Qt. Umożliwia dodawanie, edytowanie i usuwanie zadań. Projekt wykorzystuje CMake do konfiguracji kompilacji.
## Wymagania systemowe
Aplikacja jest przeznaczona do uruchomienia na systemie **Windows** i wymaga następujących komponentów:
- **CMake** (wersja 3.10 lub nowsza)
- **Qt** (wersja 5.15 lub nowsza)
- **Microsoft Visual Studio (MSVC) lub MinGW** (zalecane użycie CLion)
## Instalacja wymaganych komponentów
1. **Zainstaluj Qt**  
   Pobierz i zainstaluj [Qt](https://www.qt.io/download) (zalecana wersja 5.15 lub nowsza).  
   Podczas instalacji upewnij się, że wybierasz moduły `msvc` lub `mingw` (zgodne z Twoim kompilatorem).
2. **Zainstaluj CMake**  
   Pobierz [CMake](https://cmake.org/download/) i dodaj go do zmiennej środowiskowej `PATH` (opcjonalnie).
## Kompilacja projektu na Windows
1. Otwórz terminal **PowerShell** lub **Developer Command Prompt for VS**.
2. Skonfiguruj CMake:
   ```powershell
   cmake -B build
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
## Autorzy
Projekt stworzony przez Julię w ramach przedmiotu "Projekt C++".