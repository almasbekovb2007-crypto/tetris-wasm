# Тетрис — WebAssembly (C++ + HTML5)

## Структура проекта

```
tetris-wasm/
├── main.cpp        ← Вся игровая логика на C++
├── index.html      ← UI, Canvas-рендер, JS-мост к WASM
├── index.js        ← Генерируется emcc (не редактировать)
├── index.wasm      ← Генерируется emcc (не редактировать)
├── build.sh        ← Скрипт компиляции
└── README.md
```

## Шаг 1 — Компиляция

```bash
# Активировать emsdk (один раз)
source ~/emsdk/emsdk_env.sh

# Перейти в папку проекта
cd tetris-wasm

# Дать права и запустить сборку
chmod +x build.sh
./build.sh
```

Или напрямую:

```bash
emcc main.cpp -O2 -o index.js \
  -s WASM=1 \
  -s EXPORTED_RUNTIME_METHODS='["cwrap","ccall"]' \
  -s MODULARIZE=0 \
  -s EXPORT_NAME="Module" \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s INITIAL_MEMORY=16MB \
  -s NO_EXIT_RUNTIME=1 \
  -s ENVIRONMENT='web' \
  -s FILESYSTEM=0 \
  --no-entry \
  -std=c++17
```

## Шаг 2 — Запуск

```bash
python3 -m http.server 8000
```

Открыть браузер: **http://localhost:8000**

---

## Архитектура C++ (main.cpp)

### Классы и структуры

| Компонент | Описание |
|-----------|----------|
| `grid[MAX_H][MAX_W]` | Игровое поле — 2D массив |
| `struct Piece` | Фигура: блоки, позиция, цвет |
| `can_place()` | Проверка коллизий |
| `lock_piece_and_clear()` | Фиксация + очистка линий |
| `make_piece()` | Генерация фигур (4-7 блоков) |
| `update_ghost()` | Ghost piece (куда упадёт) |

### Экспортируемые функции (вызываются из JS через cwrap)

```
game_init(w, h, piece_sz)  — запуск новой игры
game_update(now_ms)        — обновление (1 кадр) → 0/1/2
game_restart()             — перезапуск

action_left/right/down()   — управление
action_rotate()            — поворот с kick-таблицей
action_hard_drop()         — мгновенное падение
action_pause()             — пауза/продолжение

get_cell(r, c)             — ячейка поля: 0/1..7/-1(ghost)
get_cur_block_r/c(i)       — блоки текущей фигуры
get_next_block_r/c(i)      — блоки следующей фигуры
get_score/lines/level()    — состояние игры
```

---

## Что реализовано по ТЗ

- ✅ Настройка размера поля (6-20 × 10-30)
- ✅ Фигуры 4, 5, 6, 7 блоков (тетро/пенто/гексо/гептомино)
- ✅ Автоускорение: каждые 10 линий уровень растёт
- ✅ Ghost piece — показ куда упадёт фигура
- ✅ Следующая фигура (preview)
- ✅ Очки: блок фигуры = 1 очко + бонус за линии
- ✅ Пауза / продолжение
- ✅ Выход в меню с подтверждением
- ✅ Game Over — обнаружение, вывод счёта и рекорда
- ✅ Рекорды для каждого размера поля (localStorage)
- ✅ Настройки: звук, музыка, тема, язык, клавиши
- ✅ Переназначение клавиш
- ✅ Тёмная / светлая тема
- ✅ 3 языка: RU / EN / KG
- ✅ Мобильные кнопки управления (touch)
- ✅ Звуковые эффекты (Web Audio API)
- ✅ Фоновая музыка (процедурная)
- ✅ Hard drop (Space)
- ✅ Soft drop (↓)
- ✅ Kick-таблица при вращении у стен

## Управление (по умолчанию)

| Клавиша | Действие |
|---------|----------|
| ← → | Движение |
| ↑ | Поворот |
| ↓ | Ускорение |
| Space | Hard Drop |
| P | Пауза |
