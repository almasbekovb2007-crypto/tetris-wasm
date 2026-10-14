#!/bin/bash
echo "======================================"
echo "  Компиляция Тетриса -> WebAssembly"
echo "======================================"

if ! command -v emcc &>/dev/null; then
  echo "[!] emcc не найден. Активируйте emsdk:"
  echo "    emsdk activate latest"
  echo "    source emsdk_env.sh"
  exit 1
fi

echo "[OK] emcc: $(emcc --version | head -1)"

emcc main.cpp \
  -O2 \
  -o index.js \
  -s WASM=1 \
  -s EXPORTED_RUNTIME_METHODS='["cwrap","ccall"]' \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s INITIAL_MEMORY=16777216 \
  -s NO_EXIT_RUNTIME=1 \
  -s FILESYSTEM=0 \
  -std=c++17

if [ $? -eq 0 ]; then
  echo ""
  echo "[OK] Готово!"
  ls -lh index.js index.wasm
  echo ""
  echo "Запуск: python -m http.server 8000"
  echo "Адрес:  http://localhost:8000"
else
  echo "[ОШИБКА] Компиляция не удалась"
fi
