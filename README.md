# Systemy operacyjne 2 - Projekt
## Problem filozofów przy stole – implementacja w C++

Ten projekt przedstawia rozwiązanie klasycznego [problemu filozofów przy stole](https://pl.wikipedia.org/wiki/Problem_ucztuj%C4%85cych_filozof%C3%B3w) z wykorzystaniem wątków w języku C++.

---

## Opis problemu

### Problem ucztujących filozofów

Problem ucztujących filozofów to klasyczny problem synchronizacji w programowaniu współbieżnym, zaproponowany przez Edsgera Dijkstrę. Dla wizualizacji problemu przedstawia się go w formie kilku filozofów siedzących przy okrągłym stole. Każdy z nich na przemian myśli i je. Aby zjeść, filozof musi zdobyć dwa widelce – jeden z lewej i jeden z prawej strony. Widelce są wspólne z sąsiadami, więc dwóch sąsiednich filozofów nie może jeść jednocześnie.

Celem projektu jest zapewnienie takiej synchronizacji, aby:

- żaden filozof nie głodował,
- nie doszło do zakleszczenia (deadlocku),
- nie było błędów dostępu współbieżnego (race condition),
- filozofowie mogli bezpiecznie korzystać ze współdzielonych zasobów (widełek).

#### Sekcje krytyczne i ich ochrona

Sekcje krytyczne to fragmenty kodu, które operują na danych współdzielonych przez wiele wątków – w tym przypadku:
- wektor stanów (`states[]`) każdego filozofa (THINKING, HUNGRY, EATING),
- warunki synchronizacji (zmienne warunkowe),
- mutexy chroniące wypisywanie komunikatów i stan filozofów.

W programie zastosowano następujące mechanizmy synchronizacji:

| Element           | Mechanizm synchronizacji  | Rola                                                                 |
|-------------------|---------------------------|----------------------------------------------------------------------|
| `states[]`        | `std::mutex stateMutex`   | Chroni przed jednoczesnym dostępem przy zmianie stanów filozofów     |
| `std::cout`       | `std::mutex messageMTX`   | Zapobiega mieszaniu się wypisywanych komunikatów z różnych wątków    |
| Dostęp do widełek | `std::condition_variable` | Filozof czeka, dopóki jego sąsiedzi nie jedzą i nie blokują widelców |

Każdy filozof zmienia swój stan oraz testuje sąsiadów wewnątrz zablokowanej sekcji `stateMutex`, co zapobiega warunkom wyścigu. Dostęp do zasobów (widełek) jest kontrolowany logicznie przez stany i zmienne warunkowe, a nie przez jawne blokowanie mutexów od widełek – co pozwala uniknąć deadlocku.

#### Wątki w programie

W programie uruchamianych jest tyle wątków, ilu jest filozofów. Każdy wątek:

- reprezentuje jednego filozofa,
- wykonuje metodę `run()` w nieskończonej pętli,
- przechodzi przez cykl: myślenie -> próba zdobycia widelców -> jedzenie -> odłożenie widelców.

Każdy filozof działa niezależnie i współbieżnie z innymi, ale synchronizuje dostęp do współdzielonych zasobów.

---

## Budowanie i uruchamianie

### Wymagania:
- CMake ≥ 3.29
- Kompilator z obsługą C++20

### Budowanie projektu:
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```
Domyślny generator zostanie wybrany automatycznie na podstawie środowiska. Na Windowsie może być konieczne użycie `-G "MinGW Makefiles"` lub `-G "NMake Makefiles"` w zależności od dostępnych narzędzi.

### Kompilacja ręczna:
```bash
g++ -std=c++20 main.cpp philosopher.cpp -o philosophers -pthread
```
### Sposób użycia:

```bash
./systemy-operacyjne [liczba_filozofów] [czas_myślenia_ms] [czas_jedzenia_ms]
```

### Przykłady:
```bash
./systemy-operacyjne           # domyślnie: 5 filozofów, 1000ms, 1000ms
./systemy-operacyjne 6         # 6 filozofów, 1000ms, 1000ms
./systemy-operacyjne 5 800 600 # 5 filozofów, 800ms myślenia, 600ms jedzenia
```

